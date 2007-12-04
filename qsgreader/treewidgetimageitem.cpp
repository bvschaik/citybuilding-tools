#include "treewidgetimageitem.h"
#include "sgimagerecord.h"

TreeWidgetImageItem::TreeWidgetImageItem(int i, SgImageRecord *record)
	: QTreeWidgetItem(TreeWidgetImageItem::Type) {
	this->record = record;
	
	QString text = QString("%1: %2x%3, type %4")
                     .arg(i).arg(record->width).arg(record->height).arg(record->type);
	setText(0, text);
}

SgImageRecord* TreeWidgetImageItem::imageRecord() {
	return record;
}
