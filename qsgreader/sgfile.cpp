#include "sgfile.h"

#include <QDataStream>
#include <QString>
#include <QTreeWidget>
#include <QFile>

// internal "structs"
class SgFileHeader {
public:
	quint32 version;
	quint32 unknown0, unknown1;
	quint32 max_image_records;
	quint32 num_image_records;
	quint32 num_file_records;
	quint32 num_file_records_without_system; /* ? */
	quint32 total_filesize;
	quint32 filesize_555;
	quint32 filesize_external;
};



SgFile::SgFile(QString filename) {
	this->filename = filename;
}

QList<SgFileRecord*> SgFile::loadFile() {
	QList<SgFileRecord*> records;
	if (!openFile()) {
		return records;
	}
	qDebug("Opened file");
	SgFileHeader header;
	readHeader(&header);
	qDebug("Read header, num files = %d, num images = %d",
		header.num_file_records, header.num_image_records);
	for (quint32 i = 0; i < header.num_file_records; i++) {
		SgFileRecord *record = new SgFileRecord();
		record->load(stream);
		records.append(record);
	}
	qDebug("Read %d file records", records.size());
	
	// Skip to the start of the image records
	stream->device()->seek(20680);
	
	for (quint32 i = 0; i <= header.num_image_records; i++) {
		SgImageRecord *record = new SgImageRecord();
		record->load(stream);
		record->parent = records.at(record->file_id);
		records.at(record->file_id)->addImage(record);
	}
	qDebug("Read %d image records", header.num_image_records);
	delete stream;
	return records;
}

bool SgFile::openFile() {
	QFile *file = new QFile(filename);
	if (!file->open(QIODevice::ReadOnly)) {
		qDebug("unable to open file");
		return false;
	}
	
	stream = new QDataStream(file);
	stream->setByteOrder(QDataStream::LittleEndian);
	return true;
}

void SgFile::readHeader(SgFileHeader *header) {
	qDebug("Reading header");
	*stream >> header->version;
	qDebug("Version: %d", header->version);
	*stream >> header->unknown0;
	*stream >> header->unknown1;
	*stream >> header->max_image_records;
	*stream >> header->num_image_records;
	*stream >> header->num_file_records;
	*stream >> header->num_file_records_without_system;
	*stream >> header->total_filesize;
	*stream >> header->filesize_555;
	*stream >> header->filesize_external;
	
	// Seek to the end of the header
	stream->device()->seek(680);
}

