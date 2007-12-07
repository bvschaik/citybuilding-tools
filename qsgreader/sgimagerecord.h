#ifndef SGIMAGERECORD_H
#define SGIMAGERECORD_H

#include <QtGlobal>

class QDataStream;
class SgFileRecord;

class SgImageRecord {
public:
	~SgImageRecord();
	void load(QDataStream *stream);
	
	quint32 offset;
	quint32 length;
	quint32 image_data_length;
	/* 8 zero bytes */
	quint16 width;
	quint16 height;
	quint16 unknown[2];
	/* 22 zero bytes */
	quint16 type;
	/* 4 flag/option-like bytes */
	char flags[4];
	quint8 file_id;
	/* 3 bytes + 4 zero bytes */
	quint32 id;
	SgFileRecord *parent;
};

#endif /* SGIMAGERECORD_H */
