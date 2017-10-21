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

#ifndef TEXTFILEXMLSTREAM_H
#define TEXTFILEXMLSTREAM_H

#include "textfile.h"
#include "textgroup.h"

#include <QIODevice>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class TextFileXmlStream
{
public:
    /**
     * Read data from XML file 'device' into 'file'
     * \param file File data class to read into
     * \param device Device to read from
     * \return True on success, false on failure
     */
    bool read(TextFile &file, QIODevice &device);

    /**
     * Write 'file' to 'device' in XML format
     * \param file File data class to write
     * \param device Device to write to
     * \return True on success, false on failure
     */
    bool write(TextFile &file, QIODevice &device);

private:
    bool readFile(TextFile &file, QXmlStreamReader &xml);
    bool readOpenTag(QXmlStreamReader &xml, const QString &tag);
    bool readCloseTag(QXmlStreamReader &xml, const QString &tag);
    bool readGroup(TextFile &file, QXmlStreamReader &xml);
    void writeGroup(const TextGroup &group, QXmlStreamWriter &xml);
};

#endif // TEXTFILEXMLSTREAM_H
