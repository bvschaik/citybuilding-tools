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

#include "messagefilexmlstream.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

bool MessageFileXmlStream::read(MessageFile &file, QIODevice &device, Logger &logger)
{
    if (!device.open(QIODevice::ReadOnly)) {
        logger.error(QString("Unable to open XML file for reading: %1").arg(device.errorString()));
        return false;
    }

    QXmlStreamReader xml(&device);
    bool result = readFile(file, xml, logger);

    device.close();
    return result;
}

bool MessageFileXmlStream::readFile(MessageFile &file, QXmlStreamReader &xml, Logger &logger)
{
    if (!readOpenTag(xml, "messages", logger)) {
        logger.error("Unable to find root <messages> element");
        return false;
    }
    if (xml.attributes().hasAttribute("name")) {
        file.m_name = xml.attributes().value("name").toString();
    }
    if (!readIntegerAttribute(xml, "entries", &file.m_totalEntries, logger)) {
        return false;
    }
    while (readOpenTag(xml, "message", logger)) {
        if (!readMessageEntry(file, xml, logger) || !readCloseTag(xml, "message", logger)) {
            return false;
        }
    }
    return readCloseTag(xml, "messages", logger);
}

bool MessageFileXmlStream::readMessageEntry(MessageFile &file, QXmlStreamReader &xml, Logger &logger)
{
    int id, type, subtype;
    if (!readIntegerAttribute(xml, "id", &id, logger) ||
        !readIntegerAttribute(xml, "type", &type, logger) ||
        !readIntegerAttribute(xml, "subtype", &subtype, logger)) {
        return false;
    }
    MessageEntry entry(id);
    entry.setType(type);
    entry.setSubtype(subtype);

    if (!readOpenTag(xml, "dialog", logger)) {
        return false;
    }
    MessageEntry::Dialog &dialog = entry.dialog();
    if (!readIntegerAttribute(xml, "x", &dialog.x, logger) ||
        !readIntegerAttribute(xml, "y", &dialog.y, logger) ||
        !readIntegerAttribute(xml, "width", &dialog.width, logger) ||
        !readIntegerAttribute(xml, "height", &dialog.height, logger)) {
        return false;
    }
    readCloseTag(xml, "dialog", logger);

    if (!readNextOpenTag(xml, logger)) {
        logger.error(QString("Unexpected end of message %1, "
            "expected at least one of the following tags: "
            "image, title, subtitle, video, content").arg(id));
        return false;
    }

    do {
        const QString &tag = xml.name().toString();
        if (tag == "image" || tag == "image2") {
            MessageEntry::Image &image = tag == "image" ? entry.image() : entry.image2();
            if (!readIntegerAttribute(xml, "graphic", &image.graphic, logger) ||
                !readIntegerAttribute(xml, "x", &image.x, logger) ||
                !readIntegerAttribute(xml, "y", &image.y, logger)) {
                return false;
            }
        } else if (tag == "title" || tag == "subtitle" || tag == "video") {
            MessageEntry::StringWithPosition &str = tag == "title" ? entry.title()
                : (tag == "subtitle" ? entry.subtitle() : entry.video());
            if (!readIntegerAttribute(xml, "x", &str.x, logger) ||
                !readIntegerAttribute(xml, "y", &str.y, logger)) {
                return false;
            }
            str.text = xml.readElementText();
        } else if (tag == "content") {
            entry.content().text = xml.readElementText();
        } else {
            logger.error(QString("Unexpected tag %1 in message %2").arg(tag).arg(id));
        }
        if (!readCloseTag(xml, tag, logger)) {
            return false;
        }
    } while (readNextOpenTag(xml, logger));

    file.m_entries.append(entry);
    return true;
}

bool MessageFileXmlStream::readIntegerAttribute(QXmlStreamReader &xml, const QString &attr, int *value, Logger &logger)
{
    if (!xml.attributes().hasAttribute(attr)) {
        logger.error(QString("Required attribute %1 not found on tag <%2>")
            .arg(attr, xml.name().toString()));
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

bool MessageFileXmlStream::readNextOpenTag(QXmlStreamReader &xml, Logger &logger)
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
                logger.error(QString("Invalid XML: %s").arg(xml.errorString()));
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

bool MessageFileXmlStream::readOpenTag(QXmlStreamReader &xml, const QString &tag, Logger &logger)
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

bool MessageFileXmlStream::readCloseTag(QXmlStreamReader &xml, const QString &tag, Logger &logger)
{
    if (xml.isEndElement() && xml.name() == tag) {
        return true;
    }
    while (!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::EndElement && xml.name() == tag) {
            return true;
        }
    }
    logger.error(QString("Invalid XML: end element </%1> not found").arg(tag));
    return false;
}

bool MessageFileXmlStream::write(MessageFile &file, QIODevice &device, Logger &logger)
{
    if (!device.open(QIODevice::WriteOnly)) {
        logger.error(QString("Unable to open XML file for writing: %1").arg(device.errorString()));
        return false;
    }
    QXmlStreamWriter xml(&device);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();
    xml.writeStartElement("messages");
    xml.writeAttribute("entries", QString::number(file.totalEntries()));

    QList<MessageEntry>::iterator it;
    for (it = file.m_entries.begin(); it != file.m_entries.end(); it++) {
        writeMessageEntry(*it, xml);
    }

    xml.writeEndElement();
    xml.writeEndDocument();

    device.close();
    return true;
}

void MessageFileXmlStream::writeMessageEntry(MessageEntry &entry, QXmlStreamWriter &xml)
{
    xml.writeStartElement("message");
    xml.writeAttribute("id", QString::number(entry.id()));
    xml.writeAttribute("type", QString::number(entry.type()));
    xml.writeAttribute("subtype", QString::number(entry.subtype()));

    const MessageEntry::Dialog &dialog = entry.dialog();
    xml.writeEmptyElement("dialog");
    xml.writeAttribute("x", QString::number(dialog.x));
    xml.writeAttribute("y", QString::number(dialog.y));
    xml.writeAttribute("width", QString::number(dialog.width));
    xml.writeAttribute("height", QString::number(dialog.height));

    if (!entry.image().isEmpty()) {
        const MessageEntry::Image &image = entry.image();
        xml.writeEmptyElement("image");
        xml.writeAttribute("graphic", QString::number(image.graphic));
        xml.writeAttribute("x", QString::number(image.x));
        xml.writeAttribute("y", QString::number(image.y));
    }
    if (!entry.image2().isEmpty()) {
        const MessageEntry::Image &image = entry.image2();
        xml.writeEmptyElement("image2");
        xml.writeAttribute("graphic", QString::number(image.graphic));
        xml.writeAttribute("x", QString::number(image.x));
        xml.writeAttribute("y", QString::number(image.y));
    }
    writeText(entry.title(), "title", xml);
    writeText(entry.subtitle(), "subtitle", xml);
    writeText(entry.video(), "video", xml);

    if (!entry.content().text.isEmpty()) {
        xml.writeStartElement("content");
        xml.writeCharacters(entry.content().text);
        xml.writeEndElement();
    }

    xml.writeEndElement(); // message
}

void MessageFileXmlStream::writeText(const MessageEntry::StringWithPosition &str, const QString &tag, QXmlStreamWriter &xml)
{
    if (str.isEmpty()) {
        return;
    }
    xml.writeStartElement(tag);
    xml.writeAttribute("x", QString::number(str.x));
    xml.writeAttribute("y", QString::number(str.y));
    xml.writeCharacters(str.text);
    xml.writeEndElement();
}
