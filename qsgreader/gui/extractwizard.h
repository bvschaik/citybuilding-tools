#ifndef EXTRACTWIZARD_H
#define EXTRACTWIZARD_H

#include <QDialog>
#include <QString>
#include <QStringList>

class QStackedWidget;
class QPushButton;

class ExtractInputDirPage;
class ExtractOutputDirPage;
class ExtractFileListPage;
class ExtractProgressPage;

class ExtractWizard : public QDialog {
	Q_OBJECT
	
	public:
		ExtractWizard(QWidget *parent = 0);
		~ExtractWizard();
	
	private slots:
		void prevClicked();
		void nextClicked();
		void changePage(QWidget *page);
	
	private:
		ExtractInputDirPage *inputPage;
		ExtractFileListPage *filePage;
		ExtractOutputDirPage *outputPage;
		ExtractProgressPage *progressPage;
		
		QStackedWidget *stackWidget;
		QPushButton *nextButton;
		QPushButton *prevButton;
		QPushButton *cancelButton;
		
		QString inputFolder;
		QString outputFolder;
		QStringList files;
};

#endif /* EXTRACTWIZARD_H */
