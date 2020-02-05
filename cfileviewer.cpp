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

	connect(ui->m_lpCount,	QOverload<int>::of(&QSpinBox::valueChanged),	this,	&cFileViewer::onCountChanged);
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

QPixmap cFileViewer::image()
{
	return(QPixmap(*ui->m_lpImage->pixmap()));
}

QString cFileViewer::fileName()
{
	return(m_fileName);
}

void cFileViewer::setCount(qint32 count)
{
	ui->m_lpCount->setValue(count);
}

qint32 cFileViewer::count()
{
	return(ui->m_lpCount->value());
}

void cFileViewer::onCountChanged(int i)
{
	emit countChanged(m_fileName, image(), i);
}

cFileViewer* cFileViewerList::add(const QString& fileName, const QPixmap& image)
{
	cFileViewer*	lpNew	= find(fileName);
	if(lpNew)
		return(nullptr);

	lpNew	= new cFileViewer;
	lpNew->setImage(fileName, image);
	append(lpNew);

	return(lpNew);
}

cFileViewer* cFileViewerList::find(const QString& fileName)
{
	for(cFileViewerList::iterator i = begin();i != end();i++)
	{
		if((*i)->fileName() == fileName)
			return(*i);
	}
	return(nullptr);
}
