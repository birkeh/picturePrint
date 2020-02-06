/*!
 \file cfileviewer.h

*/

#ifndef CFILEVIEWER_H
#define CFILEVIEWER_H


#include <QWidget>
#include <QList>
#include <QMetaType>


namespace Ui {
class cFileViewer;
}

/*!
 \brief

 \class cFileViewer cfileviewer.h "cfileviewer.h"
*/
class cFileViewer : public QWidget
{
	Q_OBJECT

public:
	/*!
	 \brief

	 \fn cFileViewer
	 \param parent
	*/
	explicit cFileViewer(QWidget *parent = nullptr);
	/*!
	 \brief

	 \fn ~cFileViewer
	*/
	~cFileViewer();

	/*!
	 \brief

	 \fn setImage
	 \param fileName
	 \param image
	*/
	void	setImage(const QString& fileName, const QPixmap& image);
	/*!
	 \brief

	 \fn image
	 \return QPixmap
	*/
	QPixmap	image();

	/*!
	 \brief

	 \fn fileName
	 \return QString
	*/
	QString	fileName();

	/*!
	 \brief

	 \fn setCount
	 \param count
	*/
	void	setCount(qint32 count);
	/*!
	 \brief

	 \fn count
	 \return qint32
	*/
	qint32	count();

private:
	Ui::cFileViewer*	ui; /*!< TODO: describe */
	QString				m_fileName; /*!< TODO: describe */

public slots:
	/*!
	 \brief

	 \fn onCountChanged
	 \param i
	*/
	void				onCountChanged(int i);

signals:
	/*!
	 \brief

	 \fn countChanged
	 \param fileName
	 \param pixmap
	 \param i
	*/
	void				countChanged(const QString& fileName, QPixmap pixmap, int i);
};

Q_DECLARE_METATYPE(cFileViewer*)

/*!
 \brief

 \class cFileViewerList cfileviewer.h "cfileviewer.h"
*/
class cFileViewerList : public QList<cFileViewer*>
{
public:
	/*!
	 \brief

	 \fn add
	 \param fileName
	 \param image
	 \return cFileViewer
	*/
	cFileViewer*	add(const QString& fileName, const QPixmap& image);
	/*!
	 \brief

	 \fn find
	 \param fileName
	 \return cFileViewer
	*/
	cFileViewer*	find(const QString& fileName);
};

#endif // CFILEVIEWER_H
