#include "extractwizard.h"

#include "extractinputdirpage.h"
#include "extractfilelistpage.h"
#include "extractoutputdirpage.h"
#include "extractprogresspage.h"

#include <QtGui> // TODO: specify

ExtractWizard::ExtractWizard(QWidget *parent)
	: QDialog(parent)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	QHBoxLayout *buttons = new QHBoxLayout();
	
	stackWidget = new QStackedWidget();
	
	inputPage = new ExtractInputDirPage(this);
	outputPage = new ExtractOutputDirPage(this);
	filePage = new ExtractFileListPage(this);
	progressPage = new ExtractProgressPage(this);
	
	stackWidget->addWidget(inputPage);
	stackWidget->addWidget(filePage);
	stackWidget->addWidget(outputPage);
	stackWidget->addWidget(progressPage);
	
	// Buttons
	cancelButton = new QPushButton(tr("Cancel"), this);
	prevButton = new QPushButton(tr("Previous"), this);
	nextButton = new QPushButton(tr("Next"), this);
	
	// Build layout
	buttons->addWidget(cancelButton);
	buttons->addStretch();
	buttons->addWidget(prevButton);
	buttons->addWidget(nextButton);
	
	layout->addWidget(stackWidget);
	layout->addLayout(buttons);
	
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(prevButton, SIGNAL(clicked()), this, SLOT(prevClicked()));
	connect(nextButton, SIGNAL(clicked()), this, SLOT(nextClicked()));
	
	changePage(inputPage);
}

ExtractWizard::~ExtractWizard() {
	
}

void ExtractWizard::nextClicked() {
	QWidget *current = stackWidget->currentWidget();
	
	if (current == inputPage) {
		changePage(filePage);
	} else if (current == filePage) {
		changePage(outputPage);
	} else if (current == outputPage) {
		changePage(progressPage);
	}
}

void ExtractWizard::prevClicked() {
	QWidget *current = stackWidget->currentWidget();
	
	if (current == inputPage) {
		return;
	} else if (current == filePage) {
		changePage(inputPage);
	} else if (current == outputPage) {
		changePage(filePage);
	}
}

void ExtractWizard::changePage(QWidget *page) {
	if (page == inputPage) {
		prevButton->setEnabled(false);
	} else {
		prevButton->setEnabled(true);
	}
	
	if (page == progressPage) {
		// No way to exit this page except through cancel
		nextButton->setEnabled(false);
		prevButton->setEnabled(false);
	}
	
	stackWidget->setCurrentWidget(page);
}

// TODO: closeEvent while processing
