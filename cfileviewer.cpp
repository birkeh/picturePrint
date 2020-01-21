#include "common.h"

#include "cfileviewer.h"
#include "ui_cfileviewer.h"

#include <QFileInfo>


cFileViewer::cFileViewer(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::cFileViewer)
{
	ui->setupUi(this);

	ui->m_lpImage->setMinimumSize(THUMBNAIL_WIDTH, THUMBNAIL_WIDTH);
	ui->m_lpImage->setMaximumSize(THUMBNAIL_WIDTH, THUMBNAIL_WIDTH);
	ui->m_lpImage->resize(THUMBNAIL_WIDTH, THUMBNAIL_WIDTH);

	setMinimumSize(THUMBNAIL_WIDTH+2, THUMBNAIL_WIDTH+34);
	setMaximumSize(THUMBNAIL_WIDTH+2, THUMBNAIL_WIDTH+34);
	resize(THUMBNAIL_WIDTH+2, THUMBNAIL_WIDTH+34);
}

cFileViewer::~cFileViewer()
{
	delete ui;
}

void cFileViewer::setImage(const QString& fileName, const QPixmap& image)
{
	QFileInfo	fileInfo(fileName);

	m_fileName	= fileName;
	ui->m_lpImage->setPixmap(image);
	ui->m_lpFileName->setText(fileInfo.fileName());
}
