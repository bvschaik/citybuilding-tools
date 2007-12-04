#ifndef SGFILERECORD_H
#define SGFILERECORD_H

#include "sgimagerecord.h"
#include <QList>

class QDataStream;

class SgFileRecord {
public:
	//SgFileRecord();
	void load(QDataStream *stream);
	void addImage(SgImageRecord *image);
	
	char filename[65];
	char comment[51];
	quint32 width;
	quint32 height;
	quint32 num_images;
	quint32 start_index;
	quint32 end_index;
	/* 4 bytes - quint32 between start & end */
	/* 16b, 4x int with unknown purpose */
	/*  8b, 2x int with (real?) width & height */
	/* 12b, 3x int: if any is non-zero: internal image */
	/* 24 more misc bytes, most zero */
	quint32 is_internal;
	QList<SgImageRecord*> images;
};

#endif /* SGFILERECORD_H */
