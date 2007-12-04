#ifndef SGTREEWIDGETITEM_H
#define SGTREEWIDGETITEM_H

#include <QTreeWidgetItem>

class SgTreeWidgetItem : public QTreeWidgetItem {
	
public:
	SgTreeWidgetItem(int number);
	SgTreeWidgetItem(QString text);
	
	enum ItemType {
		FileType = 1001,
		ImageType = 1002
	};
};

#endif /* SGTREEWIDGETITEM_H */
