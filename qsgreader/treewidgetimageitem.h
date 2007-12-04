#ifndef TREEWIDGETIMAGEITEM_H
#define TREEWIDGETIMAGEITEM_H

#include <QTreeWidgetItem>

class SgImageRecord;

class TreeWidgetImageItem : public QTreeWidgetItem {

public:
	TreeWidgetImageItem(int i, SgImageRecord *record);
	SgImageRecord* imageRecord();
	static const int Type = 1002;

private:
	SgImageRecord *record;
};

#endif /* TREEWIDGETIMAGEITEM_H */
