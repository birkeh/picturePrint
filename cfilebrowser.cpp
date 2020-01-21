#include "cfilebrowser.h"
#include "cfileviewer.h"

#include "ui_cfilebrowser.h"

#include <QMessageBox>
#include <QPushButton>

#include <QLabel>
#include <QVBoxLayout>

#include <QMimeType>

#include <QPainter>


cFileBrowser::cFileBrowser(QProgressBar* lpProgressBar, QList<IMAGEFORMAT>* lpImageFormats, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::cFileBrowser),
	m_lpFileListModel(nullptr),
	m_lpProgressBar(lpProgressBar),
	m_lpImageFormats(lpImageFormats)
{
	ui->setupUi(this);

	ui->m_lpFileList->setGridSize(QSize(THUMBNAIL_WIDTH+2+4, THUMBNAIL_WIDTH+34+4));

//	m_directoryListModel.setRootPath("");
	m_directoryListModel.setRootPath("C:/Users/birkeh/Pictures");
	m_directoryListModel.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
	ui->m_lpDirectoryList->setModel(&m_directoryListModel);
	ui->m_lpDirectoryList->setAnimated(false);
	ui->m_lpDirectoryList->setIndentation(20);
	ui->m_lpDirectoryList->setColumnWidth(0, 250);

	m_lpFileListModel	= new QStandardItemModel(0, 0);
	ui->m_lpFileList->setModel(m_lpFileListModel);

	connect(ui->m_lpDirectoryList->selectionModel(),	&QItemSelectionModel::selectionChanged,	this,	&cFileBrowser::onDirectoryChanged);
}

cFileBrowser::~cFileBrowser()
{
	delete ui;
}

void cFileBrowser::onDirectoryChanged(const QItemSelection& selected, const QItemSelection& /*deselected*/)
{
	m_lpProgressBar->setVisible(true);

	m_lpFileListModel->clear();

	QFileInfo		info	= m_directoryListModel.fileInfo(selected.indexes()[0]);
	QDir			dir(info.filePath());

	QFileInfoList	list	= dir.entryInfoList(generateReadList(*m_lpImageFormats), QDir::Files, QDir::Name);

	m_lpProgressBar->setRange(0, list.count());

	for(int x = 0;x < list.count();x++)
	{
		addFile(list[x]);
		m_lpProgressBar->setValue(x);
//		qApp->sendPostedEvents();
		qApp->processEvents();
	}

	m_lpProgressBar->setVisible(false);
}

void cFileBrowser::addFile(const QFileInfo& fileInfo)
{
	cEXIF*		lpExif	= new cEXIF(&m_exifTAGList, &m_exifCompressionList, &m_exifLightSourceList, &m_exifFlashList, &m_iptcTagList, &m_xmpTagList);

	if(!lpExif->fromFile(fileInfo.filePath()))
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
	cFileViewer*	lpFileViewer	= new cFileViewer;
	lpFileViewer->setImage(fileInfo.filePath(), QPixmap::fromImage(thumb));

	m_lpFileListModel->appendRow(lpItem);
	ui->m_lpFileList->setIndexWidget(m_lpFileListModel->index(m_lpFileListModel->rowCount()-1, 0), lpFileViewer);
}
