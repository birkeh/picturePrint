#include "cfileviewer.h"
#include "ui_cfileviewer.h"

cFileViewer::cFileViewer(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::cFileViewer)
{
	ui->setupUi(this);
}

cFileViewer::~cFileViewer()
{
	delete ui;
}
