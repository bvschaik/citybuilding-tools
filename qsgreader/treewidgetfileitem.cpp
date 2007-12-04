#include "treewidgetfileitem.h"
#include "sgfilerecord.h"

TreeWidgetFileItem::TreeWidgetFileItem(SgFileRecord *record)
	: QTreeWidgetItem(TreeWidgetFileItem::Type) {
	this->record = record;
	setText(0, QString(record->filename));
}

SgFileRecord* TreeWidgetFileItem::fileRecord() {
	return record;
}
