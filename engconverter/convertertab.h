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

#ifndef CONVERTERTAB_H
#define CONVERTERTAB_H

#include <QtGui/QLineEdit>
#include <QtGui/QTextEdit>

class ConverterTab : public QWidget
{
    Q_OBJECT

public:
    enum FileType {
        XML,
        ENG
    };

    ConverterTab(FileType from, FileType to, QWidget *parent = 0);
    virtual ~ConverterTab();

private slots:
    void inputBrowse();
    void outputBrowse();
    void convertFile();

private:
    const QString &typeName(FileType type);
    const QString &typeExt(FileType type);
    
    FileType fromType;
    FileType toType;
    
    QLineEdit *inputFileEdit;
    QLineEdit *outputFileEdit;
    
    QTextEdit *convertOutputEdit;
};

#endif // CONVERTERTAB_H
