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

#include "messagefile.h"

#include "messagefileengstream.h"
#include "messagefilexmlstream.h"

int MessageFile::maxEntryId() const
{
    if (m_entries.isEmpty()) {
        return 0;
    }
    return m_entries.at(m_entries.size() - 1).id();
}

int MessageFile::totalEntries() const
{
    return m_totalEntries;
}

bool MessageFile::readFromEng(QIODevice &device, Logger &logger)
{
    return MessageFileEngStream().read(*this, device, logger);
}

bool MessageFile::writeToEng(QIODevice &device, Logger &logger)
{
    return MessageFileEngStream().write(*this, device, logger);
}

bool MessageFile::readFromXml(QIODevice &device, Logger &logger)
{
    return MessageFileXmlStream().read(*this, device, logger);
}

bool MessageFile::writeToXml(QIODevice &device, Logger &logger)
{
    return MessageFileXmlStream().write(*this, device, logger);
}
