#include "clayout.h"
#include "ui_clayout.h"

#include "common.h"

#include <QPageSize>
#include <QPixmap>
#include <QPainter>

#include <QDebug>


cLayout::cLayout(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::cLayout),
	m_initializing(true)
{
	ui->setupUi(this);

	for(int id = 0;id < QPageSize::LastPageSize;id++)
		ui->m_lpPageSize->addItem(QPageSize::name(static_cast<QPageSize::PageSizeId>(id)), QVariant::fromValue(static_cast<QPageSize::PageSizeId>(id)));

	ui->m_lpUnit->addItem("Millimeter",	QVariant::fromValue(QPageSize::Millimeter));
	ui->m_lpUnit->addItem("Point",		QVariant::fromValue(QPageSize::Point));
	ui->m_lpUnit->addItem("Inch",		QVariant::fromValue(QPageSize::Inch));
	ui->m_lpUnit->addItem("Pica",		QVariant::fromValue(QPageSize::Pica));
	ui->m_lpUnit->addItem("Didot",		QVariant::fromValue(QPageSize::Didot));
	ui->m_lpUnit->addItem("Cicero",		QVariant::fromValue(QPageSize::Cicero));

	connect(ui->m_lpPageSize,			QOverload<int>::of(&QComboBox::currentIndexChanged),	this,	&cLayout::onPageSizeChanged);
	connect(ui->m_lpUnit,				QOverload<int>::of(&QComboBox::currentIndexChanged),	this,	&cLayout::onUnitChanged);
	connect(ui->m_lpBorderAllTheSame,	&QCheckBox::stateChanged,								this,	&cLayout::onAllTheSameChanged);
	connect(ui->m_lpTilesH,				QOverload<int>::of(&QSpinBox::valueChanged),			this,	&cLayout::onTileHChanged);
	connect(ui->m_lpTilesV,				QOverload<int>::of(&QSpinBox::valueChanged),			this,	&cLayout::onTileVChanged);
	connect(ui->m_lpBorderTop,			QOverload<double>::of(&QDoubleSpinBox::valueChanged),	this,	&cLayout::onBorderTopChanged);
	connect(ui->m_lpBorderLeft,			QOverload<double>::of(&QDoubleSpinBox::valueChanged),	this,	&cLayout::onBorderLeftChanged);
	connect(ui->m_lpBorderRight,		QOverload<double>::of(&QDoubleSpinBox::valueChanged),	this,	&cLayout::onBorderRightChanged);
	connect(ui->m_lpBorderBottom,		QOverload<double>::of(&QDoubleSpinBox::valueChanged),	this,	&cLayout::onBorderBottomChanged);
	connect(ui->m_lpGutterWidth,		QOverload<double>::of(&QDoubleSpinBox::valueChanged),	this,	&cLayout::onGutterChanged);

	m_initializing	= false;
}

cLayout::~cLayout()
{
	delete ui;
}

void cLayout::onPageSizeChanged(int /*index*/)
{
	redrawPreview();
}

void cLayout::onUnitChanged(int /*index*/)
{
	redrawPreview();
}

void cLayout::onAllTheSameChanged(int /*state*/)
{
	if(ui->m_lpBorderAllTheSame->isChecked())
	{
		disconnect(ui->m_lpBorderLeft,		QOverload<double>::of(&QDoubleSpinBox::valueChanged),	this,	&cLayout::onBorderLeftChanged);
		disconnect(ui->m_lpBorderRight,		QOverload<double>::of(&QDoubleSpinBox::valueChanged),	this,	&cLayout::onBorderRightChanged);
		disconnect(ui->m_lpBorderBottom,	QOverload<double>::of(&QDoubleSpinBox::valueChanged),	this,	&cLayout::onBorderBottomChanged);

		ui->m_lpBorderLeft->setValue(ui->m_lpBorderTop->value());
		ui->m_lpBorderRight->setValue(ui->m_lpBorderTop->value());
		ui->m_lpBorderBottom->setValue(ui->m_lpBorderTop->value());

		ui->m_lpBorderLeft->setEnabled(false);
		ui->m_lpBorderRight->setEnabled(false);
		ui->m_lpBorderBottom->setEnabled(false);

		redrawPreview();
	}
	else
	{
		ui->m_lpBorderLeft->setEnabled(true);
		ui->m_lpBorderRight->setEnabled(true);
		ui->m_lpBorderBottom->setEnabled(true);

		connect(ui->m_lpBorderLeft,		QOverload<double>::of(&QDoubleSpinBox::valueChanged),	this,	&cLayout::onBorderLeftChanged);
		connect(ui->m_lpBorderRight,	QOverload<double>::of(&QDoubleSpinBox::valueChanged),	this,	&cLayout::onBorderRightChanged);
		connect(ui->m_lpBorderBottom,	QOverload<double>::of(&QDoubleSpinBox::valueChanged),	this,	&cLayout::onBorderBottomChanged);
	}
}

