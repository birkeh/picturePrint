/*!
 \file cfilebrowser.h

*/

#ifndef CFILEBROWSER_H
#define CFILEBROWSER_H


#include "common.h"
#include "cexif.h"
#include "cfileviewer.h"

#include <QWidget>

#include <QListView>
#include <QFileSystemModel>
#include <QItemSelection>
#include <QStandardItemModel>
#include <QProgressBar>

#include <QFileInfo>

#include <QSqlDatabase>


namespace Ui {
class cFileBrowser;
}

/*!
 \brief

 \class cFileBrowser cfilebrowser.h "cfilebrowser.h"
*/
class cFileBrowser : public QWidget
{
	Q_OBJECT

public:
	/*!
	 \brief

	 \fn cFileBrowser
	 \param lpProgressBar
	 \param lpImageFormats
	 \param parent
	*/
	explicit cFileBrowser(QProgressBar* lpProgressBar, QList<IMAGEFORMAT>* lpImageFormats, QListView* lpSelectedList, QStandardItemModel* lpSelectedListModel, QWidget *parent = nullptr);
	/*!
	 \brief

	 \fn ~cFileBrowser
	*/
	~cFileBrowser();

private:
	Ui::cFileBrowser*		ui;						/*!< TODO: describe */
	QListView*				m_lpSelectedList;		/*!< TODO: describe */
	QFileSystemModel		m_directoryListModel;	/*!< TODO: describe */
	QStandardItemModel*		m_lpFileListModel;		/*!< TODO: describe */
	QStandardItemModel*		m_lpSelectedListModel;	/*!< TODO: describe */
	QProgressBar*			m_lpProgressBar;		/*!< TODO: describe */
	QList<IMAGEFORMAT>*		m_lpImageFormats;		/*!< TODO: describe */

	cEXIFTagList			m_exifTAGList;			/*!< TODO: describe */
	cEXIFCompressionList	m_exifCompressionList;	/*!< TODO: describe */
	cEXIFLightSourceList	m_exifLightSourceList;	/*!< TODO: describe */
	cEXIFFlashList			m_exifFlashList;		/*!< TODO: describe */

	cIPTCTagList			m_iptcTagList;			/*!< TODO: describe */

	cXMPTagList				m_xmpTagList;			/*!< TODO: describe */

	QSqlDatabase			m_cacheDB;				/*!< TODO: describe */

	bool					m_working;				/*!< TODO: describe */

	/*!
	 \brief

	 \fn directorySelected
	 \param filePath
	*/
	void					directorySelected(const QString& filePath);
	/*!
	 \brief

	 \fn addFile
	 \param fileInfo
	*/
	void					addFile(const QFileInfo& fileInfo);

private slots:
	/*!
	 \brief

	 \fn onDirectoryChanged
	 \param selected
	 \param deselected
	*/
	void					onDirectoryChanged(const QItemSelection& selected, const QItemSelection& deselected);
	/*!
	 \brief

	 \fn onCountChanged
	 \param fileViewer
	*/
	void					onCountChanged(cFileViewer* fileViewer);
	/*!
	 \brief

	 \fn onCountChangedSelected
	 \param fileViewer
	*/
	void					onCountChangedSelected(cFileViewer* fileViewer);
};

#endif // CFILEBROWSER_H
