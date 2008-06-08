#ifndef SGIMAGERECORD_H
#define SGIMAGERECORD_H

#include <QtGlobal>

class QDataStream;
class SgFileRecord;

class SgImageRecord {
public:
	SgImageRecord();
	void load(QDataStream *stream, bool includeAlpha);
	
	quint32 offset;
	quint32 length;
	quint32 image_data_length;
	/* 4 zero bytes: */
	quint8 bytes8[8];
	qint32 invert_offset;
	quint16 width;
	quint16 height;
	quint16 unknown[2];
	/* 22 zero bytes: */
	quint8 bytes22[22];
	quint16 type;
	/* 4 flag/option-like bytes */
	char flags[4];
	quint8 file_id;
	/* 3 bytes + 4 zero bytes */
	quint8 bytes7[7];
	/* For D6 and up SG3 versions: alpha masks */
	quint32 alpha_offset;
	quint32 alpha_length;
	quint32 id;
	SgFileRecord *parent;
	SgImageRecord *invert_record;
};

#endif /* SGIMAGERECORD_H */
