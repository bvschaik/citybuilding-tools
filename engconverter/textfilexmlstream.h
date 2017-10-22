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

#ifndef TEXTFILEXMLSTREAM_H
#define TEXTFILEXMLSTREAM_H

#include "textfile.h"
#include "textgroup.h"

#include <QIODevice>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

/**
 * Read/write a text/strings file in XML format
 */
class TextFileXmlStream
{
public:
    /**
     * Read data from XML file 'device' into 'file'
     * \param file File data class to read into
     * \param device Device to read from
     * \return True on success, false on failure
     */
    bool read(TextFile &file, QIODevice &device, Logger &logger);

    /**
     * Write 'file' to 'device' in XML format
     * \param file File data class to write
     * \param device Device to write to
     * \return True on success, false on failure
     */
    bool write(TextFile &file, QIODevice &device, Logger &logger);

private:
    bool readFile(TextFile &file, QXmlStreamReader &xml, Logger &logger);
    bool readOpenTag(QXmlStreamReader &xml, const QString &tag, Logger &logger);
    bool readCloseTag(QXmlStreamReader &xml, const QString &tag, Logger &logger);
    bool readGroup(TextFile &file, QXmlStreamReader &xml, Logger &logger);
    void writeGroup(const TextGroup &group, QXmlStreamWriter &xml);
};

#endif // TEXTFILEXMLSTREAM_H
