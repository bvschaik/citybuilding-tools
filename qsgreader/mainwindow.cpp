#include <QtGui>
#include <QDebug>

#include "mainwindow.h"
#include "graphicsfile.h"
#include "treewidgetfileitem.h"
#include "treewidgetimageitem.h"
#include "sgfilerecord.h"
#include "sgfile.h"
#include "sgimage.h"

MainWindow::MainWindow() {
	
	imagebox = new ImageBox();
	//setCentralWidget(imagebox);
	QSplitter *splitter = new QSplitter(this);
	
	treewidget = new QTreeWidget();
	treewidget->setHeaderLabel("No file loaded");
	/*
	QTreeWidgetItem *root = new SgTreeWidgetItem("First bmp");
	
	QTreeWidgetItem *child;
	child = new SgTreeWidgetItem(1);
	root->addChild(child);
	
	child = new SgTreeWidgetItem(2);
	root->addChild(child);
	
	
	treeWidget->addTopLevelItem(root);
	*/
	splitter->addWidget(treewidget);
	splitter->addWidget(imagebox);
	splitter->setStretchFactor(0, 0);
	splitter->setStretchFactor(1, 10);
	splitter->setSizes(QList<int>() << 200 << 300);
	setCentralWidget(splitter);
	
	createActions();
    createMenus();
	
	//connect(treewidget, SIGNAL(itemActivated(QTreeWidgetItem *, int)),
	//		this, SLOT(treeItemClicked(QTreeWidgetItem *, int)));
	//connect(treewidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
	//		this, SLOT(treeItemClicked(QTreeWidgetItem *, int)));
	
	connect(treewidget, SIGNAL(itemSelectionChanged()),
			this, SLOT(treeSelectionChanged()));
	
	if (QCoreApplication::arguments().size() > 1) {
		loadFile(QCoreApplication::arguments().at(1));
	}
	resize(500, 400);
}

void MainWindow::openFile() {
	QString filename = QFileDialog::getOpenFileName(this);
	if (!filename.isEmpty()) {
		loadFile(filename);
	}
}

void MainWindow::saveFile() {
	QString pngfilename = QFileDialog::getSaveFileName(this, tr("Save Image"),
		filename, "PNG File (*.png)");
	if (!pngfilename.isEmpty()) {
		QImage *img = imagebox->image();
		Q_ASSERT(img != NULL);
		if (img->save(pngfilename, "png")) {
			qDebug("Image saved");
		} else {
			qDebug("Image could NOT be saved");
		}
	}
}

void MainWindow::loadFile(const QString &filename) {
	this->filename = filename;
	
	qDebug() << "Filename to open: " << filename;
	SgFile sgfile(filename);
	QFileInfo fi(filename);
	treewidget->setHeaderLabel(fi.fileName());
	createTree(sgfile.loadFile());
	//imagebox->setImage(new QImage(filename));
}

void MainWindow::treeItemClicked(QTreeWidgetItem *item, int /* column */) {
	if (item == NULL) {
		qDebug("Empty item clicked");
		clearImage();
		return;
	}
	
	if (item->type() == TreeWidgetFileItem::Type) {
		qDebug("File record clicked");
		clearImage();
		return;
	} else if (item->type() != TreeWidgetImageItem::Type) {
		qDebug("Unknown widget type clicked");
		return;
	}
	
	TreeWidgetImageItem *imageitem = (TreeWidgetImageItem*) item;
	
	loadImage(imageitem->imageRecord());
}

void MainWindow::treeSelectionChanged() {
	QList<QTreeWidgetItem*> items = treewidget->selectedItems();
	if (items.size() != 1) {
		qDebug("No selection");
		clearImage();
		return;
	}
	
	QTreeWidgetItem *item = items.at(0);
	
	if (item->type() == TreeWidgetFileItem::Type) {
		qDebug("File record clicked");
		clearImage();
		return;
	} else if (item->type() != TreeWidgetImageItem::Type) {
		qDebug("Unknown widget type clicked");
		return;
	}
	
	TreeWidgetImageItem *imageitem = (TreeWidgetImageItem*) item;
	
	loadImage(imageitem->imageRecord());
}

void MainWindow::clearImage() {
	imagebox->setImage(NULL);
	selectedImage = NULL;
	saveAct->setEnabled(false);
}

void MainWindow::loadImage(SgImageRecord *imagerecord) {
	if (imagerecord == selectedImage) {
		return;
	}
	
	QTime t;
	t.start();
	
	SgImage sgimage(imagerecord, filename);
	QImage *img = sgimage.loadImage();
	
	qDebug("Loading image: %d msec", t.restart());
	
	imagebox->setImage(img);
	selectedImage = imagerecord;
	saveAct->setEnabled(true);
	
	qDebug("Setting image: %d msec", t.elapsed());
	/*
	image = new QImage("100k.png");
	imagebox->setImage(image);
	// 
	QFile file("images/The_empire.555");
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug("unable to open file");
		return;
	}
	
	QDataStream stream(&file);
	GraphicsFile graphics(&stream, 2000, 1000);
	image = graphics.loadImage();
	if (image == NULL) {
		qDebug("Image is null!");
	} else {
		qDebug("Dimensions: %d x %d", image->width(), image->height());
	}
	imagebox->setImage(image);
	*/
}

void MainWindow::createActions() {
	// Open file action
	openAct = new QAction(tr("&Open"), this);
	openAct->setShortcut(tr("Ctrl+O"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));
	
	// Save file action
	saveAct = new QAction(tr("&Save"), this);
	saveAct->setShortcut(tr("Ctrl+S"));
	saveAct->setEnabled(false);
	connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));
	
	// Exit action
	exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createMenus() {
	fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
}

void MainWindow::createTree(QList<SgFileRecord*> list) {
	QTreeWidgetItem *item;
	QTreeWidgetItem *child;
	
	// Clear tree first
	treewidget->clear();
	
	// Add widgets recursively
	for (int i = 0; i < list.size(); i++) {
		SgFileRecord *filerec = list.at(i);
		item = new TreeWidgetFileItem(filerec);
		
		// Add child records (images)
		for (int j = 0; j < (int)filerec->num_images; j++) {
			child = new TreeWidgetImageItem(j, filerec->images[j]);
			item->addChild(child);
		}
		
		treewidget->addTopLevelItem(item);
	}
}
