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
