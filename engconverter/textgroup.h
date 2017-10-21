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

#ifndef TEXTGROUP_H
#define TEXTGROUP_H

#include <QStringList>

/**
 * Data class representing a group of related strings within a text file
 */
class TextGroup
{
public:
    /**
     * Creates a new TextGroup with the specified ID
     * \param id ID of the group
     */
    TextGroup(int id);
    
    /**
     * Creates a new TextGroup with the specified ID and file offset
     * \param id ID of the group
     * \param fileOffset Start offset in the data part of the file
     */
    TextGroup(int id, int fileOffset);
    TextGroup(const TextGroup &other);

    /**
     * Gets the ID
     */
    int id() const;
    
    /**
     * Gets the file offset
     */
    int fileOffset() const;
    
    /**
     * Gets the number of strings
     */
    int size() const;
    
    /**
     * Gets the total number of words in all strings
     */
    int totalWords() const;
    
    /**
     * Gets the list of strings
     */
    const QStringList strings() const;
    
    /**
     * Adds a string to the list of strings
     * \param string String to add
     */
    void add(const QString &string);

private:
    int m_id;
    int m_fileOffset;
    QStringList m_strings;
};

#endif // TEXTGROUP_H
