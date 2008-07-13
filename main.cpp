#include "sgfile.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char **argv) {
	QApplication app(argc, argv);
	MainWindow window;
	window.show();
	return app.exec();
	/*
	if (argc < 2) {
		qDebug("Usage: <exe> <sg filename>");
		return 1;
	}
	SgFile *file = new SgFile(QString(argv[1]));
	if (!file->load()) {
		qWarning("Couldn't load the SG file");
	} else {
		qDebug("File contains %d bitmap records and %d image records",
			file->bitmapCount(), file->totalImageCount());
		int max = file->totalImageCount();
		int good = 0;
		for (int i = 0; i < max; i++) {
			QImage img = file->getImage(i);
			if (!img.isNull()) {
				good++;
				//if (i == 651 || i == 993)
					img.save(QString("output/%0.png").arg(i + 1, 5, 10, QChar('0')));
			} else {
				qDebug("Image %d is bad", i);
			}
		}
		qDebug("Found %d usable images", good);
	}
	*/
}
