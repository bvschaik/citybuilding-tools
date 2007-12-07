#include "treewidgetfileitem.h"
#include "sgfilerecord.h"

TreeWidgetFileItem::TreeWidgetFileItem(SgFileRecord *record)
	: QTreeWidgetItem(TreeWidgetFileItem::Type) {
	this->record = record;
	QString text(record->filename);
	text.append(" - ");
	text.append(QString::number(record->num_images));
	
	setText(0, text);
}

TreeWidgetFileItem::~TreeWidgetFileItem() {
	if (record != NULL) {
		delete record;
	}
}

SgFileRecord* TreeWidgetFileItem::fileRecord() {
	return record;
}
