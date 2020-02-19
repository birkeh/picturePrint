/*!
 \file common.h
*/

#ifndef COMMON_H
#define COMMON_H


#include <QSize>
#include <QSizeF>
#include <QPoint>
#include <QPointF>

#include <QDebug>


#define THUMBNAIL_WIDTH		160
#define THUMBNAIL_HEIGHT	120


#ifdef __GNUC__
	#define myDebug qDebug() << __FILE__ << "(" << __LINE__ << ") - " << __PRETTY_FUNCTION__ << ":"
#elif __MINGW32__
	#define myDebug qDebug() << __FILE__ << "(" << __LINE__ << ") - " << __PRETTY_FUNCTION__ << ":"
#else
	#define myDebug qDebug() << __FILE__ << "(" << __LINE__ << ") - " << __FUNCTION__ << ":"
#endif


/*!
 \brief
 \class tagIMAGEFORMAT common.h "common.h"
*/
typedef struct tagIMAGEFORMAT
{
	QString	shortName; /*!< TODO: describe */
	QString	description; /*!< TODO: describe */
	QString	extension; /*!< TODO: describe */
	bool	read; /*!< TODO: describe */
	bool	write; /*!< TODO: describe */
/*!
 \brief
 \typedef IMAGEFORMAT*/
} IMAGEFORMAT;


/*!
 \brief
 \fn generateReadList
 \param imageFormats
 \return QStringList
*/
QStringList		generateReadList(const QList<IMAGEFORMAT>& imageFormats);
/*!
 \brief
 \fn generateWriteList
 \param imageFormats
 \return QStringList
*/
QStringList		generateWriteList(const QList<IMAGEFORMAT>& imageFormats);

/*!
 \brief
 \fn blob2Image
 \param ba
 \return QImage
*/
QImage			blob2Image(const QByteArray& ba);
/*!
 \brief
 \fn image2Blob
 \param image
 \return QByteArray
*/
QByteArray		image2Blob(const QImage& image);

/*!
 \brief
 \fn userDir
 \return QString
*/
QString			userDir();

int				scale(int v, qreal s);
qreal			scale(qreal v, qreal s);
QSize			scale(const QSize& size, qreal s);
QSize			scale(const QSizeF& size, qreal s);
QPoint			scale(const QPoint& point, qreal s);
QPoint			scale(const QPointF& point, qreal s);


#endif // COMMON_H
