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
#include <QPrintPreviewWidget>
#include <QPageSetupDialog>
#include <QPainter>

#include <QDebug>


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

	m_lpPrinter->setPrinterName(defaultPrinter);
	onPrinterChanged(defaultPrinter);

	ui->m_lpPrinterSelect->addItems(availablePrinters);
	ui->m_lpPrinterSelect->setCurrentText(defaultPrinter);

	m_lpPrintPreviewWidget	= new QPrintPreviewWidget(m_lpPrinter, this);
	m_lpPrintPreviewWidget->setObjectName(QString::fromUtf8("m_lpPrintPreviewWidget"));
	ui->m_lpSplitter->insertWidget(0, m_lpPrintPreviewWidget);

	connect(ui->m_lpPrinterSelect,		&QComboBox::currentTextChanged,			this,	&cPrint::onPrinterChanged);
	connect(ui->m_lpPrinterSettings,	&QPushButton::clicked,					this,	&cPrint::onPrinterSettings);
	connect(ui->m_lpPrintPreview,		&QPushButton::clicked,					this,	&cPrint::onPrintPreview);
	connect(ui->m_lpPrint,				&QPushButton::clicked,					this,	&cPrint::onPrint);

	connect(m_lpPrintPreviewWidget,		&QPrintPreviewWidget::paintRequested,	this,	&cPrint::onPaintRequested);
}

cPrint::~cPrint()
{
	if(m_lpPrintPreviewWidget)
		delete m_lpPrintPreviewWidget;

	if(m_lpPrinter)
		delete m_lpPrinter;

	delete ui;
}

void cPrint::onPrinterChanged(const QString& printer)
{
	m_lpPrinter->setPrinterName(printer);

	QPrinterInfo	printerInfo(*m_lpPrinter);

	ui->m_lpPrinterMakeAndModel->setText(printerInfo.makeAndModel());
}

void cPrint::onPrinterSettings()
{
	QPrintDialog	printDialog(m_lpPrinter);
	printDialog.exec();

//	QPageSetupDialog	pageSetupDialog(m_lpPrinter);
//	pageSetupDialog.exec();
}

void cPrint::onPrintPreview()
{
	if(!m_lpSelectedListModel->rowCount())
		return;

	cFileViewer*	lpFileViewer	= static_cast<cFileViewer*>(m_lpSelectedList->indexWidget(m_lpSelectedListModel->index(0, 0)));
	if(!lpFileViewer)
		return;

	m_lpPrintPreviewWidget->updatePreview();
}

void cPrint::onPaintRequested(QPrinter* printer)
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

	printer->newPage();

	QPixmap			pixmap	= QPixmap::fromImage(image);
	QPainter		painter(printer);
	QRect			rect	= painter.viewport();
	QSize			size	= pixmap.size();

	size.scale(rect.size(), Qt::KeepAspectRatio);
	painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
	painter.setWindow(pixmap.rect());
	painter.drawPixmap(0, 0, pixmap);
}

void cPrint::onPrint()
{
//	if(!m_lpSelectedListModel->rowCount())
//		return;

//	cFileViewer*	lpFileViewer	= static_cast<cFileViewer*>(m_lpSelectedList->indexWidget(m_lpSelectedListModel->index(0, 0)));
//	if(!lpFileViewer)
//		return;

//	QString			fileName		= lpFileViewer->fileName();
//	cImage			image(fileName);

//	if(image.isNull())
//		return;

//	QPixmap			pixmap	= QPixmap::fromImage(image);
//	QPrintDialog	printDialog(m_lpPrinter, this);

//	if(printDialog.exec())
//	{
//		QPainter	painter(m_lpPrinter);
//		QRect		rect = painter.viewport();
//		QSize		size = pixmap.size();

//		size.scale(rect.size(), Qt::KeepAspectRatio);
//		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
//		painter.setWindow(pixmap.rect());
//		painter.drawPixmap(0, 0, pixmap);
//	}
}
