#include "processingoverlaywidget.h"
#include "utils.h"

#include <QPainter>
#include <QTimerEvent>
#include <QApplication>
#include <QScreen>
#include <QFont>
#include <QKeyEvent>
#include <QMovie>
//#include <QDesktopWidget>

CProcessingOverlayWidget::CProcessingOverlayWidget(QWidget *parent)
    : QDialog(parent),
      _pParentWidget(parent),
      _iNumberOfShows(0),
      _pSpinnerMovie(new QMovie(":/view/images/loader.gif"))
{
    setWindowFlags(Qt::Sheet | Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);
    setModal(true);

    setWindowModality(Qt::WindowModal);
    setAttribute(Qt::WA_TranslucentBackground);

    _pSpinnerMovie->start();
}


void CProcessingOverlayWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QColor backgroundColor = Qt::lightGray;
    backgroundColor.setAlpha(50);

    QPainter customPainter(this);
    customPainter.fillRect(rect(), backgroundColor);

    /* Draw text */
    QFont font;
    font.setPixelSize(42);
    customPainter.setFont(font);

    QString text = "Processing...";
    customPainter.drawText(rect(), Qt::AlignCenter, text);

    /* Draw loader */
    QFontMetrics metrics(font);
    QSize sizeText = metrics.size(0, text);
    QPixmap pixmap =_pSpinnerMovie->currentPixmap();
    int iCenterX = (rect().width() / 2);
    int iCenterY = (rect().height() / 2);
    customPainter.drawPixmap(iCenterX - ((sizeText.width() * 5/6) - (pixmap.width() / 2)),
                             iCenterY - (pixmap.height() / 2), pixmap);

    QWidget::paintEvent(event);
}


void CProcessingOverlayWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Escape:
            if(event->modifiers() & Qt::ShiftModifier)
            {
                hide();
            }
            break;
        default:
            QWidget::keyPressEvent(event);
    }
}


void CProcessingOverlayWidget::showEvent(QShowEvent *event)
{
//    DEBUG_OBJ_INFO

    Q_UNUSED(event)

    QWidget::showEvent(event);

    QRect rectParent = _pParentWidget->frameGeometry();
    QPoint posCenter = QPoint(_pParentWidget->pos().x() + rectParent.width() / 2,
                              _pParentWidget->pos().y() + rectParent.height() / 2);

    int iWidth = rectParent.width() - 20;
    int iHeight = 200;

    resize(iWidth, iHeight);
    move(posCenter.x() - iWidth / 2, posCenter.y() - iHeight / 2);

    _pSpinnerMovie->start();
    ++_iNumberOfShows;
    _iTimerIDRepaint = startTimer(100);
}


void CProcessingOverlayWidget::hideEvent(QHideEvent *event)
{
//    DEBUG_OBJ_INFO

    --_iNumberOfShows;

    if(_iNumberOfShows < 1)
    {
        QWidget::hideEvent(event);
        _pSpinnerMovie->stop();
    }
}


void CProcessingOverlayWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

    if(event->timerId() == _iTimerIDRepaint)
    {
        repaint();
    }
}


