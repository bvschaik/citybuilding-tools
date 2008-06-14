#ifndef SGFILE_H
#define SGFILE_H

#include <QString>
#include <QList>
#include <QImage>

class SgBitmap;
class SgImage;
class SgHeader; // = header

class SgFile {
	public:
		SgFile(const QString &filename);
		~SgFile();
		bool load();
		int bitmapCount() const;
		int totalImageCount() const;
		int imageCount(int bitmapId) const;
		
		QString getBitmapDescription(int bitmapId) const;
		
		SgImage *image(int bitmapId, int imageId) const;
		SgImage *image(int globalImageId) const;
		
		QImage getImage(int bitmapId, int imageId);
		QImage getImage(int globalImageId);
		
	private:
		bool checkVersion();
		int maxBitmapRecords() const;
		void loadBitmaps(QDataStream *stream);
		void loadImages(QDataStream *stream, bool includeAlpha);
		
		QList<SgBitmap*> bitmaps;
		QList<SgImage*> images;
		QString filename;
		SgHeader *header;
};

#endif /* SGFILE_H */
