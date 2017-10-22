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

#ifndef MESSAGEFILEENGSTREAM_H
#define MESSAGEFILEENGSTREAM_H

#include "logger.h"
#include "messagefile.h"

/**
 * Read/write a message (MM) file in ENG format
 */
class MessageFileEngStream
{
public:
    /**
     * Read data from ENG file 'device' into 'file'
     * \param file File data class to read into
     * \param device Device to read from
     * \return True on success, false on failure
     */
    bool read(MessageFile &file, QIODevice &device, Logger &logger);

    /**
     * Write 'file' to 'device' in ENG format
     * \param file File data class to write
     * \param device Device to write to
     * \return True on success, false on failure
     */
    bool write(MessageFile &file, QIODevice &device, Logger &logger);

private:
    void prepareDataStream(QDataStream &stream);
    bool readFile(MessageFile &file, QDataStream &stream, Logger &logger);
    void readMessageEntry(int id, MessageFile &file, QDataStream &stream, Logger &logger);
    bool readStringContent(MessageEntry::String &target, const char *rawData, int textSize, int id, const QString &field, Logger &logger);
    void writeEmptyEntries(QDataStream &eng, int lastWrittenIndex, int nextIndex);
    void writeMessageEntry(MessageEntry &entry, MessageFile &file, QDataStream &stream, QByteArray &textData);
    void writeStringContent(MessageEntry::String &string, QByteArray &textData);
};

#endif // MESSAGEFILEENGSTREAM_H
