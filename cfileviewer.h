#ifndef CFILEVIEWER_H
#define CFILEVIEWER_H


#include <QWidget>
#include <QList>
#include <QMetaType>


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
	QPixmap	image();

	QString	fileName();

	void	setCount(qint32 count);
	qint32	count();

private:
	Ui::cFileViewer*	ui;
	QString				m_fileName;

public slots:
	void				onCountChanged(int i);

signals:
	void				countChanged(const QString& fileName, QPixmap pixmap, int i);
};

Q_DECLARE_METATYPE(cFileViewer*)

class cFileViewerList : public QList<cFileViewer*>
{
public:
	cFileViewer*	add(const QString& fileName, const QPixmap& image);
	cFileViewer*	find(const QString& fileName);
};

#endif // CFILEVIEWER_H
