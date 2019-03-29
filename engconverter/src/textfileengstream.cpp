/*
 * Citybuilding tools: a set of utilities to work with the game files
 * Copyright (c) 2017  Bianca van Schaik <bvschaik@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "textfileengstream.h"

#include <QTextCodec>

enum {
    /**
     * Total index entries in the file.
     */
    MAX_INDEX_ENTRIES = 1000,
    /**
     * Maximum size of the data part of the file.
     * 1 MB should be more than enough: max original data size is 250 kB.
     */
    MAX_DATA_SIZE = 1000000
};

bool TextFileEngStream::read(TextFile &file, QIODevice &device, const QString &encoding, Logger &logger)
{
    if (!device.open(QIODevice::ReadOnly)) {
        logger.error(QString("Unable to open ENG file for reading: %1").arg(device.errorString()));
        return false;
    }
    
    QDataStream stream(&device);
    prepareDataStream(stream, encoding);

    bool result = readFile(file, stream, logger);
    
    device.close();
    return result;
}

bool TextFileEngStream::readFile(TextFile &file, QDataStream &stream, Logger &logger)
{
    // header
    char rawName[17];
    stream.readRawData(rawName, 16);
    rawName[16] = 0; // make sure we have an end to the string
    file.m_name = QString::fromAscii(rawName);
    
    // skip: max used entry, total strings, total words
    // --> they are calculated on the fly when converting back to .eng
    stream.skipRawData(12);
    
    for (int id = 0; id < MAX_INDEX_ENTRIES; id++) {
        qint32 offset, used;
        stream >> offset;
        stream >> used;
        if (used) {
            file.m_groups.append(TextGroup(id, offset));
            if (used > 1) {
                file.m_indexWithCounts = true;
            }
        }
    }
    
    char rawText[MAX_DATA_SIZE + 1];
    int textSize = stream.readRawData(rawText, MAX_DATA_SIZE);
    if (!stream.atEnd()) {
        logger.warn("Data part of the file is too large, max supported is 1 MB");
    }
    rawText[textSize] = 0; // ensure end-of-string
    // remove duplicate trailing \0's
    while (textSize > 0 && rawText[textSize - 1] == 0 && rawText[textSize - 2] == 0) {
        textSize--;
    }

    for (int i = 0; i < file.m_groups.size(); i++) {
        TextGroup &group = file.m_groups[i];
        int startOffset = group.fileOffset();
        int endOffset = i + 1 == file.m_groups.size() ? textSize : file.m_groups.at(i + 1).fileOffset();
        
        if (startOffset > textSize || endOffset > textSize || startOffset > endOffset || startOffset < 0) {
            logger.error(QString("Invalid data offset for group %1: %2-%3")
                .arg(QString::number(group.id()))
                .arg(QString::number(startOffset))
                .arg(QString::number(endOffset)));
            return false;
        }
        
        while (startOffset < endOffset) {
            // skip 0-chars, used by GOG engineers to hack the files
            while (!rawText[startOffset] && startOffset < endOffset) {
                startOffset++;
            }
            group.add(QString::fromAscii(&rawText[startOffset]));
            while (rawText[startOffset]) {
                startOffset++;
            }
            startOffset++; // read trailing \0
        }
    }
    return true;
}

static bool compareTextGroup(const TextGroup &g1, const TextGroup &g2)
{
    return g1.id() < g2.id();
}

bool TextFileEngStream::write(TextFile &file, QIODevice &device, const QString &encoding, Logger &logger)
{
    if (!device.open(QIODevice::WriteOnly)) {
        logger.error(QString("Unable to open ENG file for writing: %1").arg(device.errorString()));
        return false;
    }

    QDataStream stream(&device);
    prepareDataStream(stream, encoding);
    
    qSort(file.m_groups.begin(), file.m_groups.end(), compareTextGroup);

    QByteArray name = file.m_name.toAscii();
    if (name.length() > 16) {
        logger.warn(QString("Name '%1' is longer than 16 characters and will be truncated").arg(file.m_name));
    }
    stream.writeRawData(name.leftJustified(16, 0, true).data(), 16);
    qint32 groups = file.maxGroupId() + 1;
    qint32 strings = file.totalStrings();
    qint32 words = file.totalWords();
    
    stream << groups;
    stream << strings;
    stream << words;
    
    int lastWrittenIndex = -1;
    QByteArray textData;
    QList<TextGroup>::const_iterator it;
    for (it = file.m_groups.constBegin(); it != file.m_groups.constEnd(); it++) {
        writeEmptyEntries(stream, lastWrittenIndex, (*it).id(), textData.size());
        lastWrittenIndex = (*it).id();

        const QStringList &strings = (*it).strings();
        stream << (qint32) textData.size();
        stream << (qint32) (file.m_indexWithCounts ? strings.size() : 1);
        
        QStringList::const_iterator str;
        for (str = strings.constBegin(); str != strings.constEnd(); str++) {
            textData.append(*str);
            textData.append((char) 0); // append(str) doesn't include trailing \0
        }
    }
    writeEmptyEntries(stream, lastWrittenIndex, MAX_INDEX_ENTRIES, 0);
    textData.append((char) 0);
    stream.writeRawData(textData.data(), textData.length());
    // pad to the next 2 bytes
    if (textData.length() % 2) {
        stream << (qint8) 0;
    }
    return true;
}

void TextFileEngStream::writeEmptyEntries(QDataStream &eng, int lastWrittenIndex, int nextIndex, int offset)
{
    for (int i = lastWrittenIndex + 1; i < nextIndex; i++) {
        eng << (qint32) offset;
        eng << (qint32) 0;
    }
}

void TextFileEngStream::prepareDataStream(QDataStream &stream, const QString &encoding)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName(encoding.toAscii()));
    stream.setByteOrder(QDataStream::LittleEndian);
}
