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

#ifndef TEXTFILEENGSTREAM_H
#define TEXTFILEENGSTREAM_H

#include "textfile.h"

#include <QDataStream>
#include <QIODevice>

class TextFileEngStream
{
public:
    /**
     * Read data from ENG file 'device' into 'file'
     * \param file File data class to read into
     * \param device Device to read from
     * \return True on success, false on failure
     */
    bool read(TextFile &file, QIODevice &device, Logger &logger);

    /**
     * Write 'file' to 'device' in ENG format
     * \param file File data class to write
     * \param device Device to write to
     * \return True on success, false on failure
     */
    bool write(TextFile &file, QIODevice &device, Logger &logger);

private:
    void prepareDataStream(QDataStream &stream);
    bool readFile(TextFile &file, QDataStream &stream, Logger &logger);
    void writeEmptyEntries(QDataStream &stream, int lastWrittenIndex, int nextIndex, int offset);
};

#endif // TEXTFILEENGSTREAM_H
