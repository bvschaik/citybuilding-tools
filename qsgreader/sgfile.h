#ifndef SGFILE_H
#define SGFILE_H

#include <QString>
#include "sgfilerecord.h"

class QDataStream;
class SgFileHeader;

class SgFile {
	
public:
	SgFile(QString filename);
	QList<SgFileRecord*> loadFile();

private:
	bool openFile();
	void readHeader(SgFileHeader *header);
	
	QString filename;
	QDataStream *stream;
};

#endif /* SGFILE_H */
