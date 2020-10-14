/*
 *   Citybuilding Mappers - create minimaps from citybuilding game files
 *   Copyright (C) 2007, 2008  Bianca van Schaik
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include "helpdialog.h"

#include <QtGui/QFont>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

HelpDialog::HelpDialog(QWidget *parent, const QString &appname)
	: QDialog(parent) {
	
	this->appname = appname;
	
	setWindowTitle(appname + tr(" Help"));
	
	// Create textedit
	QTextEdit *textEdit = new QTextEdit();
	textEdit->setReadOnly(true);
	textEdit->setWordWrapMode(QTextOption::WordWrap);
	textEdit->setPlainText(getHelpText());
	textEdit->setMinimumSize(480, 300);
	
	// Create ok button
	QPushButton *okButton = new QPushButton(tr("OK"), this);
	okButton->setDefault(true);
	connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
	
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->addStretch();
	buttonLayout->addWidget(okButton);
	buttonLayout->addStretch();
	
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(textEdit);
	layout->addLayout(buttonLayout);
	
	setLayout(layout);
	setModal(true);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

QString HelpDialog::getHelpText() {
	QString text = tr(
"This program can open the .sg2 and .sg3 files that were used in "
"the citybuilding games from Impressions Games.\n\n"
"Loading a single SG file:\n"
"Open a .sg2/.sg3 file and a tree of images will appear to the left. "
"Click any image to show it on the right. Use the File menu to save the "
"image as PNG.\n\n"
"Batch-extraction:\n"
"To extract all images in one go, use File -> Batch Extract. "
"Select a directory where SG files are stored, select a destination, "
"and all images will be extracted.\n\n"
"SG files contain multiple images, usually thousands. The SG file itself acts as a "
"dictionary for all the images, the actual pixels are stored in "
"the .555 files that are also present in the games.\n\n");
	
	return text;
}
