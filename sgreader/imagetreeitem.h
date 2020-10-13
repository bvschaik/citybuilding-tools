#ifndef IMAGETREEITEM_H
#define IMAGETREEITEM_H

#include <QtCore/QString>
#include <QtWidgets/QTreeWidgetItem>

#include "sgimage.h"

class ImageTreeItem : public QTreeWidgetItem {
	public:
		ImageTreeItem(QTreeWidget *parent, int id, SgImage *image);
		ImageTreeItem(QTreeWidgetItem *parent, int id, SgImage *image);
		
		SgImage *image();
		
		static const int ItemType = QTreeWidgetItem::UserType;
		
	private:
		void setColumnData();
		SgImage *imageRecord;
		int imageId;
};

#endif /* IMAGETREEITEM_H */
