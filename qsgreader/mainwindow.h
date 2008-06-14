#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui> // TODO: specify

#include "sgfile.h"
#include "sgimage.h"

class QAction;

class MainWindow : public QMainWindow {
	Q_OBJECT
	
	public:
		MainWindow();
		//~MainWindow();
		
	
	private slots:
		void openFile();
		void saveFile();
		void extractAll();
		void treeSelectionChanged();
		
	private:
		void createChildren();
		void createMenu();
		void createActions();
		
		void loadFile(const QString &filename);
		void loadImage(SgImage *image);
		void clearImage();
		
		QTreeWidget *treeWidget;
		QLabel *imageLabel;
		QString filename;
		QImage image;
		SgFile *sgFile;
		
		QAction *openAction;
		QAction *saveAction;
		QAction *extractAllAction;
};

#endif /* MAINWINDOW_H */
