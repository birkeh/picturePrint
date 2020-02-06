/*!
 \file cmainwindow.h

*/

#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "csplashscreen.h"
#include "common.h"

#include "cfilebrowser.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QProgressBar>


QT_BEGIN_NAMESPACE
namespace Ui { class cMainWindow; }
QT_END_NAMESPACE


/*!
 \brief

 \class cMainWindow cmainwindow.h "cmainwindow.h"
*/
class cMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	cMainWindow(cSplashScreen* lpSplashScreen, QWidget *parent = nullptr);
	/*!
	 \brief

	 \fn ~cMainWindow
	*/
	~cMainWindow();

private:
	Ui::cMainWindow*				ui; /*!< TODO: describe */
	cSplashScreen*					m_lpSplashScreen; /*!< TODO: describe */
	QProgressBar*					m_lpProgressBar; /*!< TODO: describe */

	cFileBrowser*					m_lpFileBrowser; /*!< TODO: describe */

	QList<IMAGEFORMAT>				m_imageFormats; /*!< TODO: describe */

	/*!
	 \brief

	 \fn initUI
	*/
	void							initUI();
	/*!
	 \brief

	 \fn createActions
	*/
	void							createActions();
	/*!
	 \brief

	 \fn createFileActions
	*/
	void							createFileActions();
	/*!
	 \brief

	 \fn createContextActions
	*/
	void							createContextActions();

	/*!
	 \brief

	 \fn setImageFormats
	*/
	void							setImageFormats();
	/*!
	 \brief

	 \fn addImageFormat
	 \param shortName
	 \param description
	 \param extension
	 \param readList
	 \param writeList
	*/
	void							addImageFormat(const QString& shortName, const QString& description, const QString& extension, QList<QByteArray>& readList, QList<QByteArray>& writeList);

protected:
	/*!
	 \brief

	 \fn closeEvent
	 \param event
	*/
	void							closeEvent(QCloseEvent* event);
};
#endif // CMAINWINDOW_H
