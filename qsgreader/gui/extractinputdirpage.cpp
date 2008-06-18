#include "extractinputdirpage.h"

#include <QtGui> // TODO: specify

ExtractInputDirPage::ExtractInputDirPage(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	QLabel *intro = new QLabel(tr("Choose a folder which contains .sg2 or .sg3 "
		"files, or choose from an installed game"));
	
	layout->addWidget(intro);
}

QString ExtractInputDirPage::inputDir() const {
	return inputDirectory;
}
