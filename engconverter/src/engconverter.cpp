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

#include "engconverter.h"

#include "convertertab.h"
#include "helpdialog.h"

#include <QtGui/QtGui>

EngConverter::EngConverter(): QMainWindow()
{
    QTabWidget *tabWidget = new QTabWidget(this);
    
    tabWidget->addTab(new ConverterTab(ConverterTab::ENG, ConverterTab::XML, this), tr("ENG to XML"));
    tabWidget->addTab(new ConverterTab(ConverterTab::XML, ConverterTab::ENG, this), tr("XML to ENG"));

    QPushButton *helpButton = new QPushButton(tr("Help"));
    helpButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(helpButton, SIGNAL(clicked()), SLOT(showHelp()));
    
    QPushButton *quitButton = new QPushButton(tr("Quit"));
    quitButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(quitButton, SIGNAL(clicked()), SLOT(close()));

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(tabWidget, 0, 0, 1, 2);
    gridLayout->addWidget(helpButton, 1, 0, Qt::AlignLeft);
    gridLayout->addWidget(quitButton, 1, 1, Qt::AlignRight);
    
    QWidget *widget = new QWidget();
    widget->setLayout(gridLayout);

    setCentralWidget(widget);
    setWindowTitle(tr("ENG Converter"));
    setWindowIcon(QIcon(":/icon.png"));
}

EngConverter::~EngConverter()
{}

void EngConverter::showHelp()
{
    HelpDialog dialog(this, "ENG Converter");
    dialog.exec();
}
