#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "imagebox.h"

class QTreeWidget;
class QTreeWidgetItem;
class QImage;
class SgFileRecord;
class SgImageRecord;

class MainWindow : public QMainWindow {
	Q_OBJECT
	
public:
    MainWindow();
	~MainWindow();
	
protected:
	//void closeEvent(QCloseEvent *event);
	
private slots:
	void openFile();
	void saveFile();
	void treeItemClicked(QTreeWidgetItem *item, int column);
	void treeSelectionChanged();

private:
	void clearImage();
	void loadImage(SgImageRecord *imagerecord);
	void loadFile(const QString &fileName);
	void createActions();
    void createMenus();
	void createTree(QList<SgFileRecord*> list);
	
	ImageBox *imagebox;
	QTreeWidget *treewidget;
	QString filename;
	SgImageRecord *selectedImage;
	
	QMenu *fileMenu;
	QAction *openAct;
	QAction *saveAct;
	QAction *exitAct;
	
	QFileDialog openFileDialog;
};

#endif /* MAINWINDOW_H */
