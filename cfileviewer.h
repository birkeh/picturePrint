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

	void	setImage(const QString& fileName, const QPixmap& image);

private:
	Ui::cFileViewer*	ui;
	QString				m_fileName;
};

#endif // CFILEVIEWER_H
