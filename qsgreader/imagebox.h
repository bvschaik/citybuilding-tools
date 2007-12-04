#ifndef IMAGEBOX_H
#define IMAGEBOX_H

#include <QScrollArea>

class QLabel;

class ImageBox : public QScrollArea {
	
public:
	ImageBox();
	QImage * image();
	void setImage(QImage *img);
	
private:
	QLabel *imageLabel;
	QImage *img;
};

#endif /* IMAGEBOX_H */
