#ifndef CFILEBROWSER_H
#define CFILEBROWSER_H

#include <QWidget>

namespace Ui {
class cFileBrowser;
}

class cFileBrowser : public QWidget
{
	Q_OBJECT

public:
	explicit cFileBrowser(QWidget *parent = nullptr);
	~cFileBrowser();

private:
	Ui::cFileBrowser *ui;
};

#endif // CFILEBROWSER_H
