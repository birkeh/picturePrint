#ifndef COMMON_H
#define COMMON_H


#include <QDebug>


#ifdef __GNUC__
	#define myDebug qDebug() << __FILE__ << "(" << __LINE__ << ") - " << __PRETTY_FUNCTION__ << ":"
#elif __MINGW32__
	#define myDebug qDebug() << __FILE__ << "(" << __LINE__ << ") - " << __PRETTY_FUNCTION__ << ":"
#else
	#define myDebug qDebug() << __FILE__ << "(" << __LINE__ << ") - " << __FUNCTION__ << ":"
#endif


typedef struct tagIMAGEFORMAT
{
	QString	shortName;
	QString	description;
	QString	extension;
	bool	read;
	bool	write;
/*!
 \brief

 \typedef IMAGEFORMAT*/
} IMAGEFORMAT;


#endif // COMMON_H
