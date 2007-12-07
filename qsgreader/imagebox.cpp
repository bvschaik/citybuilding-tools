#include <QLabel>
#include <QImage>
#include "imagebox.h"

ImageBox::ImageBox() : QScrollArea() {
	img = NULL;
	imageLabel = new QLabel();
	setWidget(imageLabel);
}

ImageBox::~ImageBox() {
	qDebug("Destroying image box");
	if (img != NULL) {
		delete img;
	}
}

QImage* ImageBox::image() {
	return img;
}

void ImageBox::setImage(QImage *image) {
	if (img != NULL) {
		delete img;
	}
	this->img = image;
	//qDebug("image: %d", image);
	if (img == NULL || (img->height() == 0 && img->width() == 0)) {
		//qDebug("image is null");
		imageLabel->setPixmap(NULL);
	} else {
		//qDebug("image is NOT null: width = %d, height = %d", img->width(), img->height());
		imageLabel->setPixmap(QPixmap::fromImage(*img));
	}
	imageLabel->adjustSize();
}
