#include "sgimagerecord.h"

#include <QDataStream>

SgImageRecord::~SgImageRecord() {
	//qDebug("Deleting image record %d", id);
}

void SgImageRecord::load(QDataStream *stream) {
	//qDebug("Creating image record %d", id);
	*stream >> offset;
	*stream >> length;
	*stream >> image_data_length;
	stream->skipRawData(8);
	*stream >> width;
	*stream >> height;
	*stream >> unknown[0];
	*stream >> unknown[1];
	stream->skipRawData(22);
	*stream >> type;
	stream->readRawData(flags, 4);
	*stream >> file_id;
	stream->skipRawData(7);
}
