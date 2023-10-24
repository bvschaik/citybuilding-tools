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
#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QtCore/QString>
#include <QtWidgets/QDialog>

class HelpDialog : public QDialog {
	public:
		/**
		* Constructor, constructs a new help dialog
		* @param parent Parent widget
		* @param appname Name of the application, used in the title bar
		*/
		HelpDialog(QWidget *parent, const QString & appname);
	
	private:
		QString getHelpText();
		
		QString appname;
};

#endif /* HELPDIALOG_H */
