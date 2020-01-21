#ifndef CFILEBROWSER_H
#define CFILEBROWSER_H


#include "common.h"
#include "cexif.h"
#include <QWidget>

#include <QFileSystemModel>
#include <QItemSelection>
#include <QStandardItemModel>
#include <QProgressBar>

#include <QFileInfo>


namespace Ui {
class cFileBrowser;
}

class cFileBrowser : public QWidget
{
	Q_OBJECT

public:
	explicit cFileBrowser(QProgressBar* lpProgressBar, QList<IMAGEFORMAT>* lpImageFormats, QWidget *parent = nullptr);
	~cFileBrowser();

private:
	Ui::cFileBrowser*		ui;
	QFileSystemModel		m_directoryListModel;
	QStandardItemModel*		m_lpFileListModel;
	QProgressBar*			m_lpProgressBar;
	QList<IMAGEFORMAT>*		m_lpImageFormats;

	cEXIFTagList			m_exifTAGList;
	cEXIFCompressionList	m_exifCompressionList;
	cEXIFLightSourceList	m_exifLightSourceList;
	cEXIFFlashList			m_exifFlashList;

	cIPTCTagList			m_iptcTagList;

	cXMPTagList				m_xmpTagList;

	void					addFile(const QFileInfo& fileInfo);

private slots:
	void					onDirectoryChanged(const QItemSelection& selected, const QItemSelection& deselected);
};

#endif // CFILEBROWSER_H
