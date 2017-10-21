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

#include "textfilexmlstream.h"

#include "textfile.h"
#include "textgroup.h"


bool TextFileXmlStream::read(TextFile &file, QIODevice &device)
{
    if (!device.open(QIODevice::ReadOnly)) {
        // TODO Unable to open file %s for reading
        return false;
    }
    
    QXmlStreamReader xml(&device);
    bool result = readFile(file, xml);
    
    device.close();
    return result;
}

bool TextFileXmlStream::readFile(TextFile &file, QXmlStreamReader &xml)
{
    if (!readOpenTag(xml, "strings")) {
        // TODO Unable to find root <strings> element
        return false;
    }
    if (xml.attributes().hasAttribute("name")) {
        file.m_name = xml.attributes().value("name").toString();
    }
    if (xml.attributes().hasAttribute("indexWithCounts")) {
        file.m_indexWithCounts = xml.attributes().value("indexWithCounts").toString() != "false";
    }
    while (readOpenTag(xml, "group")) {
        if (!readGroup(file, xml) || !readCloseTag(xml, "group")) {
            return false;
        }
    }
    return readCloseTag(xml, "strings");
}

bool TextFileXmlStream::readOpenTag(QXmlStreamReader &xml, const QString &tag)
{
    if (xml.isStartElement() && xml.name() == tag) {
        return true;
    }
    while (!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        switch (token) {
            case QXmlStreamReader::StartDocument:
            case QXmlStreamReader::Comment:
            case QXmlStreamReader::DTD:
            case QXmlStreamReader::Characters:
            case QXmlStreamReader::EntityReference:
            case QXmlStreamReader::ProcessingInstruction:
                continue;
                
            case QXmlStreamReader::Invalid:
                // TODO Invalid XML file: %s, xml.errorString()
                return false;
            case QXmlStreamReader::EndDocument:
            case QXmlStreamReader::EndElement:
                // TODO Unexpected end of EndElement
                return false;
                
            case QXmlStreamReader::StartElement:
                if (xml.name() == tag) {
                    return true;
                } else {
                    // TODO Invalid XML: expected %s, got %s
                    return false;
                }
        }
    }
    // TODO Unexpected end of document
    return false;
}

bool TextFileXmlStream::readCloseTag(QXmlStreamReader &xml, const QString &tag)
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
    // TODO End element </%s> not found
    return false;
}

bool TextFileXmlStream::readGroup(TextFile &file, QXmlStreamReader &xml)
{
    if (!xml.attributes().hasAttribute("id")) {
        // TODO Group does not have an ID attribute
        return false;
    }
    bool ok;
    int id = xml.attributes().value("id").toString().toInt(&ok);
    if (!ok) {
        // TODO Group ID is not an integer: %s
        return false;
    }
    TextGroup group(id);
    int index = 0;
    while (readOpenTag(xml, "string")) {
        int textId = xml.attributes().value("id").toString().toInt(&ok);
        if (!ok) {
            // TODO String ID is not an integer: %s
        }
        if (textId != group.size()) {
            // TODO Strings in group %d are not ordered properly
            return false;
        }
        index++;
        group.add(xml.readElementText());
        readCloseTag(xml, "string");
    }
    file.m_groups.append(group);
    return true;
}

bool TextFileXmlStream::write(TextFile &file, QIODevice &device)
{
    if (!device.open(QIODevice::WriteOnly)) {
        // TODO Unable to open file %s for writing
        return false;
    }
    QXmlStreamWriter xml(&device);
    xml.setAutoFormatting(true);
    
    xml.writeStartDocument();
    xml.writeStartElement("strings");
    xml.writeAttribute("name", file.m_name);
    xml.writeAttribute("indexWithCounts", file.m_indexWithCounts ? "true" : "false");
    
    QList<TextGroup>::const_iterator it;
    for (it = file.m_groups.constBegin(); it != file.m_groups.constEnd(); it++) {
        writeGroup(*it, xml);
    }
    
    xml.writeEndElement();
    xml.writeEndDocument();
    
    device.close();
}

void TextFileXmlStream::writeGroup(const TextGroup &group, QXmlStreamWriter &xml)
{
    xml.writeStartElement("group");
    xml.writeAttribute("id", QString::number(group.id()));

    const QStringList &strings = group.strings();
    QStringList::const_iterator it;
    int index = 0;
    for (it = strings.constBegin(); it != strings.constEnd(); it++) {
        xml.writeStartElement("string");
        xml.writeAttribute("id", QString::number(index));
        xml.writeCharacters(*it);
        xml.writeEndElement();
        index++;
    }

    xml.writeEndElement();
}
