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

#include "messageentry.h"

MessageEntry::MessageEntry(int id)
: m_id(id)
{}

int MessageEntry::id() const
{
    return m_id;
}

int MessageEntry::type() const
{
    return m_type;
}

void MessageEntry::setType(int type)
{
    m_type = type;
}

int MessageEntry::subtype() const
{
    return m_subtype;
}

void MessageEntry::setSubtype(int subtype)
{
    m_subtype = subtype;
}

MessageEntry::Image &MessageEntry::image()
{
    return m_image;
}

MessageEntry::Image &MessageEntry::image2()
{
    return m_image2;
}

bool MessageEntry::urgent() const
{
    return m_urgent;
}

void MessageEntry::setUrgent(bool urgent)
{
    m_urgent = urgent;
}

MessageEntry::Window &MessageEntry::window()
{
    return m_window;
}

MessageEntry::StringWithPosition &MessageEntry::title()
{
    return m_title;
}

MessageEntry::StringWithPosition &MessageEntry::subtitle()
{
    return m_subtitle;
}

MessageEntry::StringWithPosition &MessageEntry::video()
{
    return m_video;
}

MessageEntry::String &MessageEntry::content()
{
    return m_content;
}

bool MessageEntry::isEmpty() const
{
    if (m_type || m_subtype || m_urgent) {
        return false;
    }
    if (!m_image.isEmpty() || !m_image2.isEmpty()) {
        return false;
    }
    return m_title.isEmpty() && m_subtitle.isEmpty() && m_video.isEmpty() && m_content.isEmpty();
}

bool MessageEntry::Image::isEmpty() const
{
    return graphic == 0;
}

bool MessageEntry::String::isEmpty() const
{
    return offset == 0 && text.isEmpty();
}
