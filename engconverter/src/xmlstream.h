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

#ifndef XMLSTREAM_H
#define XMLSTREAM_H

#include "logger.h"

#include <QXmlStreamReader>

class XmlStream
{
protected:
    bool readIntegerAttribute(QXmlStreamReader &xml, const QString &attr, int *value, Logger &logger);
    bool readBooleanAttribute(QXmlStreamReader &xml, const QString &attr, bool *value, Logger &logger);
    bool readNextOpenTag(QXmlStreamReader &xml, Logger &logger);
    bool readOpenTag(QXmlStreamReader &xml, const QString &tag, Logger &logger);
    bool readCloseTag(QXmlStreamReader &xml, const QString &tag, Logger &logger);
};

#endif // XMLSTREAM_H
