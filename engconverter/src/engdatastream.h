#ifndef ENGDATASTREAM_H
#define ENGDATASTREAM_H

#include "logger.h"

#include <QDataStream>

class EngDataStream : public QDataStream
{
public:
    EngDataStream(QIODevice &device, const QString &encoding, Logger &logger);
};

#endif // ENGDATASTREAM_H
