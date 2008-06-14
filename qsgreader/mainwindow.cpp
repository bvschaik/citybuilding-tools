#include "mainwindow.h"
#include "imagetreeitem.h"

MainWindow::MainWindow()
	: QMainWindow()
{
	createChildren();
	createActions();
	createMenu();
	
	if (QCoreApplication::arguments().size() > 1) {
		loadFile(QCoreApplication::arguments().at(1));
	}
	
	resize(600, 400);
}

/* Slots */
void MainWindow::openFile() {
	QString newfilename = QFileDialog::getOpenFileName(this, "Load SG file",
		filename, "Sierra Graphics files (*.sg2 *.sg3)");
	if (!newfilename.isEmpty()) {
		loadFile(newfilename);
	}
}

void MainWindow::saveFile() {
	QString pngfilename = QFileDialog::getSaveFileName(this, tr("Save Image"),
		filename, "PNG File (*.png)");
	if (!pngfilename.isEmpty()) {
		Q_ASSERT(!image.isNull());
		if (image.save(pngfilename, "png")) {
			qDebug("Image saved");
		} else {
			qDebug("Image could NOT be saved");
		}
	}
}

void MainWindow::extractAll() {
	//BatchExtractDialog dialog(this);
	//dialog.exec();
}

void MainWindow::treeSelectionChanged() {
	
	QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
	if (items.size() != 1) {
		qDebug("No selection");
		clearImage();
		return;
	}
	
	QTreeWidgetItem *item = items.at(0);
	
	if (item->type() == ImageTreeItem::ItemType) {
		ImageTreeItem *imageitem = (ImageTreeItem *)item;
		loadImage(imageitem->image());
	} else {
		clearImage();
	}
}

void MainWindow::loadFile(const QString &filename) {
	treeWidget->clear();
	treeWidget->setHeaderLabel("No file loaded");
	clearImage();
	
	sgFile = new SgFile(filename);
	if (!sgFile->load()) {
		return;
	}
	
	this->filename = filename;
	
	QFileInfo fi(filename);
	treeWidget->setHeaderLabel(fi.fileName());
	
	if (sgFile->bitmapCount() == 1 ||
			sgFile->imageCount(0) == sgFile->totalImageCount()) {
		// Just have a long list of images
		int numImages = sgFile->totalImageCount();
		for (int i = 0; i < numImages; i++) {
			QTreeWidgetItem *item = new ImageTreeItem(treeWidget, i,
				sgFile->image(i));
		}
	} else {
		// Split up by file
		int numBitmaps = sgFile->bitmapCount();
		for (int b = 0; b < numBitmaps; b++) {
			QTreeWidgetItem *bitmapItem =
				new QTreeWidgetItem(treeWidget,
					QStringList(sgFile->getBitmapDescription(b)));
			
			int numImages = sgFile->imageCount(b);
			for (int i = 0; i < numImages; i++) {
				ImageTreeItem *item = new ImageTreeItem(bitmapItem, i,
					sgFile->image(b, i));
			}
		}
	}
}

void MainWindow::loadImage(SgImage *img) {
	image = img->getImage();
	if (image.isNull()) {
		imageLabel->setText("Couldn't load image");
	} else {
		imageLabel->setPixmap(QPixmap::fromImage(image));
	}
	imageLabel->adjustSize();
}

void MainWindow::clearImage() {
	imageLabel->setPixmap(QPixmap());
}

/* Creating stuff */
void MainWindow::createChildren() {
	QSplitter *splitter = new QSplitter(this);
	QScrollArea *scroll = new QScrollArea(splitter);
	
	treeWidget = new QTreeWidget(splitter);
	treeWidget->setHeaderLabel("No file loaded");
	
	imageLabel = new QLabel();
	imageLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	
	scroll->setWidget(imageLabel);
	imageLabel->show();
	
	splitter->addWidget(treeWidget);
	splitter->addWidget(scroll);
	splitter->setStretchFactor(0, 0);
	splitter->setStretchFactor(1, 10);
	splitter->setSizes(QList<int>() << 200 << 400);
	setCentralWidget(splitter);
	
	connect(treeWidget, SIGNAL(itemSelectionChanged()),
			this, SLOT(treeSelectionChanged()));
}

void MainWindow::createActions() {
	openAction = new QAction("&Open...", this);
	connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
	
	saveAction = new QAction("&Save image...", this);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
	
	extractAllAction = new QAction("&Batch extract...", this);
	connect(extractAllAction, SIGNAL(triggered()), this, SLOT(extractAll()));
}

void MainWindow::createMenu() {
	QMenu *menu = menuBar()->addMenu("&File");
	menu->addAction(openAction);
	menu->addAction(saveAction);
	
	menu->addAction(extractAllAction);
}
