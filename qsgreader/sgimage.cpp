#include "sgimage.h"
#include "sgimagerecord.h"
#include "sgfilerecord.h"
#include <QImage>
#include <QDebug>

enum {
	ISOMETRIC_TILE_WIDTH = 58,
	ISOMETRIC_TILE_HEIGHT = 30,
	ISOMETRIC_TILE_BYTES = 1800
};

SgImage::SgImage(SgImageRecord *record, QString sgFileName) {
	this->record = record;
	this->sgFileName = sgFileName;
}

QImage* SgImage::loadImage() {
	QString filename = find555File();
	qDebug() << "555 filename: " << filename;
	
	quint8 *buffer = fillBuffer(filename);
	if (buffer == NULL) {
		return NULL;
	}
	
	QImage *result = NULL;
	
	switch (record->type) {
		case PlainImage:
		case 0:
		case 10:
		case 12:
			result = loadPlainImage(buffer);
			break;
		
		case IsometricImage:
			result = loadIsometricImage(buffer);
			break;
		
		case SpriteImage:
		case 276:
			result = loadSpriteImage(buffer);
			break;
	}
	
	delete buffer;
	return result;
}

QImage* SgImage::loadPlainImage(quint8 *buf) {
	// Check whether the image data is OK
	if (record->height * record->width * 2 != (int)record->length) {
		qDebug("Image data length doesn't match image size");
		return NULL;
	}
	quint8 *buffer = (quint8*)buf;
	QImage *img = new QImage(record->width, record->height, QImage::Format_ARGB32);
	img->fill(0); // Transparent black
	
	int i = 0;
	for (int y = 0; y < (int)record->height; y++) {
		for (int x = 0; x < (int)record->width; x++, i+= 2) {
			set555Pixel(img, x, y, buffer[i] | (buffer[i+1] << 8));
		}
	}
	
	return img;
}

QImage* SgImage::loadIsometricImage(quint8 *buffer) {
	QImage *img = new QImage(record->width, record->height, QImage::Format_ARGB32);
	img->fill(0); // Transparent black
	
	writeIsometricBase(img, buffer);
	
	writeTransparentImage(img, &buffer[record->image_data_length],
		record->length - record->image_data_length);
	
	return img;
}

QImage* SgImage::loadSpriteImage(quint8 *buffer) {
	QImage *img = new QImage(record->width, record->height, QImage::Format_ARGB32);
	img->fill(0); // Transparent black
	
	writeTransparentImage(img, buffer, record->length);
	
	return img;
}

void SgImage::writeIsometricBase(QImage *img, const quint8 *buffer) {
	int i = 0, x, y;
	int width, height, height_offset;
	int size = record->flags[3];
	int x_offset, y_offset;
	
	width = img->width();
	height = (width + 2) / 2; /* 58 -> 30, 118 -> 60, etc */
	height_offset = img->height() - height;
	y_offset = height_offset;
	
	/* Check if buffer length is enough: (width + 2) * height / 2 * 2bpp */
	if ((width + 2) * height != (int)record->image_data_length) {
		qDebug("Image data length doesn't match footprint size\n");
		return;
	}
	
	i = 0;
	for (y = 0; y < (size + (size - 1)); y++) {
		x_offset = (y < size ? (size - y - 1) : (y - size + 1))
			* ISOMETRIC_TILE_HEIGHT;
		for (x = 0; x < (y < size ? y + 1 : 2 * size - y - 1); x++, i++) {
			writeIsometricTile(img, &buffer[i * ISOMETRIC_TILE_BYTES],
				x_offset, y_offset);
			x_offset += ISOMETRIC_TILE_WIDTH + 2;
		}
		y_offset += ISOMETRIC_TILE_HEIGHT / 2;
	}
	
}

void SgImage::writeIsometricTile(QImage *img, const quint8 *buffer,
		int offset_x, int offset_y) {
	int half_height = ISOMETRIC_TILE_HEIGHT / 2;
	int height = ISOMETRIC_TILE_HEIGHT;
	int width = ISOMETRIC_TILE_WIDTH;
	int x, y, i = 0;
	
	for (y = 0; y < half_height; y++) {
		int start = height - 2 * (y + 1);
		int end = width - start;
		for (x = start; x < end; x++, i += 2) {
			set555Pixel(img, offset_x + x, offset_y + y,
				(buffer[i+1] << 8) | buffer[i]);
		}
	}
	for (y = half_height; y < height; y++) {
		int start = 2 * y - height;
		int end = width - start;
		for (x = start; x < end; x++, i += 2) {
			set555Pixel(img, offset_x + x, offset_y + y,
				(buffer[i+1] << 8) | buffer[i]);
		}
	}
}

