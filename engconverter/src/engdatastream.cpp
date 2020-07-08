#include "engdatastream.h"

#include "traditionalchinesecodec.h"

#include <QTextCodec>

EngDataStream::EngDataStream(QIODevice &device, const QString &encoding, Logger &logger)
: QDataStream(&device)
{
    if (encoding.compare(QString("c3-tc")) == 0) {
        QTextCodec::setCodecForCStrings(new TraditionalChineseCodec(logger));
    } else {
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName(encoding.toAscii()));
    }
    setByteOrder(QDataStream::LittleEndian);
}
