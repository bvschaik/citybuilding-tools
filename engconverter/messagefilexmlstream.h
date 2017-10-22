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

#ifndef MESSAGEFILEXMLSTREAM_H
#define MESSAGEFILEXMLSTREAM_H

#include "messagefile.h"
#include "messageentry.h"
#include "logger.h"

#include <QIODevice>
#include <QXmlStreamWriter>

/**
 * Read/write a message (MM) file in XML format
 */
class MessageFileXmlStream
{
public:
    /**
     * Read data from XML file 'device' into 'file'
     * \param file File data class to read into
     * \param device Device to read from
     * \return True on success, false on failure
     */
    bool read(MessageFile &file, QIODevice &device, Logger &logger);

    /**
     * Write 'file' to 'device' in XML format
     * \param file File data class to write
     * \param device Device to write to
     * \return True on success, false on failure
     */
    bool write(MessageFile &file, QIODevice &device, Logger &logger);

private:
    void writeMessageEntry(MessageEntry &entry, QXmlStreamWriter &xml);
    void writeText(const MessageEntry::StringWithPosition &str, const QString &tag, QXmlStreamWriter &xml);
};

#endif // MESSAGEFILEXMLSTREAM_H
