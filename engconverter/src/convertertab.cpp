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

#include "convertertab.h"

#include "fileconverter.h"
#include "logger.h"

#include <QtGui/QtGui>

ConverterTab::ConverterTab(ConverterTab::FileType from, ConverterTab::FileType to, QWidget *parent)
: QWidget(parent), fromType(from), toType(to)
{
    QLabel *inputFileLabel = new QLabel(tr("Input %1 file:").arg(typeName(from)));
    inputFileEdit = new QLineEdit();
    QPushButton *inputFileBrowse = new QPushButton(tr("Browse..."));

    QLabel *outputFileLabel = new QLabel(tr("Output %1 file:").arg(typeName(to)));
    outputFileEdit = new QLineEdit();
    QPushButton *outputFileBrowse = new QPushButton(tr("Browse..."));

    QLabel *encodingLabel = new QLabel(tr("Encoding:"));
    encodingComboBox = new QComboBox();
    encodingComboBox->addItem("Windows-1252 - Default", QVariant("Windows-1252"));
    encodingComboBox->addItem("Windows-1250 - Eastern European", QVariant("Windows-1250"));
    encodingComboBox->addItem("Windows-1251 - Cyrillic", QVariant("Windows-1251"));
    encodingComboBox->addItem("Traditional Chinese (C3)", QVariant("c3-tc"));

    QPushButton *convertButton = new QPushButton(tr("Convert"));

    QLabel *convertOutputLabel = new QLabel(tr("Conversion output:"));
    convertOutputEdit = new QTextEdit();
    convertOutputEdit->setReadOnly(true);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(inputFileLabel, 0, 0);
    mainLayout->addWidget(inputFileEdit, 0, 1);
    mainLayout->addWidget(inputFileBrowse, 0, 2);

    mainLayout->addWidget(outputFileLabel, 1, 0);
    mainLayout->addWidget(outputFileEdit, 1, 1);
    mainLayout->addWidget(outputFileBrowse, 1, 2);

    mainLayout->addWidget(encodingLabel, 2, 0);
    mainLayout->addWidget(encodingComboBox, 2, 1, 1, 2);

    mainLayout->addWidget(convertButton, 3, 1, 1, 2);

    mainLayout->addWidget(convertOutputLabel, 4, 0, Qt::AlignTop);
    mainLayout->addWidget(convertOutputEdit, 4, 1, 1, 2);
    
    setLayout(mainLayout);

    connect(inputFileBrowse, SIGNAL(clicked()), SLOT(inputBrowse()));
    connect(outputFileBrowse, SIGNAL(clicked()), SLOT(outputBrowse()));
    
    connect(convertButton, SIGNAL(clicked()), SLOT(convertFile()));
}

ConverterTab::~ConverterTab()
{}

void ConverterTab::inputBrowse()
{
    QString inputFileName = QFileDialog::getOpenFileName(this,
        tr("Choose the %1 file you wish to convert").arg(typeName(fromType)),
        inputFileEdit->text(), tr("%1 File (%2)").arg(typeName(fromType), typeWildcard(fromType)));
    
    if (!inputFileName.isEmpty()) {
        inputFileEdit->setText(inputFileName);
    }
}

void ConverterTab::outputBrowse()
{
    QString suggestion = outputFileEdit->text();
    if (suggestion.isEmpty() && !inputFileEdit->text().isEmpty()) {
        QString filename = inputFileEdit->text();
        suggestion = filename.replace(filename.length() - 4, 4, typeExt(toType));
    }
    QString outputFileName = QFileDialog::getSaveFileName(this, tr("Output %1 file").arg(typeName(toType)),
        suggestion, tr("%1 File (%2)").arg(typeName(toType), typeWildcard(toType)));
    
    if (!outputFileName.isEmpty()) {
        outputFileEdit->setText(outputFileName);
    }
}

void ConverterTab::convertFile()
{
    QFile inputFile(inputFileEdit->text());
    QFile outputFile(outputFileEdit->text());
    QString encoding = encodingComboBox->itemData(encodingComboBox->currentIndex()).toString();
    FileConverter converter;
    Logger logger;
    logger.info(QString("Using encoding: %1").arg(encoding));
    bool success;
    if (fromType == ENG) {
        success = converter.convertEngToXml(inputFile, outputFile, encoding, logger);
    } else {
        success = converter.convertXmlToEng(inputFile, outputFile, encoding, logger);
    }
    QString firstLine = success ? tr("Conversion OK") : tr("*** Conversion FAILED ***");
    
    QString totalLog = firstLine + "\n\n" + logger.messages().join("\n");
    convertOutputEdit->setText(totalLog);
}

const QString &ConverterTab::typeName(ConverterTab::FileType type)
{
    static const QString eng("ENG");
    static const QString xml("XML");
    return type == ENG ? eng : xml;
}

const QString &ConverterTab::typeExt(ConverterTab::FileType type)
{
    static const QString eng(".eng");
    static const QString xml(".xml");
    return type == ENG ? eng : xml;
}

const QString &ConverterTab::typeWildcard(ConverterTab::FileType type)
{
    static const QString eng("*.eng;*.rus");
    static const QString xml("*.xml");
    return type == ENG ? eng : xml;
}
