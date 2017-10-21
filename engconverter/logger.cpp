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

#include "logger.h"

void Logger::error(const QString &message)
{
    m_messages.append(QString("ERROR: %1").arg(message));
}

void Logger::warn(const QString &message)
{
    m_messages.append(QString("Warning: %1").arg(message));
}

void Logger::info(const QString &message)
{
    m_messages.append(message);
}

const QStringList Logger::messages() const
{
    return m_messages;
}
