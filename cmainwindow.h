#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "csplashscreen.h"
#include "common.h"

#include <QMainWindow>
#include <QCloseEvent>


QT_BEGIN_NAMESPACE
namespace Ui { class cMainWindow; }
QT_END_NAMESPACE


class cMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	cMainWindow(cSplashScreen* lpSplashScreen, QWidget *parent = nullptr);
	~cMainWindow();

private:
	Ui::cMainWindow*				ui;
	cSplashScreen*					m_lpSplashScreen;

	QList<IMAGEFORMAT>				m_imageFormats;

	void							initUI();
	void							createActions();
	void							createFileActions();
	void							createContextActions();

	void							setImageFormats();
	void							addImageFormat(const QString& shortName, const QString& description, const QString& extension, QList<QByteArray>& readList, QList<QByteArray>& writeList);

protected:
	void							closeEvent(QCloseEvent* event);
};
#endif // CMAINWINDOW_H
