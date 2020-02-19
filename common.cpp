/*!
 \file common.cpp
*/

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

int scale(int v, qreal s)
{
	return(static_cast<int>(static_cast<qreal>(v)*s));
}

qreal scale(qreal v, qreal s)
{
	return(v*s);
}

QSize scale(const QSize& size, qreal s)
{
	return(QSize(scale(size.width(), s), scale(size.height(), s)));
}

QSize scale(const QSizeF& size, qreal s)
{
	return(QSize(scale(size.width(), s), scale(size.height(), s)));
}

QPoint scale(const QPoint& point, qreal s)
{
	return(QPoint(scale(point.x(), s), scale(point.y(), s)));
}

QPoint scale(const QPointF& point, qreal s)
{
	return(QPoint(scale(point.x(), s), scale(point.y(), s)));
}
