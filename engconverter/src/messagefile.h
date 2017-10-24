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

#ifndef MESSAGEFILE_H
#define MESSAGEFILE_H

#include "logger.h"
#include "messageentry.h"

#include <QIODevice>

class MessageFileEngStream;
class MessageFileXmlStream;

/**
 * Data class representing a message (MM) file
 */
class MessageFile
{
public:
    /**
     * Last entry ID in use
     */
    int maxEntryId() const;

    /**
     * Total number of entries in the index
     */
    int totalEntries() const;
    
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
    QString m_name;
    int m_totalEntries;
    
    QList<MessageEntry> m_entries;

    friend class MessageFileEngStream;
    friend class MessageFileXmlStream;
};

#endif // MESSAGEFILE_H
