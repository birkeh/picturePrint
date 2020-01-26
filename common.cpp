#include "common.h"

#include <QImage>
#include <QBuffer>
#include <QDir>


QStringList generateReadList(const QList<IMAGEFORMAT>& imageFormats)
{
	QStringList	readList;

	for(int z = 0;z < imageFormats.count();z++)
	{
		IMAGEFORMAT	i	= imageFormats[z];

		if(i.read)
			readList.append(i.extension.split(" "));
	}

	return(readList);
}

QStringList generateWriteList(const QList<IMAGEFORMAT>& imageFormats)
{
	QStringList	writeList;

	for(int z = 0;z < imageFormats.count();z++)
	{
		IMAGEFORMAT	i	= imageFormats[z];

		if(i.write)
			writeList.append(i.extension.split(""));
	}

	return(writeList);
}

QImage blob2Image(const QByteArray& ba)
{
	QImage		image;

	if(!ba.isEmpty())
	{
		if(!image.loadFromData(ba))
			myDebug << "image load error.";
	}

	return(image);
}

QByteArray image2Blob(const QImage &image)
{
	QByteArray	ba;
	QBuffer		buffer(&ba);
	buffer.open(QIODevice::WriteOnly);
	image.save(&buffer, "JPG");
	buffer.close();

	return(ba);
}

QString userDir()
{
	return(QDir::homePath() + QDir::separator() + ".picturePrint" + QDir::separator());
}
