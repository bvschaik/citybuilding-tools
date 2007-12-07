#ifndef TREEWIDGETFILEITEM_H
#define TREEWIDGETFILEITEM_H

#include <QTreeWidgetItem>

class SgFileRecord;

class TreeWidgetFileItem : public QTreeWidgetItem {

public:
	TreeWidgetFileItem(SgFileRecord *record);
	~TreeWidgetFileItem();
	SgFileRecord* fileRecord();
	static const int Type = 1001;

private:
	SgFileRecord *record;
};

#endif /* TREEWIDGETFILEITEM_H */
