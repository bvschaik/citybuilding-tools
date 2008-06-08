#include "sgimagerecord.h"

#include <QDataStream>

SgImageRecord::SgImageRecord()
	: parent(0), invert_record(0)
{
	//qDebug("Deleting image record %d", id);
}

void SgImageRecord::load(QDataStream *stream, bool includeAlpha) {
	//qDebug("Creating image record %d", id);
	*stream >> offset;
	*stream >> length;
	*stream >> image_data_length;
	//stream->skipRawData(8);
	stream->readRawData((char*)bytes8, 4);
	*stream >> invert_offset;
	*stream >> width;
	*stream >> height;
	*stream >> unknown[0];
	*stream >> unknown[1];
	//stream->skipRawData(22);
	stream->readRawData((char*)bytes22, 22);
	*stream >> type;
	stream->readRawData(flags, 4);
	*stream >> file_id;
	//stream->skipRawData(7);
	stream->readRawData((char*)bytes7, 7);
	
	if (includeAlpha) {
		*stream >> alpha_offset;
		*stream >> alpha_length;
	} else {
		alpha_offset = alpha_length = 0;
	}
	
	/*
	// Print stuff:
	printf("(8) type %x (%x %x %x %x %x %x %x %x)\n", type,
		bytes8[0], bytes8[1], bytes8[2], bytes8[3],
		bytes8[4], bytes8[5], bytes8[6], bytes8[7]);
	printf("(22) type %x (%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x)\n", type,
		bytes22[0], bytes22[1], bytes22[2], bytes22[3],
		bytes22[4], bytes22[5], bytes22[6], bytes22[7],
		bytes22[8], bytes22[9], bytes22[10], bytes22[11],
		bytes22[12], bytes22[13], bytes22[14], bytes22[15],
		bytes22[16], bytes22[17], bytes22[18], bytes22[19],
		bytes22[20], bytes22[21]);
	printf("(7) type %x (%x %x %x %x %x %x %x)\n", type,
		bytes7[0], bytes7[1], bytes7[2], bytes7[3],
		bytes7[4], bytes7[5], bytes7[6]);
	printf("(flags) type %x (%d %d %d %d)\n", type, flags[0], flags[1], flags[2], flags[3]);
	*/
}
