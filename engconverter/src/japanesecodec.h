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

#ifndef JAPANESECODEC_H
#define JAPANESECODEC_H

#include <QTextCodec>
#include <QList>

class JpUnicodeConv {
public:
    virtual ~JpUnicodeConv() {}
    enum Rules {
        // "ASCII" is ANSI X.3.4-1986, a.k.a. US-ASCII here.
        Default                        = 0x0000,

        Unicode                        = 0x0001,
        Unicode_JISX0201                = 0x0001,
        Unicode_ASCII                 = 0x0002,
        JISX0221_JISX0201         = 0x0003,
        JISX0221_ASCII                = 0x0004,
        Sun_JDK117                     = 0x0005,
        Microsoft_CP932                = 0x0006,

        NEC_VDC                = 0x0100,                // NEC Vender Defined Char
        UDC                        = 0x0200,                // User Defined Char
        IBM_VDC                = 0x0400                // IBM Vender Defined Char
    };
    static JpUnicodeConv *newConverter(int rule);

    virtual uint asciiToUnicode(uint h, uint l) const;
    /*virtual*/ uint jisx0201ToUnicode(uint h, uint l) const;
    virtual uint jisx0201LatinToUnicode(uint h, uint l) const;
    /*virtual*/ uint jisx0201KanaToUnicode(uint h, uint l) const;
    virtual uint jisx0208ToUnicode(uint h, uint l) const;
    virtual uint jisx0212ToUnicode(uint h, uint l) const;

    uint asciiToUnicode(uint ascii) const {
        return asciiToUnicode((ascii & 0xff00) >> 8, (ascii & 0x00ff));
    }
    uint jisx0201ToUnicode(uint jis) const {
        return jisx0201ToUnicode((jis & 0xff00) >> 8, (jis & 0x00ff));
    }
    uint jisx0201LatinToUnicode(uint jis) const {
        return jisx0201LatinToUnicode((jis & 0xff00) >> 8, (jis & 0x00ff));
    }
    uint jisx0201KanaToUnicode(uint jis) const {
        return jisx0201KanaToUnicode((jis & 0xff00) >> 8, (jis & 0x00ff));
    }
    uint jisx0208ToUnicode(uint jis) const {
        return jisx0208ToUnicode((jis & 0xff00) >> 8, (jis & 0x00ff));
    }
    uint jisx0212ToUnicode(uint jis) const {
        return jisx0212ToUnicode((jis & 0xff00) >> 8, (jis & 0x00ff));
    }

    virtual uint unicodeToAscii(uint h, uint l) const;
    /*virtual*/ uint unicodeToJisx0201(uint h, uint l) const;
    virtual uint unicodeToJisx0201Latin(uint h, uint l) const;
    /*virtual*/ uint unicodeToJisx0201Kana(uint h, uint l) const;
    virtual uint unicodeToJisx0208(uint h, uint l) const;
    virtual uint unicodeToJisx0212(uint h, uint l) const;

    uint unicodeToAscii(uint unicode) const {
        return unicodeToAscii((unicode & 0xff00) >> 8, (unicode & 0x00ff));
    }
    uint unicodeToJisx0201(uint unicode) const {
        return unicodeToJisx0201((unicode & 0xff00) >> 8, (unicode & 0x00ff));
    }
    uint unicodeToJisx0201Latin(uint unicode) const {
        return unicodeToJisx0201Latin((unicode & 0xff00) >> 8, (unicode & 0x00ff));
    }
    uint unicodeToJisx0201Kana(uint unicode) const {
        return unicodeToJisx0201Kana((unicode & 0xff00) >> 8, (unicode & 0x00ff));
    }
    uint unicodeToJisx0208(uint unicode) const {
        return unicodeToJisx0208((unicode & 0xff00) >> 8, (unicode & 0x00ff));
    }
    uint unicodeToJisx0212(uint unicode) const {
        return unicodeToJisx0212((unicode & 0xff00) >> 8, (unicode & 0x00ff));
    }

    uint sjisToUnicode(uint h, uint l) const;
    uint unicodeToSjis(uint h, uint l) const;
    uint sjisibmvdcToUnicode(uint h, uint l) const;
    uint unicodeToSjisibmvdc(uint h, uint l) const;
    uint cp932ToUnicode(uint h, uint l) const;
    uint unicodeToCp932(uint h, uint l) const;

    uint sjisToUnicode(uint sjis) const {
        return sjisToUnicode((sjis & 0xff00) >> 8, (sjis & 0x00ff));
    }
    uint unicodeToSjis(uint unicode) const {
        return unicodeToSjis((unicode & 0xff00) >> 8, (unicode & 0x00ff));
    }

protected:
    explicit JpUnicodeConv(int r) : rule(r) {}

private:
    int rule;
};

class JapaneseCodec : public QTextCodec
{
public:
    JapaneseCodec();
    ~JapaneseCodec();

    QByteArray name() const;
    int mibEnum() const;

    QString convertToUnicode(const char *, int, ConverterState *) const;
    QByteArray convertFromUnicode(const QChar *, int, ConverterState *) const;

protected:
    const JpUnicodeConv *conv;
};

#endif // JAPANESECODEC_H