void cLayout::onTileHChanged(int /*value*/)
{
	redrawPreview();
}

void cLayout::onTileVChanged(int /*value*/)
{
	redrawPreview();
}

void cLayout::onBorderTopChanged(double /*value*/)
{
	if(ui->m_lpBorderAllTheSame->isChecked())
	{
		ui->m_lpBorderLeft->setValue(ui->m_lpBorderTop->value());
		ui->m_lpBorderRight->setValue(ui->m_lpBorderTop->value());
		ui->m_lpBorderBottom->setValue(ui->m_lpBorderTop->value());
	}

	redrawPreview();
}

void cLayout::onBorderLeftChanged(double /*value*/)
{
	redrawPreview();
}

void cLayout::onBorderRightChanged(double /*value*/)
{
	redrawPreview();
}

void cLayout::onBorderBottomChanged(double /*value*/)
{
	redrawPreview();
}

void cLayout::onGutterChanged(double /*value*/)
{
	redrawPreview();
}

void cLayout::resizeEvent(QResizeEvent *event)
{
	if(!m_initializing)
		redrawPreview();

	QWidget::resizeEvent(event);
}

void cLayout::redrawPreview()
{
	QSize		widgetSize		= ui->m_lpLayoutPreview->size();
	QPageSize	pageSize(ui->m_lpPageSize->currentData().value<QPageSize::PageSizeId>());
	QSizeF		size			= pageSize.size(ui->m_lpUnit->currentData().value<QPageSize::Unit>());
	qreal		pageWidth		= size.width();
	qreal		pageHeight		= size.height();
	qreal		borderTop		= ui->m_lpBorderTop->value();
	qreal		borderLeft		= ui->m_lpBorderLeft->value();
	qreal		borderRight		= ui->m_lpBorderRight->value();
	qreal		borderBottom	= ui->m_lpBorderBottom->value();
	qreal		gutter			= ui->m_lpGutterWidth->value();
	qreal		tilesH			= static_cast<qreal>(ui->m_lpTilesH->value());
	qreal		tilesV			= static_cast<qreal>(ui->m_lpTilesV->value());
	qreal		scaleFactor		= static_cast<qreal>(widgetSize.width()-10) / pageWidth;
	qreal		tmp				= static_cast<qreal>(widgetSize.height()-10) / pageHeight;

	QString		unit;

	switch(ui->m_lpUnit->currentData().value<QPageSize::Unit>())
	{
	case QPageSize::Millimeter:
		unit	= tr("Millimeter");
		break;
	case QPageSize::Point:
		unit	= tr("Point(s)");
		break;
	case QPageSize::Inch:
		unit	= tr("Inch");
		break;
	case QPageSize::Pica:
		unit	= tr("Pica");
		break;
	case QPageSize::Didot:
		unit	= tr("Didot");
		break;
	case QPageSize::Cicero:
		unit	= tr("Cicero");
		break;
	}

	ui->m_lpPaperSizeText->setText(QString::number(pageWidth) + " " + unit + " x " + QString::number(pageHeight) + " " + unit);

	ui->m_lpBorderTop->setSuffix(QString(" ") + unit);
	ui->m_lpBorderLeft->setSuffix(QString(" ") + unit);
	ui->m_lpBorderRight->setSuffix(QString(" ") + unit);
	ui->m_lpBorderBottom->setSuffix(QString(" ") + unit);
	ui->m_lpGutterWidth->setSuffix(QString(" ") + unit);

	if(tmp < scaleFactor)
		scaleFactor	= tmp;

	QPixmap		pixmap(scale(size, scaleFactor));
	pixmap.fill(Qt::white);
	QPainter painter(&pixmap);

	QRectF		rect;
	qreal		rectW	= (pageWidth-borderLeft-borderRight-gutter*(tilesH-1)) / tilesH;
	qreal		rectH	= (pageHeight-borderTop-borderBottom-gutter*(tilesV-1)) / tilesV;

//	borderTop			= scale(borderTop, scaleFactor);
//	borderLeft			= scale(borderLeft, scaleFactor);

	painter.setPen(Qt::black);

	for(qreal x = 0;x < tilesH;x++)
	{
		for(qreal y = 0;y < tilesV;y++)
		{
			qreal	t	= borderTop+y*(rectH+gutter);
			qreal	l	= borderLeft+x*(rectW+gutter);

			rect.setTopLeft(scale(QPointF(l, t), scaleFactor));
			rect.setSize(scale(QSizeF(rectW, rectH), scaleFactor));
//			rect.setTopLeft(QPoint(borderLeft+x*(rectW+gutter), borderTop+y*(rectH+gutter)));
//			rect.setSize(QSizeF(rectW, rectH));
			painter.drawRect(rect);
		}
	}

	ui->m_lpLayoutPreview->setPixmap(pixmap);
}
