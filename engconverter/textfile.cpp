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

#include "textfile.h"

#include "textfileengstream.h"
#include "textfilexmlstream.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <qtextcodec.h>


TextFile::TextFile()
: m_indexWithCounts(false)
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
