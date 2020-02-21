/*!
 \file cfilebrowser.cpp

*/

#include "cfilebrowser.h"

#include "ui_cfilebrowser.h"

#include <QMessageBox>
#include <QPushButton>

#include <QLabel>
#include <QVBoxLayout>

#include <QMimeType>

#include <QPainter>

#include <QSettings>
#include <QStandardPaths>

#include <QTimer>

#include <QSqlQuery>
#include <QSqlError>


cFileBrowser::cFileBrowser(QProgressBar* lpProgressBar, QList<IMAGEFORMAT>* lpImageFormats, QListView* lpSelectedList, QStandardItemModel* lpSelectedListModel, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::cFileBrowser),
	m_lpSelectedList(lpSelectedList),
	m_lpFileListModel(nullptr),
	m_lpSelectedListModel(lpSelectedListModel),
	m_lpProgressBar(lpProgressBar),
	m_lpImageFormats(lpImageFormats),
	m_working(false)
{
	ui->setupUi(this);

	ui->m_lpFileList->setGridSize(QSize(THUMBNAIL_WIDTH+2+4, THUMBNAIL_WIDTH+34+4));

	m_directoryListModel.setRootPath("");
	m_directoryListModel.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
	ui->m_lpDirectoryList->setModel(&m_directoryListModel);
	ui->m_lpDirectoryList->setAnimated(false);
	ui->m_lpDirectoryList->setIndentation(20);
	ui->m_lpDirectoryList->setColumnWidth(0, 250);

	m_lpFileListModel	= new QStandardItemModel(0, 0);
	ui->m_lpFileList->setModel(m_lpFileListModel);

	m_cacheDB	= QSqlDatabase::addDatabase("QSQLITE", "cacheDB");
	m_cacheDB.setHostName("localhost");
	m_cacheDB.setDatabaseName(userDir() + "cache.db");

	m_cacheDB.open();
	if(!m_cacheDB.open())
	{
		QDir	dir(userDir());
		dir.mkpath(userDir());

		if(!m_cacheDB.open())
			return;
	}

	QSqlQuery	query(m_cacheDB);

	if(!m_cacheDB.tables().contains("files"))
	{
		query.prepare("CREATE TABLE files "
					  "( "
					  "    fileName                 TEXT, "
					  "    fileSize                 BIGINT, "
					  "    fileDate                 DATETIME, "
					  "    cacheDate                DATETIME, "
					  "    thumbnail                BLOB "
					  ");");

		if(!query.exec())
		{
			m_cacheDB.close();
			return;
		}
	}

	QSettings	settings;
	QStringList	picturesPathList	= QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
	QString		picturesPath;

	if(picturesPathList.count())
		picturesPath	= picturesPathList[0];

	QString		filePath		= settings.value("lastPath", QVariant::fromValue(picturesPath)).toString();
	QModelIndex	index			= m_directoryListModel.index(filePath);
	ui->m_lpDirectoryList->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
	directorySelected(filePath);

	connect(ui->m_lpDirectoryList->selectionModel(),	&QItemSelectionModel::selectionChanged,	this,	&cFileBrowser::onDirectoryChanged);
}

cFileBrowser::~cFileBrowser()
{
	if(m_cacheDB.isOpen())
		m_cacheDB.close();

	delete ui;
}

void cFileBrowser::onDirectoryChanged(const QItemSelection& selected, const QItemSelection& /*deselected*/)
{
	directorySelected(m_directoryListModel.fileInfo(selected.indexes()[0]).filePath());
}

void cFileBrowser::onCountChanged(cFileViewer* fileViewer)
{
	cFileViewer*	lpFileViewer;

	for(int x = 0;x < m_lpSelectedListModel->rowCount();x++)
	{
		lpFileViewer	= static_cast<cFileViewer*>(m_lpSelectedList->indexWidget(m_lpSelectedListModel->index(x, 0)));
		if(lpFileViewer)
		{
			if(lpFileViewer->fileName() == fileViewer->fileName())
			{
				if(fileViewer->count())
				{
					disconnect(lpFileViewer,	&cFileViewer::countChanged,	this,	&cFileBrowser::onCountChangedSelected);
					lpFileViewer->setCount(fileViewer->count());
					connect(lpFileViewer,	&cFileViewer::countChanged,	this,	&cFileBrowser::onCountChangedSelected);
				}
				else
					m_lpSelectedListModel->removeRow(x);

				emit countChanged();
				return;
			}
		}
	}

	QStandardItem*	lpItem			= new QStandardItem("");
	cFileViewer*	lpFileViewer1	= new cFileViewer;
	lpFileViewer1->setImage(fileViewer->fileName(), fileViewer->image());
	lpFileViewer1->setCount(fileViewer->count());

	m_lpSelectedListModel->appendRow(lpItem);
	m_lpSelectedList->setIndexWidget(m_lpSelectedListModel->index(m_lpSelectedListModel->rowCount()-1, 0), lpFileViewer1);

	connect(lpFileViewer1,	&cFileViewer::countChanged,	this,	&cFileBrowser::onCountChangedSelected);

	emit countChanged();
}

