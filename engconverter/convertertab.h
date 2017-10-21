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
