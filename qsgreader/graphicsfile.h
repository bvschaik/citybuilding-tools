#ifndef GRAPHICS_FILE_H
#define GRAPHICS_FILE_H

#include <QtGlobal>

class QDataStream;
class QImage;

class GraphicsFile {
public:
	GraphicsFile(QDataStream *reader, int width, int height);
	QImage * loadImage();
	
private:
	quint32 rgb555_to_rgb24(quint16 rgb16);
	void readRow(QImage *img, int row);
	
	int width;
	int height;
	QDataStream *reader;
};

#endif /* GRAPHICS_FILE_H */
