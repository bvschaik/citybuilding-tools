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
