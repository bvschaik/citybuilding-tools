#ifndef SGIMAGE_H
#define SGIMAGE_H

#include <QString>
#include <QDir>

class QImage;
class SgImageRecord;

class SgImage {
public:
	SgImage(SgImageRecord *record, QString sgFileName);
	QImage* loadImage();

private:
	QString find555File();
	quint8* fillBuffer(QString filename);
	QString findFilenameCaseInsensitive(QDir directory, QString filename);
	QImage* loadPlainImage(quint8 *buffer);
	QImage* loadIsometricImage(quint8 *buffer);
	QImage* loadSpriteImage(quint8 *buffer);
	void writeIsometricBase(QImage *img, const quint8 *buffer);
	void writeIsometricTile(QImage *img, const quint8 *buffer,
		int offset_x, int offset_y, int tile_width, int tile_height);
	void writeTransparentImage(QImage *img, const quint8 *buffer, int length);
	void set555Pixel(QImage *img, int x, int y, quint16 color);
	
	enum ImageType {
		PlainImage = 1,
		IsometricImage = 30,
		SpriteImage = 256
	};
	
	SgImageRecord *record;
	QString sgFileName;
};

#endif /* SGIMAGE_H */
