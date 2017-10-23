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
    logger.error("XML reading of message files is not supported yet");
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

    const MessageEntry::Window &window = entry.window();
    xml.writeEmptyElement("window");
    xml.writeAttribute("x", QString::number(window.x));
    xml.writeAttribute("y", QString::number(window.y));
    xml.writeAttribute("width", QString::number(window.width));
    xml.writeAttribute("height", QString::number(window.height));

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
