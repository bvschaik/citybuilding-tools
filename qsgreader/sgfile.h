#ifndef SGFILE_H
#define SGFILE_H

#include <QString>
#include "sgfilerecord.h"

class QDataStream;
class SgFileHeader;

class SgFile {
	
public:
	SgFile(QString filename);
	~SgFile();
	QList<SgFileRecord*> loadFile();

private:
	bool openFile();
	bool checkVersion(quint32 version, quint32 filesize);
	void readImages(SgFileHeader header, QList<SgFileRecord*> records);
	quint32 getNumFileRecords(quint32 version);
	void readHeader(SgFileHeader *header);
	
	QString filename;
	QDataStream *stream;
};

#endif /* SGFILE_H */
