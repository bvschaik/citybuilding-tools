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

#include "textgroup.h"

TextGroup::TextGroup(int id)
: m_id(id), m_fileOffset(-1)
{}

TextGroup::TextGroup(int id, int fileOffset)
: m_id(id), m_fileOffset(fileOffset)
{}

TextGroup::TextGroup(const TextGroup &other)
: m_id(other.m_id), m_fileOffset(other.m_fileOffset), m_strings(other.m_strings)
{}

int TextGroup::id() const
{
    return m_id;
}

int TextGroup::fileOffset() const
{
    return m_fileOffset;
}

const QStringList TextGroup::strings() const
{
    return m_strings;
}

int TextGroup::size() const
{
    return m_strings.size();
}

void TextGroup::add(const QString &string)
{
    m_strings.append(string);
}

static bool isLetter(QChar c, bool inWord)
{
    if (c.isLetter()) {
        return true;
    }
    if (inWord && c != ' ') {
        return true;
    }
    return false;
}

static int countWords(const QString &str)
{
    int words = 0;
    int index = 0;
    bool inWord = false;
    while (index < str.length()) {
        if (isLetter(str[index], inWord)) {
            if (!inWord) {
                inWord = true;
                words++;
            }
        } else {
            if (inWord) {
                inWord = false;
            }
        }
        index++;
    }
    return words;
}

int TextGroup::totalWords() const
{
    int total = 0;
    QStringList::const_iterator it;
    for (it = m_strings.constBegin(); it != m_strings.constEnd(); it++) {
        total += countWords(*it);
    }
    return total;
}
