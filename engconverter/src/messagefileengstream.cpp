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

#include "messagefileengstream.h"

#include <QTextCodec>

enum {
    /**
     * Maximum size of the data part of the file.
     * 1 MB should be more than enough: max original data size is 520 kB.
     */
    MAX_DATA_SIZE = 1000000
};

bool MessageFileEngStream::read(MessageFile &file, QIODevice &device, Logger &logger)
{
    if (!device.open(QIODevice::ReadOnly)) {
        logger.error(QString("Unable to open ENG file for reading: %1").arg(device.errorString()));
        return false;
    }
    
    QDataStream stream(&device);
    prepareDataStream(stream, file.m_encoding);

    bool result = readFile(file, stream, logger);
    
    device.close();
    return result;
}

bool MessageFileEngStream::readFile(MessageFile &file, QDataStream &stream, Logger &logger)
{
    // header
    char rawName[17];
    stream.readRawData(rawName, 16);
    rawName[16] = 0; // make sure we have an end to the string
    file.m_name = QString::fromAscii(rawName);

    qint32 total, used;
    stream >> total >> used;
    
    file.m_totalEntries = total;

    for (int id = 0; id < total; id++) {
        readMessageEntry(id, file, stream);
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

    bool result = true;
    for (int i = 0; i < file.m_entries.size(); i++) {
        MessageEntry &entry = file.m_entries[i];
        int id = entry.id();
        result &= readStringContent(entry.video(), rawText, textSize, id, "video", logger);
        result &= readStringContent(entry.title(), rawText, textSize, id, "title", logger);
        result &= readStringContent(entry.subtitle(), rawText, textSize, id, "subtitle", logger);
        result &= readStringContent(entry.content(), rawText, textSize, id, "content", logger);
    }
    return result;
}

bool MessageFileEngStream::readStringContent(MessageEntry::String &target, const char *rawData, int textSize, int id, const QString &field, Logger &logger)
{
    int offset = target.offset;
    if (offset) {
        if (offset < 0 || offset > textSize) {
            logger.error(QString("Invalid data offset %2 for %3 text in entry %1")
                .arg(QString::number(id))
                .arg(QString::number(offset))
                .arg(field));
            return false;
        }
        target.text = QString(&rawData[offset]);
    }
    return true;
}

static int readShort(QDataStream &stream)
{
    qint16 value;
    stream >> value;
    return value;
}

static int readInt(QDataStream &stream)
{
    qint32 value;
    stream >> value;
    return value;
}

void MessageFileEngStream::readMessageEntry(int id, MessageFile &file, QDataStream &stream)
{
    MessageEntry entry(id);

    entry.setType(readShort(stream));
    entry.setSubtype(readShort(stream));
    stream.skipRawData(2); // unused short

    MessageEntry::Dialog &window = entry.dialog();
    window.x = readShort(stream);
    window.y = readShort(stream);
    window.width = readShort(stream);
    window.height = readShort(stream);

    MessageEntry::Image &image = entry.image();
    image.graphic = readShort(stream);
    image.x = readShort(stream);
    image.y = readShort(stream);

    MessageEntry::Image &image2 = entry.image2();
    image2.graphic = readShort(stream);
    image2.x = readShort(stream);
    image2.y = readShort(stream);

    MessageEntry::StringWithPosition &title = entry.title();
    title.x = readShort(stream);
    title.y = readShort(stream);
    
    MessageEntry::StringWithPosition &subtitle = entry.subtitle();
    subtitle.x = readShort(stream);
    subtitle.y = readShort(stream);

    stream.skipRawData(4); // unused x/y pair
    
    MessageEntry::StringWithPosition &video = entry.video();
    video.x = readShort(stream);
    video.y = readShort(stream);

    stream.skipRawData(14); // unused padding
    
    entry.setUrgent(readInt(stream));
    
    video.offset = readInt(stream);
    stream.skipRawData(4); // unused offset
    title.offset = readInt(stream);
    subtitle.offset = readInt(stream);
    entry.content().offset = readInt(stream);
    
    if (!entry.isEmpty()) {
        file.m_entries.append(entry);
    }
}

static bool compareMessageEntry(const MessageEntry &e1, const MessageEntry &e2)
{
    return e1.id() < e2.id();
}

bool MessageFileEngStream::write(MessageFile &file, QIODevice &device, Logger &logger)
{
    if (!device.open(QIODevice::WriteOnly)) {
        logger.error(QString("Unable to open ENG file for writing: %1").arg(device.errorString()));
        return false;
    }

    QDataStream stream(&device);
    prepareDataStream(stream, file.m_encoding);
    
    qSort(file.m_entries.begin(), file.m_entries.end(), compareMessageEntry);

    QByteArray name = file.m_name.toAscii();
    if (name.length() > 16) {
        logger.warn(QString("Name '%1' is longer than 16 characters and will be truncated").arg(file.m_name));
    }
    stream.writeRawData(name.leftJustified(16, 0, true).data(), 16);
    stream << (qint32) file.totalEntries();
    stream << (qint32) (file.maxEntryId() + 1);
    
    int lastWrittenIndex = -1;
    QByteArray textData;
    // first 16 bytes of textData are empty, to make strings start at offsets > 0
    for (int i = 0; i < 16; i++) {
        textData.append((char) 0);
    }
    
    QList<MessageEntry>::iterator it;
    for (it = file.m_entries.begin(); it != file.m_entries.end(); it++) {
        MessageEntry &entry = *it;
        writeEmptyEntries(stream, lastWrittenIndex, entry.id());
        lastWrittenIndex = entry.id();

        writeMessageEntry(entry, stream, textData);
    }
    writeEmptyEntries(stream, lastWrittenIndex, file.m_totalEntries);
    textData.append((char) 0);
    stream.writeRawData(textData.data(), textData.length());
    return true;
}

void MessageFileEngStream::writeEmptyEntries(QDataStream &eng, int lastWrittenIndex, int nextIndex)
{
    for (int i = lastWrittenIndex + 1; i < nextIndex; i++) {
        for (int d = 0; d < 80; d += 4) {
            eng << (qint32) 0;
        }
    }
}

void MessageFileEngStream::writeMessageEntry(MessageEntry &entry, QDataStream &stream, QByteArray &textData)
{
    // write texts to textData and store the offsets
    writeStringContent(entry.video(), textData);
    writeStringContent(entry.title(), textData);
    writeStringContent(entry.subtitle(), textData);
    writeStringContent(entry.content(), textData);
    
    stream << (qint16) entry.type();
    stream << (qint16) entry.subtype();
    stream << (qint16) 0; // unused short

    MessageEntry::Dialog &window = entry.dialog();
    stream << (qint16) window.x;
    stream << (qint16) window.y;
    stream << (qint16) window.width;
    stream << (qint16) window.height;

    MessageEntry::Image &image = entry.image();
    stream << (qint16) image.graphic;
    stream << (qint16) image.x;
    stream << (qint16) image.y;

    MessageEntry::Image &image2 = entry.image2();
    stream << (qint16) image2.graphic;
    stream << (qint16) image2.x;
    stream << (qint16) image2.y;

    MessageEntry::StringWithPosition &title = entry.title();
    stream << (qint16) title.x;
    stream << (qint16) title.y;
    
    MessageEntry::StringWithPosition &subtitle = entry.subtitle();
    stream << (qint16) subtitle.x;
    stream << (qint16) subtitle.y;

    stream << (qint32) 0; // unused x/y pair
    
    MessageEntry::StringWithPosition &video = entry.video();
    stream << (qint16) video.x;
    stream << (qint16) video.y;

    for (int i = 0; i < 7; i++) {
        stream << (qint16) 0;
    }
    
    stream << (qint32) (entry.urgent() ? 1 : 0);
    
    stream << (qint32) video.offset;
    stream << (qint32) 0; // unused offset
    stream << (qint32) title.offset;
    stream << (qint32) subtitle.offset;
    stream << (qint32) entry.content().offset;
}

void MessageFileEngStream::writeStringContent(MessageEntry::String &string, QByteArray &textData)
{
    if (string.text.isEmpty()) {
        string.offset = 0;
    } else {
        string.offset = textData.size();
        textData.append(string.text);
        textData.append((char) 0);
    }
}

void MessageFileEngStream::prepareDataStream(QDataStream &stream, const QString &encoding)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName(encoding.toAscii()));
    stream.setByteOrder(QDataStream::LittleEndian);
}
