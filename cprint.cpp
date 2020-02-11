/*!
 \file cprint.cpp

*/

#include "cprint.h"
#include "ui_cprint.h"

#include "cfileviewer.h"

#include "cimage.h"

#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintDialog>
#include <QPageSetupDialog>
#include <QPainter>


cPrint::cPrint(QProgressBar* lpProgressBar, QListView* lpSelectedList, QStandardItemModel* lpSelectedListModel, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::cPrint),
	m_lpProgressBar(lpProgressBar),
	m_lpSelectedList(lpSelectedList),
	m_lpSelectedListModel(lpSelectedListModel)
{
	ui->setupUi(this);

	m_lpPrinter	= new QPrinter;

	QString		defaultPrinter		= QPrinterInfo::defaultPrinterName();
	QStringList	availablePrinters	= QPrinterInfo::availablePrinterNames();

	ui->m_lpPrinterSelect->addItems(availablePrinters);
	ui->m_lpPrinterSelect->setCurrentText(defaultPrinter);

	connect(ui->m_lpPrinterSettings,	&QPushButton::clicked,	this,	&cPrint::onPrinterSettings);
	connect(ui->m_lpPrint,				&QPushButton::clicked,	this,	&cPrint::onPrint);
}

cPrint::~cPrint()
{
	if(m_lpPrinter)
		delete m_lpPrinter;

	delete ui;
}

void cPrint::onPrinterSettings()
{
	QPageSetupDialog	pageSetupDialog(m_lpPrinter);
	pageSetupDialog.exec();
}

void cPrint::onPrint()
{
	if(!m_lpSelectedListModel->rowCount())
		return;

	cFileViewer*	lpFileViewer	= static_cast<cFileViewer*>(m_lpSelectedList->indexWidget(m_lpSelectedListModel->index(0, 0)));
	if(!lpFileViewer)
		return;

	QString			fileName		= lpFileViewer->fileName();
	cImage			image(fileName);

	if(image.isNull())
		return;

	QPixmap			pixmap	= QPixmap::fromImage(image);
	QPrintDialog	printDialog(m_lpPrinter, this);

	if(printDialog.exec())
	{
		QPainter	painter(m_lpPrinter);
		QRect		rect = painter.viewport();
		QSize		size = pixmap.size();

		size.scale(rect.size(), Qt::KeepAspectRatio);
		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
		painter.setWindow(pixmap.rect());
		painter.drawPixmap(0, 0, pixmap);
	}
}
