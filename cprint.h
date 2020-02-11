/*!
 \file cprint.h

*/

#ifndef CPRINT_H
#define CPRINT_H


#include <QWidget>

#include <QProgressBar>
#include <QListView>
#include <QStandardItemModel>

#include <QPrinter>


namespace Ui {
class cPrint;
}

/*!
 \brief

 \class cPrint cprint.h "cprint.h"
*/
class cPrint : public QWidget
{
	Q_OBJECT

public:
	/*!
	 \brief

	 \fn cPrint
	 \param parent
	*/
	explicit cPrint(QProgressBar* lpProgressBar, QListView* lpSelectedList, QStandardItemModel* lpSelectedListModel, QWidget *parent = nullptr);
	/*!
	 \brief

	 \fn ~cPrint
	*/
	~cPrint();

private:
	Ui::cPrint*			ui;						/*!< TODO: describe */
	QProgressBar*		m_lpProgressBar;		/*!< TODO: describe */
	QListView*			m_lpSelectedList;		/*!< TODO: describe */
	QStandardItemModel*	m_lpSelectedListModel;	/*!< TODO: describe */

	QPrinter*			m_lpPrinter;			/*!< TODO: describe */

private slots:
	/*!
	 \brief

	 \fn onPrinterSettings
	*/
	void					onPrinterSettings();
	/*!
	 \brief

	 \fn onPrint
	*/
	void					onPrint();
};

#endif // CPRINT_H
