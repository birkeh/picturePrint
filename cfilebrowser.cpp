#include "cfilebrowser.h"
#include "ui_cfilebrowser.h"

cFileBrowser::cFileBrowser(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::cFileBrowser)
{
	ui->setupUi(this);
}

cFileBrowser::~cFileBrowser()
{
	delete ui;
}
