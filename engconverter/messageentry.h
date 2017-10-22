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

#ifndef MESSAGEENTRY_H
#define MESSAGEENTRY_H

#include <QString>

class MessageEntry
{
public:
    // inner classes
    class Image
    {
    public:
        int graphic;
        int x;
        int y;
    };
    class Window
    {
    public:
        int x;
        int y;
        int width;
        int height;
    };
    class String
    {
    public:
        int offset;
        QString text;
    };
    class StringWithPosition : public String
    {
    public:
        int x;
        int y;
    };
    
public:
    /**
     * Creates a new entry with the specified ID
     * \param id ID of the entry
     */
    MessageEntry(int id);
    
    int id() const;
    
    int type() const;
    void setType(int type);

    int subtype() const;
    void setSubtype(int subtype);

    Image &image1();

    Image &image2();

    Window &window();

    bool urgent() const;
    void setUrgent(bool urgent);

    StringWithPosition &title();

    StringWithPosition &subtitle();

    StringWithPosition &video();

    String &content();

    /**
     * Determines whether this entry is empty: has all fields set to 0
     * \return True if this entry is empty, false if at least one field is non-zero
     */
    bool isEmpty() const;

private:
    int m_id;
    int m_type;
    int m_subtype;
    Image m_image1;
    Image m_image2;
    Window m_window;
    bool m_urgent;
    StringWithPosition m_title;
    StringWithPosition m_subtitle;
    StringWithPosition m_video;
    String m_content;
};

#endif // MESSAGEENTRY_H
