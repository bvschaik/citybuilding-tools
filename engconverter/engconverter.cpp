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

#include "engconverter.h"

#include "convertertab.h"

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
}

EngConverter::~EngConverter()
{}

void EngConverter::showHelp()
{
    // TODO implement
}

#include "engconverter.moc"
