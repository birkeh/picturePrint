#ifndef CFILEVIEWER_H
#define CFILEVIEWER_H

#include <QWidget>

namespace Ui {
class cFileViewer;
}

class cFileViewer : public QWidget
{
	Q_OBJECT

public:
	explicit cFileViewer(QWidget *parent = nullptr);
	~cFileViewer();

private:
	Ui::cFileViewer *ui;
};

#endif // CFILEVIEWER_H
