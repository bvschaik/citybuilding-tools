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

#ifndef TEXTFILE_H
#define TEXTFILE_H

#include "textgroup.h"

#include <QDataStream>
#include <QList>
#include <QString>
#include <QXmlStreamWriter>

class TextFileEngStream;
class TextFileXmlStream;

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
     * \return True on success, false on failure
     */
    bool readFromEng(QIODevice &device);

    /**
     * Read text from device in XML format
     * \param device Device to read from
     * \return True on success, false on failure
     */
    bool readFromXml(QIODevice &device);

    /**
     * Write text to device in ENG format
     * \param device Device to write to
     * \return True on success, false on failure
     */
    bool writeToEng(QIODevice &device);

    /**
     * Write text to device in XML format
     * \param device Device to write to
     * \return True on success, false on failure
     */
    bool writeToXml(QIODevice &device);

private:
    bool m_indexWithCounts;
    QString m_name;
    QList<TextGroup> m_groups;

    friend class TextFileEngStream;
    friend class TextFileXmlStream;
};

#endif // TEXTFILE_H
