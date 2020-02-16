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

#include "helpdialog.h"

#include <QtGui/QtGui>

HelpDialog::HelpDialog(QWidget *parent, const QString &appname)
: QDialog(parent) {
    QTextEdit *helpEdit = new QTextEdit();
    helpEdit->setReadOnly(true);
    helpEdit->setPlainText(getHelpText());
    helpEdit->setMinimumSize(480, 300);
    
    QTextEdit *licenseEdit = new QTextEdit();
    licenseEdit->setReadOnly(true);
    licenseEdit->setPlainText(getLicenseText(appname));
    licenseEdit->setMinimumSize(480, 300);
    
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->addTab(helpEdit, tr("Help"));
    tabWidget->addTab(licenseEdit, tr("License"));
    
    // Create ok button
    QPushButton *okButton = new QPushButton(tr("OK"), this);
    okButton->setDefault(true);
    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();
    
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(tabWidget);
    layout->addLayout(buttonLayout);

    setWindowTitle(appname + tr(" Help"));
    setLayout(layout);
    setModal(true);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

QString HelpDialog::getLicenseText(const QString &appname) {
    QString text = tr(
"{MYAPPNAME} 0.4\n"
"Copyright (c) 2017-2020 Bianca van Schaik <bvschaik@gmail.com>\n"
"{MYAPPNAME} is released under the MIT license, see below"
" for the license text.\n\n"
"The source code of this program can be downloaded at:\n"
"https://github.com/bvschaik/citybuilding-tools\n").replace("{MYAPPNAME}", appname);
    
    text += tr(
"-----------------------------------\n"
"MIT LICENSE\n"
"\n"
"Permission is hereby granted, free of charge, to any person obtaining a copy"
" of this software and associated documentation files (the \"Software\"), to deal"
" in the Software without restriction, including without limitation the rights"
" to use, copy, modify, merge, publish, distribute, sublicense, and/or sell"
" copies of the Software, and to permit persons to whom the Software is"
" furnished to do so, subject to the following conditions:\n"
"\n"
"The above copyright notice and this permission notice shall be included in all"
" copies or substantial portions of the Software.\n"
"\n"
"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR"
" IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,"
" FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE"
" AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER"
" LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,"
" OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE"
" SOFTWARE.\n");
    
    return text;
}

QString HelpDialog::getHelpText()
{
    QString text = tr(
"This program can convert language files for the Impressions Games citybuilding "
"games to and from XML format so they can be edited easily.\n\n"
"Usage should be self-explanatory. Select the correct tab to convert either "
"from ENG to XML, or from XML to ENG. Select the location of the input file, "
"select the location where the output file should be written, select the used "
"encoding for your file, and hit Convert.");

    return text;
}