void cFileBrowser::onCountChangedSelected(cFileViewer* fileViewer)
{
	cFileViewer*	lpFileViewer;

	for(int x = 0;x < m_lpFileListModel->rowCount();x++)
	{
		lpFileViewer	= static_cast<cFileViewer*>(ui->m_lpFileList->indexWidget(m_lpFileListModel->index(x, 0)));
		if(lpFileViewer)
		{
			if(lpFileViewer->fileName() == fileViewer->fileName())
			{
				disconnect(lpFileViewer,	&cFileViewer::countChanged,	this,	&cFileBrowser::onCountChangedSelected);
				lpFileViewer->setCount(fileViewer->count());
				connect(lpFileViewer,	&cFileViewer::countChanged,	this,	&cFileBrowser::onCountChangedSelected);

				emit countChanged();
				return;
			}
		}
	}

	emit countChanged();
}

void cFileBrowser::directorySelected(const QString& filePath)
{
	m_working	= true;

	m_lpProgressBar->setVisible(true);
	ui->m_lpDirectoryList->setEnabled(false);

	m_lpFileListModel->clear();

	QDir			dir(filePath);

	QFileInfoList	list	= dir.entryInfoList(generateReadList(*m_lpImageFormats), QDir::Files, QDir::Name);

	m_lpProgressBar->setRange(0, list.count());

	for(int x = 0;x < list.count();x++)
	{
		addFile(list[x]);
		m_lpProgressBar->setValue(x);
		qApp->processEvents();
	}

	ui->m_lpDirectoryList->setEnabled(true);
	m_lpProgressBar->setVisible(false);

	QSettings	settings;
	settings.setValue("lastPath", QVariant::fromValue(filePath));

	m_working	= false;
}

void cFileBrowser::addFile(const QFileInfo& fileInfo)
{
	cEXIF*		lpExif	= new cEXIF(&m_exifTAGList, &m_exifCompressionList, &m_exifLightSourceList, &m_exifFlashList, &m_iptcTagList, &m_xmpTagList);
	lpExif->setCacheDB(&m_cacheDB);

	if(!lpExif->fromFile(fileInfo.filePath(), true))
	{
		delete lpExif;
		return;
	}

	QImage	thumbnail	= lpExif->thumbnail();

	QImage		thumb(THUMBNAIL_WIDTH, thumbnail.height(), QImage::Format_ARGB32);
	thumb.fill(qRgba(0, 0, 0, 0));
	QPainter	painter(&thumb);
	painter.drawImage((THUMBNAIL_WIDTH-thumbnail.width())/2, 0, thumbnail);

	QStandardItem*	lpItem			= new QStandardItem("");
	cFileViewer*	lpFileViewerNew	= new cFileViewer;
	lpFileViewerNew->setImage(fileInfo.filePath(), QPixmap::fromImage(thumb));

	m_lpFileListModel->appendRow(lpItem);
	ui->m_lpFileList->setIndexWidget(m_lpFileListModel->index(m_lpFileListModel->rowCount()-1, 0), lpFileViewerNew);

	cFileViewer*	lpFileViewer;

	for(int x = 0;x < m_lpSelectedListModel->rowCount();x++)
	{
		lpFileViewer	= static_cast<cFileViewer*>(m_lpSelectedList->indexWidget(m_lpSelectedListModel->index(x, 0)));
		if(lpFileViewer)
		{
			if(lpFileViewer->fileName() == lpFileViewerNew->fileName())
			{
				lpFileViewerNew->setCount(lpFileViewer->count());
				break;
			}
		}
	}

	connect(lpFileViewerNew,	&cFileViewer::countChanged,	this,	&cFileBrowser::onCountChanged);

	delete lpExif;
}
