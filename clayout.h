/*!
 \file clayout.h

*/

#ifndef CLAYOUT_H
#define CLAYOUT_H


#include <QWidget>
#include <QPageSize>


namespace Ui {
class cLayout;
}

/*!
 \brief

 \class cLayout clayout.h "clayout.h"
*/
class cLayout : public QWidget
{
	Q_OBJECT

public:
	/*!
	 \brief

	 \fn cLayout
	 \param parent
	*/
	explicit cLayout(QWidget *parent = nullptr);
	/*!
	 \brief

	 \fn ~cLayout
	*/
	~cLayout();

	QPageSize				pageSize();
	qreal					borderTop();
	qreal					borderLeft();
	qreal					borderRight();
	qreal					borderBottom();
	qreal					gutter();
	int						tilesH();
	int						tilesV();
	QPageSize::Unit			unit();

private:
	Ui::cLayout*			ui;				/*!< TODO: describe */
	bool					m_initializing;	/*!< TODO: describe */

	/*!
	 \brief

	 \fn redrawPreview
	*/
	void					redrawPreview();

private slots:
	/*!
	 \brief

	 \fn onPageSizeChanged
	 \param index
	*/
	void					onPageSizeChanged(int index);

	/*!
	 \brief

	 \fn onUnitChanged
	 \param index
	*/
	void					onUnitChanged(int index);

	/*!
	 \brief

	 \fn onAllTheSameChanged
	 \param state
	*/
	void					onAllTheSameChanged(int state);

	/*!
	 \brief

	 \fn onTileHChanged
	 \param value
	*/
	void					onTileHChanged(int value);
	/*!
	 \brief

	 \fn onTileVChanged
	 \param value
	*/
	void					onTileVChanged(int value);

	/*!
	 \brief

	 \fn onBorderTopChanged
	 \param value
	*/
	void					onBorderTopChanged(double value);
	/*!
	 \brief

	 \fn onBorderLeftChanged
	 \param value
	*/
	void					onBorderLeftChanged(double value);
	/*!
	 \brief

	 \fn onBorderRightChanged
	 \param value
	*/
	void					onBorderRightChanged(double value);
	/*!
	 \brief

	 \fn onBorderBottomChanged
	 \param value
	*/
	void					onBorderBottomChanged(double value);
	/*!
	 \brief

	 \fn onGutterChanged
	 \param value
	*/
	void					onGutterChanged(double value);

signals:
	/*!
	 \brief

	 \fn layoutChanged
	 \param layout
	 \param i
	*/
	void					layoutChanged();

protected:
	/*!
	 \brief

	 \fn resizeEvent
	 \param event
	*/
	void					resizeEvent(QResizeEvent *event) override;
};

#endif // CLAYOUT_H
