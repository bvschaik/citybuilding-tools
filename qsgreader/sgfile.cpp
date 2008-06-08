#include "sgfile.h"

#include <QDataStream>
#include <QString>
#include <QTreeWidget>
#include <QFile>
#include <QFileInfo>

// internal "structs"
class SgFileHeader {
public:
	quint32 sg_filesize;
	quint32 version;
	quint32 unknown1;
	quint32 max_image_records;
	quint32 num_image_records;
	quint32 num_file_records;
	quint32 num_file_records_without_system; /* ? */
	quint32 total_filesize;
	quint32 filesize_555;
	quint32 filesize_external;
};

enum {
	SG_HEADER_SIZE = 680
};

SgFile::SgFile(QString filename) {
	this->filename = filename;
	stream = NULL;
}

SgFile::~SgFile() {
	if (stream != NULL) {
		delete stream->device();
		delete stream;
	}
}

/**
* Loads the file and returns the file records within it as list of 
* SgFileRecord objects
* @return list: empty if loading the file failed because the file didn't 
* exist or the file is not an sg2 or sg3 file.
*/
QList<SgFileRecord*> SgFile::loadFile() {
	QList<SgFileRecord*> records;
	if (!openFile()) {
		return records;
	}
	qDebug("Opened file");
	SgFileHeader header;
	readHeader(&header);
	
	if (!checkVersion(header.version, header.sg_filesize)) {
		return records;
	}
	
	quint32 fileRecordCount = getNumFileRecords(header.version);
	
	qDebug("Read header, num files = %d, num images = %d",
		header.num_file_records, header.num_image_records);
	for (quint32 i = 0; i < header.num_file_records; i++) {
		SgFileRecord *record = new SgFileRecord();
		record->load(stream);
		records.append(record);
	}
	qDebug("Read %d file records", records.size());
	
	// Skip to the start of the image records
	stream->device()->seek(SG_HEADER_SIZE +
		fileRecordCount * SgFileRecord::RecordSize);
	
	readImages(header, records);
	
	/*
	for (quint32 i = 0; i <= header.num_image_records; i++) {
		SgImageRecord *record = new SgImageRecord();
		record->load(stream);
		record->parent = records.at(record->file_id);
		records.at(record->file_id)->addImage(record);
	}
	qDebug("Read %d image records", header.num_image_records);
	*/
	return records;
}

void SgFile::readImages(SgFileHeader header, QList<SgFileRecord*> records) {
	// There is one dummy (null) record in front of the lot
	SgImageRecord dummy;
	dummy.load(stream, header.version >= 0xd6);
	SgImageRecord **tempRecords = new SgImageRecord*[header.num_image_records];
	
	for (quint32 i = 0; i < header.num_image_records; i++) {
		SgImageRecord *record = new SgImageRecord();
		record->id = i;
		record->load(stream, header.version >= 0xd6);
		if (record->invert_offset < 0 && (i + record->invert_offset) >= 0) {
			record->invert_record = tempRecords[i + record->invert_offset];
		}
		record->parent = records.at(record->file_id);
		if (i == 200) {
			qDebug("Record 200 has parent %d", record->file_id);
		}
		tempRecords[i] = record;
		records.at(record->file_id)->addImage(record); // TODO: UNSAFE
	}
	delete[] tempRecords;
	qDebug("Read %d image records", header.num_image_records);
}

bool SgFile::openFile() {
	QFile *file = new QFile(filename);
	if (!file->open(QIODevice::ReadOnly)) {
		qDebug("unable to open file");
		delete file;
		return false;
	}
	
	stream = new QDataStream(file);
	stream->setByteOrder(QDataStream::LittleEndian);
	return true;
}

bool SgFile::checkVersion(quint32 version, quint32 filesize) {
	if (version == 0xd3) {
		// SG2 file: filesize = 74480 or 522680 (depending on whether it's
		// a "normal" sg2 or an enemy sg2
		if (filesize == 74480 || filesize == 522680) {
			return true;
		}
	} else if (version == 0xd5 || version == 0xd6) {
		// SG3 file: filesize = the actual size of the sg3 file
		QFileInfo fi(filename);
		if (filesize == 74480 || fi.size() == filesize) {
			return true;
		}
	}
	
	// All other cases:
	return false;
}

quint32 SgFile::getNumFileRecords(quint32 version) {
	if (version == 0xd3) {
		return 100; // SG2
	} else {
		return 200; // SG3
	}
}

void SgFile::readHeader(SgFileHeader *header) {
	qDebug("Reading header");
	*stream >> header->sg_filesize;
	*stream >> header->version;
	qDebug("Version: %d", header->version);
	*stream >> header->unknown1;
	*stream >> header->max_image_records;
	*stream >> header->num_image_records;
	*stream >> header->num_file_records;
	*stream >> header->num_file_records_without_system;
	*stream >> header->total_filesize;
	*stream >> header->filesize_555;
	*stream >> header->filesize_external;
	
	// Seek to the end of the header
	stream->device()->seek(SG_HEADER_SIZE);
}

