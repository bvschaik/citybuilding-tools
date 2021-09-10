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

#include "xmlstream.h"

bool XmlStream::readIntegerAttribute(QXmlStreamReader &xml, const QString &attr, int *value, Logger &logger)
{
    if (!xml.attributes().hasAttribute(attr)) {
        logger.error(QString("Required attribute %1 not found on tag <%2>")
            .arg(attr, xml.name().toString()));
        return false;
    }
    const QString valueStr = xml.attributes().value(attr).toString();
    bool ok;
    *value = valueStr.toInt(&ok);
    if (!ok) {
        logger.error(QString("Attribute %1 (%2) on tag <%3> must be an integer")
            .arg(attr, valueStr, xml.name().toString()));
        return false;
    }
    return true;
}

bool XmlStream::readBooleanAttribute(QXmlStreamReader &xml, const QString &attr, bool *value, Logger &logger)
{
    if (!xml.attributes().hasAttribute(attr)) {
        *value = false;
        return true;
    }
    const QString valueStr = xml.attributes().value(attr).toString();
    if (valueStr == "true") {
        *value = true;
        return true;
    } else if (valueStr == "false") {
        *value = false;
        return true;
    } else {
        logger.error(QString("Attribute %1 (%2) on tag <%3> must be a boolean")
            .arg(attr, valueStr, xml.name().toString()));
        return false;
    }
}

bool XmlStream::readNextOpenTag(QXmlStreamReader &xml, Logger &logger)
{
    while (!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        switch (token) {
            case QXmlStreamReader::StartDocument:
            case QXmlStreamReader::Comment:
            case QXmlStreamReader::DTD:
            case QXmlStreamReader::Characters:
            case QXmlStreamReader::EntityReference:
            case QXmlStreamReader::ProcessingInstruction:
            case QXmlStreamReader::NoToken:
                continue;

            case QXmlStreamReader::Invalid:
                logger.error(QString("Invalid XML: %1").arg(xml.errorString()));
                return false;

            case QXmlStreamReader::EndDocument:
            case QXmlStreamReader::EndElement:
                return false;

            case QXmlStreamReader::StartElement:
                return true;
        }
    }
    return false;
}

bool XmlStream::readOpenTag(QXmlStreamReader &xml, const QString &tag, Logger &logger)
{
    if (xml.isStartElement() && xml.name() == tag) {
        return true;
    }
    if (readNextOpenTag(xml, logger)) {
        if (xml.name() == tag) {
            return true;
        } else {
            logger.error(QString("Invalid XML: expected tag <%1>, got <%2>")
                    .arg(tag, xml.name().toString()));
        }
    }
    return false;
}

bool XmlStream::readCloseTag(QXmlStreamReader &xml, const QString &tag, Logger &logger)
{
    if (xml.isEndElement() && xml.name() == tag) {
        return true;
    }
    while (!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::EndElement && xml.name() == tag) {
            return true;
        } else if (token == QXmlStreamReader::Invalid) {
            logger.error(QString("Invalid XML: %1").arg(xml.errorString()));
            return false;
        }
    }
    logger.error(QString("Invalid XML: end element </%1> not found").arg(tag));
    return false;
}
