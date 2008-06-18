#ifndef EXTRACTINPUTDIRPAGE_H
#define EXTRACTINPUTDIRPAGE_H

#include <QWidget>

class ExtractInputDirPage : public QWidget {
	Q_OBJECT
	
	public:
		ExtractInputDirPage(QWidget *parent);
		QString inputDir() const;
	
	signals:
		void validChanged(bool isValid);
	
	private:
		QString inputDirectory;
};

#endif /* EXTRACTINPUTDIRPAGE_H */
