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

#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QStringList>

/**
 * Collects log messages
 */
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

    /**
     * Set context data which is logged before every error/warning
     * \param context Context info
     */
    void setContext(const QString &context);

private:
    void addMessage(const QString &message);
    
    QStringList m_messages;
    QString m_context;
};

#endif // LOGGER_H
