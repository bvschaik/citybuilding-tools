#include "sgtreewidgetitem.h"

SgTreeWidgetItem::SgTreeWidgetItem(int number)
	: QTreeWidgetItem(QStringList(QString::number(number)), SgTreeWidgetItem::ImageType)
{ }

SgTreeWidgetItem::SgTreeWidgetItem(QString text)
	: QTreeWidgetItem(QStringList(text), SgTreeWidgetItem::FileType)
{ }
