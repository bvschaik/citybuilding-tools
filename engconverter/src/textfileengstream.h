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

#ifndef TEXTFILEENGSTREAM_H
#define TEXTFILEENGSTREAM_H

#include "engdatastream.h"
#include "textfile.h"

#include <QDataStream>
#include <QIODevice>

/**
 * Read/write a text/strings file in ENG format
 */
class TextFileEngStream
{
public:
    /**
     * Read data from ENG file 'device' into 'file'
     * \param file File data class to read into
     * \param device Device to read from
     * \param encoding Encoding to use
     * \param logger Logger for messages
     * \return True on success, false on failure
     */
    bool read(TextFile &file, QIODevice &device, const QString &encoding, Logger &logger);

    /**
     * Write 'file' to 'device' in ENG format
     * \param file File data class to write
     * \param device Device to write to
     * \param encoding Encoding to use
     * \param logger Logger for messages
     * \return True on success, false on failure
     */
    bool write(TextFile &file, QIODevice &device, const QString &encoding, Logger &logger);

private:
    void prepareDataStream(EngDataStream &stream, const QString &encoding);
    bool readFile(TextFile &file, EngDataStream &stream, Logger &logger);
    void writeEmptyEntries(EngDataStream &stream, int lastWrittenIndex, int nextIndex, int offset);
};

#endif // TEXTFILEENGSTREAM_H
