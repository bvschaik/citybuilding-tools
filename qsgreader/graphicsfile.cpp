#include "graphicsfile.h"

#include <QImage>
#include <QDataStream>

GraphicsFile::GraphicsFile(QDataStream *reader, int width, int height) {
	this->reader = reader;
	this->width = width;
	this->height = height;
}

QImage * GraphicsFile::loadImage() {
	if (reader == NULL || reader->atEnd()) {
		return NULL;
	}
	
	reader->setByteOrder(QDataStream::LittleEndian);
	QImage *img = new QImage(width, height, QImage::Format_RGB32);
	
	for (int i = 0; i < height; i++) {
		readRow(img, i);
	}
	return img;
}

void GraphicsFile::readRow(QImage *img, int row) {
	quint16 color;
	quint32 rgb;
	for (int i = 0; i < width; i++) {
		*reader >> color;
		quint32 rgb = rgb555_to_rgb24(color);
		img->setPixel(i, row, rgb);
	}
}

quint32 GraphicsFile::rgb555_to_rgb24(quint16 rgb16) {
	quint32 red = 0, green = 0, blue = 0;
	
	red = (rgb16 & 0x7c00) >> 10;
	red = (red << 3) | (red >> 2);
	
	green = (rgb16 & 0x3E0) >> 5;
	green = (green << 3) | (green >> 3); // No, the last 3 is NOT a typo
	
	blue = (rgb16 & 0x1F);
	blue = (blue << 3) | (blue >> 2);
	
	return (red << 16) | (green << 8) | blue;
}
