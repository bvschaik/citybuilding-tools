#include "imagetreeitem.h"

ImageTreeItem::ImageTreeItem(QTreeWidget *parent, int id, SgImage *image)
	: QTreeWidgetItem(parent, ItemType), imageRecord(image), imageId(id)
{
	setColumnData();
}

ImageTreeItem::ImageTreeItem(QTreeWidgetItem *parent, int id, SgImage *image)
	: QTreeWidgetItem(parent, ItemType), imageRecord(image), imageId(id)
{
	setColumnData();
}

SgImage *ImageTreeItem::image() {
	return imageRecord;
}

void ImageTreeItem::setColumnData() {
	setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	setData(0, Qt::DisplayRole, QString("%0: %1").arg(imageId + 1, 5)
		.arg(imageRecord->description()));
	//setData(0, Qt::DisplayRole, imageId);
	//setData(1, Qt::DisplayRole, imageRecord->description());
}
