/*!
 \file cmainwindow.cpp

*/

#include "common.h"

#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include <QSettings>

#include <QImageReader>
#include <QImageWriter>

#include <QSqlDatabase>
#include <QSqlQuery>


cMainWindow::cMainWindow(cSplashScreen* lpSplashScreen, QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::cMainWindow),
	m_lpSplashScreen(lpSplashScreen),
	m_lpFileBrowser(nullptr),
	m_lpPrint(nullptr)
{
	setImageFormats();

	initUI();
	createActions();

	ui->m_lpMainTab->setCurrentIndex(1);
}

cMainWindow::~cMainWindow()
{
	delete ui;
}

void cMainWindow::showEvent(QShowEvent *event)
{
	QMainWindow::showEvent(event);
}

void cMainWindow::closeEvent(QCloseEvent *event)
{
//	if(m_working)
//	{
//		QMessageBox::information(this, tr("Close"), tr("Can't close, pictureConvert is working."));
//		event->ignore();
//		return;
//	}

	QSettings	settings;
	settings.setValue("main/width", QVariant::fromValue(size().width()));
	settings.setValue("main/height", QVariant::fromValue(size().height()));
	settings.setValue("main/x", QVariant::fromValue(x()));
	settings.setValue("main/y", QVariant::fromValue(y()));
	if(this->isMaximized())
		settings.setValue("main/maximized", QVariant::fromValue(true));
	else
		settings.setValue("main/maximized", QVariant::fromValue(false));

//	settings.setValue("main/thumbnailSize", QVariant::fromValue(ui->m_lpThumbnailSize->value()));

	event->accept();
}

void cMainWindow::initUI()
{
	QSettings	settings;

	ui->setupUi(this);

	QIcon::setThemeName("arc");

	m_lpProgressBar			= new QProgressBar(this);
	m_lpProgressBar->setVisible(false);
	ui->m_lpStatusBar->addPermanentWidget(m_lpProgressBar);

	m_lpSelectedListModel	= new QStandardItemModel(0, 0);
	ui->m_lpSelectedList->setModel(m_lpSelectedListModel);

	m_lpFileBrowser	= new cFileBrowser(m_lpProgressBar, &m_imageFormats, ui->m_lpSelectedList, m_lpSelectedListModel, this);
	ui->m_lpMainTab->addTab(m_lpFileBrowser, "Files");

	m_lpPrint		= new cPrint(m_lpProgressBar, ui->m_lpSelectedList, m_lpSelectedListModel, this);
	ui->m_lpMainTab->addTab(m_lpPrint, "Print");

	if(!settings.value("main/maximized").toBool())
	{
		qint32		iX		= settings.value("main/x", QVariant::fromValue(-1)).toInt();
		qint32		iY		= settings.value("main/y", QVariant::fromValue(-1)).toInt();
		qint32		iWidth	= settings.value("main/width", QVariant::fromValue(-1)).toInt();
		qint32		iHeight	= settings.value("main/height", QVariant::fromValue(-1)).toInt();

		if(iWidth != -1 && iHeight != -1)
			resize(iWidth, iHeight);
		if(iX != -1 && iY != -1)
			move(iX, iY);
	}

//	QStringList			headerLabels	= QStringList() << tr("icon") << tr("path") << tr("file") << tr("size") << tr("date") << tr("width") << tr("height") << ("");
//	m_lpFileListModel->setHorizontalHeaderLabels(headerLabels);
}

void cMainWindow::createActions()
{
//	setToolButtonStyle(Qt::ToolButtonFollowStyle);

//	createFileActions();
//	createContextActions();

//	connect(ui->m_lpFileList,		&cTreeView::deleteEntrys,	this,		&cMainWindow::onDeleteEntrys);
//	connect(ui->m_lpThumbnailSize,	&QSlider::valueChanged,		this,		&cMainWindow::onThumbnailSize);
}

void cMainWindow::createContextActions()
{
}

void cMainWindow::createFileActions()
{
//	m_lpFileToolBar	= addToolBar("file");

//	const QIcon	openIcon			= QIcon::fromTheme("document-open");
//	m_lpOpenFileAction				= m_lpFileToolBar->addAction(openIcon, tr("&Open..."), this, &cMainWindow::onAddFile);
//	m_lpOpenFileAction->setShortcut(QKeySequence::Open);

//	const QIcon	openDirectoryIcon	= QIcon::fromTheme("folder");
//	m_lpOpenDirectoryAction			= m_lpFileToolBar->addAction(openIcon, tr("&Open Folder..."), this, &cMainWindow::onAddFolder);


//	m_lpListToolBar	= addToolBar("list");

//	const QIcon	deleteIcon			= QIcon::fromTheme("edit-delete");
//	m_lpDeleteAction				= m_lpListToolBar->addAction(deleteIcon, tr("&Delete"), this, &cMainWindow::onDeleteEntrys);
//	m_lpDeleteAction->setShortcut(QKeySequence::Delete);

//	const QIcon	clearIcon			= QIcon::fromTheme("edit-clear");
//	m_lpClearAction					= m_lpListToolBar->addAction(clearIcon, tr("&Clear"), this, &cMainWindow::onClearList);


//	m_lpActionToolBar	= addToolBar("action");

//	const QIcon	convertIcon			= QIcon::fromTheme("system-run");
//	m_lpConvertAction				= m_lpActionToolBar->addAction(convertIcon, tr("&Convert"), this, &cMainWindow::onConvert);
//	m_lpConvertAction->setShortcut(QKeySequence::Delete);

//	const QIcon	stopIcon			= QIcon::fromTheme("process-stop");
//	m_lpStopAction					= m_lpActionToolBar->addAction(stopIcon, tr("&Stop"), this, &cMainWindow::onStop);
}

void cMainWindow::setImageFormats()
{
	QList<QByteArray>	readList	= QImageReader::supportedImageFormats();
	QList<QByteArray>	writeList	= QImageWriter::supportedImageFormats();

	QSqlDatabase	db	= QSqlDatabase::addDatabase("QSQLITE", "picturePrint");
	db.setHostName("localhost");
	db.setDatabaseName("picturePrint.db");

	if(!db.open())
		return;

	QSqlQuery	query(db);

	query.prepare("SELECT shortname, description, extension FROM imageFormat;");
	if(!query.exec())
	{
		db.close();
		return;
	}

	while(query.next())
		addImageFormat(query.value("shortname").toString(), query.value("description").toString(), query.value("extension").toString(), readList, writeList);

	db.close();
}

void cMainWindow::addImageFormat(const QString& shortName, const QString& description, const QString& extension, QList<QByteArray>& readList, QList<QByteArray>& writeList)
{
	bool	r	= readList.contains(QByteArray(shortName.toUtf8()));
	bool	w	= writeList.contains(QByteArray(shortName.toUtf8()));

	if(QString(shortName).isEmpty())
		r	= true;

	IMAGEFORMAT	i;
	i.shortName		= shortName;
	i.description	= description;
	i.extension		= extension;
	i.read			= r;
	i.write			= w;
	m_imageFormats.append(i);
}
