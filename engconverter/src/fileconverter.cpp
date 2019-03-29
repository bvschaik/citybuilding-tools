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

#include "fileconverter.h"

#include "messagefile.h"
#include "textfile.h"

#include <QDataStream>
#include <QXmlStreamReader>

bool FileConverter::convertEngToXml(QFile &inputFile, QFile &outputFile, const QString &encoding, Logger &logger)
{
    FileType type = determineEngFileType(inputFile, logger);
    bool success;
    if (type == TYPE_TEXT) {
        logger.info("Determined file type: text");
        TextFile file(encoding);
        success = file.readFromEng(inputFile, logger) && file.writeToXml(outputFile, logger);
    } else if (type == TYPE_MESSAGE) {
        logger.info("Determined file type: message");
        MessageFile file(encoding);
        success = file.readFromEng(inputFile, logger) && file.writeToXml(outputFile, logger);
    } else {
        success = false;
        logger.error("Unknown input file type");
    }
    return success;
}

bool FileConverter::convertXmlToEng(QFile &inputFile, QFile &outputFile, const QString &encoding, Logger &logger)
{
    FileType type = determineXmlFileType(inputFile, logger);
    bool success;
    if (type == TYPE_TEXT) {
        logger.info("Determined file type: text");
        TextFile file(encoding);
        success = file.readFromXml(inputFile, logger) && file.writeToEng(outputFile, logger);
    } else if (type == TYPE_MESSAGE) {
        logger.info("Determined file type: message");
        MessageFile file(encoding);
        success = file.readFromXml(inputFile, logger) && file.writeToEng(outputFile, logger);
    } else {
        success = false;
        logger.error("Unknown input file type");
    }
    return success;
}

FileConverter::FileType FileConverter::determineEngFileType(QFile &file, Logger &logger)
{
    if (!file.open(QIODevice::ReadOnly)) {
        logger.error(QString("Unable to open input file: %1").arg(file.errorString()));
        return TYPE_UNKNOWN;
    }
    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);
    
    FileType type = TYPE_UNKNOWN;
    stream.skipRawData(16); // header text in both cases
    qint32 first, second;
    stream >> first >> second;
    if ((first == 400 || first == 1000) && first >= second) {
        // first = total entries, second = used entries
        type = TYPE_MESSAGE;
    } else if (first <= 400 && first <= second) {
        // first = number of used entries, second = total number of strings,
        // most likely text
        type = TYPE_TEXT;
    } else {
        // check start of data section to determine if it's text after all
        stream.skipRawData(8000);
        quint32 zero, nonzero;
        stream >> zero >> nonzero;
        if (zero == 0 && nonzero != 0) {
            type = TYPE_TEXT;
        }
    }
    file.close();
    return type;
}

FileConverter::FileType FileConverter::determineXmlFileType(QFile &file, Logger &logger)
{
    if (!file.open(QIODevice::ReadOnly)) {
        logger.error(QString("Unable to open input file: %1").arg(file.errorString()));
        return TYPE_UNKNOWN;
    }
    QXmlStreamReader xml(&file);
    
    FileType type = TYPE_UNKNOWN;
    while (!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            QString tag = xml.name().toString();
            if (tag == "strings") {
                type = TYPE_TEXT;
            } else if (tag == "messages") {
                type = TYPE_MESSAGE;
            }
        }
    }
    file.close();
    return type;
}
