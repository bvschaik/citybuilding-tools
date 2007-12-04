#include "sgfilerecord.h"

#include <QDataStream>
/*
SgFileRecord::SgFileRecord() {
	//images = new QList<SgImageRecord>();
	images();
}
*/
void SgFileRecord::load(QDataStream *stream) {
	stream->readRawData(filename, 65);
	filename[64] = 0;
	stream->readRawData(comment, 51);
	comment[50] = 0;
	
	*stream >> width;
	*stream >> height;
	*stream >> num_images;
	*stream >> start_index;
	*stream >> end_index;
	stream->skipRawData(32);
	*stream >> is_internal;
	stream->skipRawData(28);
}

void SgFileRecord::addImage(SgImageRecord *image) {
	images.append(image);
}
