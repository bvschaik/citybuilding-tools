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

#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QStringList>

class Logger
{
public:
    /**
     * Log an error message
     * \param message Message
     */
    void error(const QString &message);

    /**
     * Log a warning message
     * \param message Message
     */
    void warn(const QString &message);

    /**
     * Log an info message
     * \param message Message
     */
    void info(const QString &message);

    /**
     * Gets all messages in the order they were produced
     */
    const QStringList messages() const;

private:
    QStringList m_messages;
};

#endif // LOGGER_H
