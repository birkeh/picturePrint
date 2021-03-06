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


cPrint::cPrint(QProgressBar* lpProgressBar, QListView* lpSelectedList, QStandardItemModel* lpSelectedListModel, cLayout* lpLayout, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::cPrint),
	m_lpProgressBar(lpProgressBar),
	m_lpSelectedList(lpSelectedList),
	m_lpSelectedListModel(lpSelectedListModel),
	m_lpLayout(lpLayout)
{
	ui->setupUi(this);

	ui->m_lpPaperSourceLabel->setVisible(false);
	ui->m_lpPaperSource->setVisible(false);

	m_printerStateText.insert(QPrinter::Idle,				tr("idle"));
	m_printerStateText.insert(QPrinter::Active,				tr("Active"));
	m_printerStateText.insert(QPrinter::Aborted,			tr("Aborted"));
	m_printerStateText.insert(QPrinter::Error,				tr("Error"));

	m_printerPaperSource.insert(QPrinter::Auto,				tr("Auto"));
	m_printerPaperSource.insert(QPrinter::Cassette,			tr("Cassette"));
	m_printerPaperSource.insert(QPrinter::Envelope,			tr("Envelope"));
	m_printerPaperSource.insert(QPrinter::EnvelopeManual,	tr("EnvelopeManual"));
	m_printerPaperSource.insert(QPrinter::FormSource,		tr("FormSource"));
	m_printerPaperSource.insert(QPrinter::LargeCapacity,	tr("LargeCapacity"));
	m_printerPaperSource.insert(QPrinter::LargeFormat,		tr("LargeFormat"));
	m_printerPaperSource.insert(QPrinter::Lower,			tr("Lower"));
	m_printerPaperSource.insert(QPrinter::MaxPageSource,	tr("MaxPageSource"));
	m_printerPaperSource.insert(QPrinter::Middle,			tr("Middle"));
	m_printerPaperSource.insert(QPrinter::Manual,			tr("Manual"));
	m_printerPaperSource.insert(QPrinter::OnlyOne,			tr("OnlyOne"));
	m_printerPaperSource.insert(QPrinter::Tractor,			tr("Tractor"));
	m_printerPaperSource.insert(QPrinter::SmallFormat,		tr("SmallFormat"));
	m_printerPaperSource.insert(QPrinter::Upper,			tr("Upper"));
	m_printerPaperSource.insert(QPrinter::CustomSource,		tr("CustomSource"));
	m_printerPaperSource.insert(QPrinter::LastPaperSource,	tr("LastPaperSource"));

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

	ui->m_lpZoomOut->setIcon(QIcon::fromTheme("zoom-out"));
	ui->m_lpZoomFit->setIcon(QIcon::fromTheme("zoom-fit"));
	ui->m_lpZoomOriginal->setIcon(QIcon::fromTheme("zoom-original"));
	ui->m_lpZoomIn->setIcon(QIcon::fromTheme("zoom-in"));

	ui->m_lpPrintPreview->setIcon(QIcon::fromTheme("document-print-preview"));
	ui->m_lpPrint->setIcon(QIcon::fromTheme("document-print"));

	connect(ui->m_lpPrinterSelect,		&QComboBox::currentTextChanged,			this,	&cPrint::onPrinterChanged);
	connect(ui->m_lpPrinterProperties,	&QPushButton::clicked,					this,	&cPrint::onPrinterSettings);
	connect(ui->m_lpPrintPreview,		&QPushButton::clicked,					this,	&cPrint::onPrintPreview);
	connect(ui->m_lpPrint,				&QPushButton::clicked,					this,	&cPrint::onPrint);

	connect(ui->m_lpZoomOut,			&QPushButton::clicked,					this,	&cPrint::onZoomOut);
	connect(ui->m_lpZoomFit,			&QPushButton::clicked,					this,	&cPrint::onZoomFit);
	connect(ui->m_lpZoomOriginal,		&QPushButton::clicked,					this,	&cPrint::onZoomOriginal);
	connect(ui->m_lpZoomIn,				&QPushButton::clicked,					this,	&cPrint::onZoomIn);

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

void cPrint::onLayoutChanged()
{
	ui->m_lpPrintPreview->setEnabled(true);
}

void cPrint::onCountChanged()
{
	ui->m_lpPrintPreview->setEnabled(true);
}

void cPrint::onPrinterChanged(const QString& printer)
{
	m_lpPrinter->setPrinterName(printer);

	ui->m_lpPaperSize->clear();
	ui->m_lpPaperSource->clear();

	QPrinterInfo					printerInfo(*m_lpPrinter);
	QList<QPageSize>				pageSizes		= printerInfo.supportedPageSizes();
	QList<QPrinter::PaperSource>	paperSources	= m_lpPrinter->supportedPaperSources();

	ui->m_lpPrinterStatus->setText(m_printerStateText[printerInfo.state()]);

	ui->m_lpPrinterMakeAndModel->setText(printerInfo.makeAndModel());

	for(int x = 0;x < pageSizes.count();x++)
		ui->m_lpPaperSize->addItem(pageSizes[x].name());
	ui->m_lpPaperSize->setCurrentText(QPageSize::name(static_cast<QPageSize::PageSizeId>(m_lpPrinter->pageSize())));

	ui->m_lpPrinterLocation->setText(printerInfo.location());

	for(int x = 0;x < paperSources.count();x++)
		ui->m_lpPaperSource->addItem(m_printerPaperSource[paperSources[x]]);
	ui->m_lpPaperSource->setCurrentText(m_printerPaperSource[m_lpPrinter->paperSource()]);

	ui->m_lpPrintPreview->setEnabled(true);
}

void cPrint::onPrinterSettings()
{
	QPrintDialog	printDialog(m_lpPrinter);
	printDialog.exec();

	ui->m_lpPrintPreview->setEnabled(true);
}

void cPrint::onPrintPreview()
{
//	if(!m_lpSelectedListModel->rowCount())
//		return;

//	cFileViewer*	lpFileViewer	= static_cast<cFileViewer*>(m_lpSelectedList->indexWidget(m_lpSelectedListModel->index(0, 0)));
//	if(!lpFileViewer)
//		return;

	m_lpPrintPreviewWidget->updatePreview();

	ui->m_lpPrintPreview->setEnabled(false);
}

void cPrint::onPaintRequested(QPrinter* printer)
{
	QPageSize		pageSize		= m_lpLayout->pageSize();
	QPageSize::Unit	unit			= m_lpLayout->unit();
	QMarginsF		pageMargins		= m_lpLayout->pageMargins();
	qreal			borderTop		= m_lpLayout->borderTop();
	qreal			borderLeft		= m_lpLayout->borderLeft();
	qreal			borderRight		= m_lpLayout->borderRight();
	qreal			borderBottom	= m_lpLayout->borderBottom();
	qreal			gutter			= m_lpLayout->gutter();
	int				tilesH			= m_lpLayout->tilesH();
	int				tilesV			= m_lpLayout->tilesV();

	printer->setPageSize(pageSize);
	printer->setPageMargins(pageMargins, static_cast<QPageLayout::Unit>(unit));

	QSizeF			printerSize		= printer->pageRect(QPrinter::DevicePixel).size();
	qreal			pageWidth		= printerSize.width();
	qreal			pageHeight		= printerSize.height();
	qreal			rectW			= (pageWidth-borderLeft-borderRight-gutter*(tilesH-1)) / tilesH;
	qreal			rectH			= (pageHeight-borderTop-borderBottom-gutter*(tilesV-1)) / tilesV;
	bool			portrait		= true;

	if(rectW > rectH)
		portrait	= false;

	printer->newPage();

	QPainter		painter(printer);
	int				listIndex		= 0;
	qint32			imageCount		= 0;
	cImage			image;

	for(;;)
	{
		for(int x = 0;x < tilesH;x++)
		{
			for(int y = 0;y < tilesV;y++)
			{
				if(!imageCount)
				{
					cFileViewer*	lpFileViewer	= static_cast<cFileViewer*>(m_lpSelectedList->indexWidget(m_lpSelectedListModel->index(listIndex, 0)));
					if(!lpFileViewer)
						return;

					imageCount	= lpFileViewer->count()-1;
					listIndex++;

					QString			fileName		= lpFileViewer->fileName();
					image.load(fileName);
				}
				else
					imageCount--;

				if(image.isNull())
					return;

				QPixmap			pixmap;

				if((image.width() < image.height() && !portrait) ||
						(image.width() > image.height() && portrait))
				{
					QPoint	center	= image.rect().center();
					QMatrix	matrix;
					matrix.translate(center.x(), center.y());
					matrix.rotate(90);
					pixmap	= QPixmap::fromImage(image.transformed(matrix));
				}
				else
					pixmap	= QPixmap::fromImage(image);

				QRect			rect			= QRect(0, 0, static_cast<int>(rectW), static_cast<int>(rectH));
				QSize			size			= pixmap.size();

				int	t	= static_cast<int>(borderTop+static_cast<qreal>(y)*(rectH+gutter));
				int	l	= static_cast<int>(borderLeft+static_cast<qreal>(x)*(rectW+gutter));

//				size.scale(rect.size(), Qt::KeepAspectRatio);
				size.scale(rect.size(), Qt::KeepAspectRatioByExpanding);

				if(size.width() > rect.width())
					size.setWidth(rect.width());
				if(size.height() > rect.height())
					size.setHeight(rect.height());

				painter.setViewport(l, t, size.width(), size.height());

				painter.setWindow(pixmap.rect());
				painter.drawPixmap(0, 0, pixmap);

				if(!imageCount && listIndex >= m_lpSelectedListModel->rowCount())
					return;
			}
		}
		printer->newPage();
	}
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

void cPrint::onZoomOut()
{
	m_lpPrintPreviewWidget->zoomOut();
}

void cPrint::onZoomFit()
{
	m_lpPrintPreviewWidget->fitInView();
}

void cPrint::onZoomOriginal()
{
	m_lpPrintPreviewWidget->setZoomFactor(1.0);
}

void cPrint::onZoomIn()
{
	m_lpPrintPreviewWidget->zoomIn();
}