void SgImage::writeTransparentImage(QImage *img, const quint8 *buffer, int length) {
	int i = 0;
	int x = 0, y = 0, j;
	int width = img->width();
	
	while (i < length) {
		quint8 c = buffer[i++];
		if (c == 255) {
			/* The next byte is the number of pixels to skip */
			x += buffer[i++];
			if (x >= width) {
				y++; x -= width;
			}
		} else {
			/* `c' is the number of image data bytes */
			for (j = 0; j < c; j++, i += 2) {
				set555Pixel(img, x, y, buffer[i] | (buffer[i+1] << 8));
				x++;
				if (x >= width) {
					y++; x = 0;
				}
			}
		}
	}
}

quint8 * SgImage::fillBuffer(QString filename) {
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) {
		return NULL;
	}
	QDataStream stream(&file);
	
	char *buffer = new char[record->length];
	if (buffer == NULL) {
		qDebug("Cannot allocate %d bytes of memory", record->length);
		return NULL;
	}
	
	if (record->parent->is_internal) {
		file.seek(record->offset);
	} else {
		// Somehow externals have 1 byte added to their offset
		file.seek(record->offset - 1);
	}
	
	/* Fill buffer */
	int bytes_read = stream.readRawData(buffer, record->length);
	if (bytes_read != (int)record->length) {
		qDebug("Unable to read %d bytes from file; read %d\n",
			record->length, bytes_read);
		file.close();
		delete buffer;
		return NULL;
	}
	qDebug("Filled buffer");
	return (quint8*) buffer;
}

void SgImage::set555Pixel(QImage *img, int x, int y, quint16 color) {
	if (color == 0xf81f) {
		return;
	}
	//quint32 red, green, blue;
	quint32 rgb = 0xff000000;
	
	// Red: bits 11-15, should go to bits 17-24
	rgb |= ((color & 0x7c00) << 9) | ((color & 0x7000) << 4);
	
	// Green: bits 6-10, should go to bits 9-16
	rgb |= ((color & 0x3e0) << 6) | ((color & 300));
	
	// Blue: bits 1-5, should go to bits 1-8
	rgb |= ((color & 0x1f) << 3) | ((color & 0x1c) >> 2);
	/*
	red = (color & 0x7c00) >> 10;
	red = (red << 3) | (red >> 2);
	
	green = (color & 0x3E0) >> 5;
	green = (green << 3) | (green >> 3); // No, the last 3 is NOT a typo
	
	blue = (color & 0x1F);
	blue = (blue << 3) | (blue >> 2);
	*/
	//img->setPixel(x, y, 0xff000000 | (red << 16) | (green << 8) | blue);
	img->setPixel(x, y, rgb);
}

QString SgImage::find555File() {
	QFileInfo fileinfo(sgFileName);
	
	// Fetch basename of the file
	// either the same name as sg(2|3) or from file record
	QString basename;
	if (record->parent->is_internal) {
		basename = fileinfo.fileName();
	} else {
		basename = QString(record->parent->filename);
	}
	
	// Change the extension to .555
	int position = basename.lastIndexOf('.');
	if (position != -1) {
		basename.replace(position + 1, 3, "555");
	}
	qDebug() << "Searching for file: " << basename;
	
	QString path = findFilenameCaseInsensitive(fileinfo.dir(), basename);
	if (path.length() > 0) {
		return path;
	}
	
	QDir dirinfo = fileinfo.dir();
	if (dirinfo.cd("555")) {
		return findFilenameCaseInsensitive(dirinfo, basename);
	}
	return "";
}

QString SgImage::findFilenameCaseInsensitive(QDir directory, QString filename) {
	filename = filename.toLower();
	
	QStringList files = directory.entryList(QStringList(filename), QDir::Files);
	for (int i = 0; i < files.size(); i++) {
		if (filename == files[i].toLower()) {
			return directory.absoluteFilePath(files[i]);
		}
		qDebug() << "No match: " << files[i];
	}
	
	return "";
}
