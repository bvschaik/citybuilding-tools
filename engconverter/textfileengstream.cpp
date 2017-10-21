/*
 * Citybuilding tools: a set of utilities to work with the game files
 * Copyright (C) 2017  Bianca van Schaik <bvschaik@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

bool TextFileEngStream::read(TextFile &file, QIODevice &device, Logger &logger)
{
    if (!device.open(QIODevice::ReadOnly)) {
        logger.error(QString("Unable to open ENG file for reading: %1").arg(device.errorString()));
        return false;
    }
    
    QDataStream stream(&device);
    prepareDataStream(stream);

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

bool TextFileEngStream::write(TextFile &file, QIODevice &device, Logger &logger)
{
    if (!device.open(QIODevice::WriteOnly)) {
        logger.error(QString("Unable to open ENG file for writing: %1").arg(device.errorString()));
        return false;
    }

    QDataStream stream(&device);
    prepareDataStream(stream);
    
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

void TextFileEngStream::prepareDataStream(QDataStream &stream)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1252"));
    stream.setByteOrder(QDataStream::LittleEndian);
}
