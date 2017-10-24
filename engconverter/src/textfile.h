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

#ifndef TEXTFILE_H
#define TEXTFILE_H

#include "textgroup.h"
#include "logger.h"

#include <QDataStream>
#include <QList>
#include <QString>
#include <QXmlStreamWriter>

class TextFileEngStream;
class TextFileXmlStream;

/**
 * Data class representing a text/strings file
 */
class TextFile
{
public:
    TextFile();
    
    /**
     * Name of the file
     */
    const QString &name() const;
    
    /**
     * List of groups
     */
    const QList<TextGroup> groups() const;

    /**
     * Last group ID in use
     */
    int maxGroupId() const;

    /**
     * Total number of strings
     */
    int totalStrings() const;

    /**
     * Total number of words in all strings
     */
    int totalWords() const;

    /**
     * Read text from device in ENG format
     * \param device Device to read from
     * \param logger Logger for messages
     * \return True on success, false on failure
     */
    bool readFromEng(QIODevice &device, Logger &logger);

    /**
     * Read text from device in XML format
     * \param device Device to read from
     * \param logger Logger for messages
     * \return True on success, false on failure
     */
    bool readFromXml(QIODevice &device, Logger &logger);

    /**
     * Write text to device in ENG format
     * \param device Device to write to
     * \param logger Logger for messages
     * \return True on success, false on failure
     */
    bool writeToEng(QIODevice &device, Logger &logger);

    /**
     * Write text to device in XML format
     * \param device Device to write to
     * \param logger Logger for messages
     * \return True on success, false on failure
     */
    bool writeToXml(QIODevice &device, Logger &logger);

private:
    bool m_indexWithCounts;
    QString m_name;
    QList<TextGroup> m_groups;

    friend class TextFileEngStream;
    friend class TextFileXmlStream;
};

#endif // TEXTFILE_H
