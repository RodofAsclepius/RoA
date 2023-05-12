#include "popupwidget.h"
#include "utils.h"

#include <QGridLayout>
#include <QMouseEvent>


CPopupWidget::CPopupWidget(QWidget *parent, QPoint pos, const QRect& rect)
    : QWidget(parent),
      _pChildWidget(nullptr)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setWindowModality(Qt::ApplicationModal);

    setAttribute( Qt::WA_DeleteOnClose, false);

    _rectActivated = rect;

    move(pos);

    setMouseTracking(true);
}


CPopupWidget::~CPopupWidget()
{
    if(_pChildWidget != nullptr)
    {
        _pGridLayout->removeWidget(_pChildWidget);
    }
}


void CPopupWidget::setChildWidget(QWidget *pChildWidget)
{
    _pChildWidget = pChildWidget;

    _pGridLayout = new QGridLayout();
    _pGridLayout->addWidget(_pChildWidget);
    setLayout(_pGridLayout);
}


QWidget *CPopupWidget::getChildWidget() const
{
    return _pChildWidget;
}


void CPopupWidget::onMouseLeftRect()
{
    close();
}


void CPopupWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mousePos(event->screenPos().x(), event->screenPos().y());

    QRect windowRect = frameGeometry();

    if(!_rectActivated.contains(mousePos) && !windowRect.contains(mousePos))
    {
        onMouseLeftRect();
    }
}


