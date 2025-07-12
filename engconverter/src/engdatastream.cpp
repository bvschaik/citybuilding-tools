#include "engdatastream.h"

#include "japanesecodec.h"
#include "koreancodec.h"
#include "simplifiedchinesecodec.h"
#include "traditionalchinesecodec.h"

#include <QTextCodec>

EngDataStream::EngDataStream(QIODevice &device, const QString &encoding, Logger &logger)
: QDataStream(&device)
{
    if (encoding.compare(QString("c3-tc")) == 0) {
        QTextCodec::setCodecForCStrings(new TraditionalChineseCodec(logger));
    } else if (encoding.compare(QString("c3-sc")) == 0) {
        QTextCodec::setCodecForCStrings(new SimplifiedChineseCodec(logger));
    } else if (encoding.compare(QString("CP949")) == 0) {
        QTextCodec::setCodecForCStrings(new KoreanCodec());
    } else if (encoding.compare(QString("Shift_JIS")) == 0) {
        QTextCodec::setCodecForCStrings(new JapaneseCodec());
    } else {
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName(encoding.toAscii()));
    }
    setByteOrder(QDataStream::LittleEndian);
}
