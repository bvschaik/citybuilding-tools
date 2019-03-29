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

#include "textfile.h"

#include "textfileengstream.h"
#include "textfilexmlstream.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <qtextcodec.h>


TextFile::TextFile(const QString &encoding)
: m_indexWithCounts(false), m_encoding(encoding)
{}

const QString &TextFile::name() const
{
    return m_name;
}

const QList<TextGroup> TextFile::groups() const
{
    return m_groups;
}

int TextFile::maxGroupId() const
{
    if (m_groups.isEmpty()) {
        return 0;
    }
    return m_groups.at(m_groups.size() - 1).id();
}

int TextFile::totalStrings() const
{
    int total = 0;
    QList<TextGroup>::const_iterator it;
    for (it = m_groups.constBegin(); it != m_groups.constEnd(); it++) {
        total += (*it).size();
    }
    return total;
}

int TextFile::totalWords() const
{
    int total = 0;
    QList<TextGroup>::const_iterator it;
    for (it = m_groups.constBegin(); it != m_groups.constEnd(); it++) {
        total += (*it).totalWords();
    }
    return total;
}

bool TextFile::readFromEng(QIODevice &device, Logger &logger)
{
    return TextFileEngStream().read(*this, device, logger);
}

bool TextFile::writeToEng(QIODevice &device, Logger &logger)
{
    return TextFileEngStream().write(*this, device, logger);
}

bool TextFile::readFromXml(QIODevice &device, Logger &logger)
{
    return TextFileXmlStream().read(*this, device, logger);
}

bool TextFile::writeToXml(QIODevice &device, Logger &logger)
{
    return TextFileXmlStream().write(*this, device, logger);
}
