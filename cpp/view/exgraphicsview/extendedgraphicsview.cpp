#include "extendedgraphicsview.h"
#include "colorandfontmap.h"
#include <QMouseEvent>

CExtendedGraphicsView::CExtendedGraphicsView()
    : QGraphicsView()
{
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::TextAntialiasing);

    pScene_ = new QGraphicsScene(0, 0, 9999, 9999, this);
    setScene(pScene_);

    CColorAndFontMap &clrFont = CColorAndFontMap::instance();
    QColor clrBackGround = clrFont.getColor("widget/chart");
    setBackgroundBrush(clrBackGround);
}

CExtendedGraphicsView::~CExtendedGraphicsView()
{

}

//bool CExtendedGraphicsView::eventFilter(QObject *object, QEvent *event) {

//  if (event->type() == QEvent::MouseButtonPress)
//  {
//      QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
//      // Enter here any button you like
////      if (mouse_event->button() == Qt::MiddleButton)
//      if (mouse_event->button() == Qt::RightButton)
//      {
//          // temporarly enable dragging mode
//          this->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
//          // emit a left mouse click (the default button for the drag mode)
//          QMouseEvent* pressEvent = new QMouseEvent(QEvent::GraphicsSceneMousePress,
//                                    mouse_event->pos(), Qt::MouseButton::LeftButton,
//                                    Qt::MouseButton::LeftButton, Qt::KeyboardModifier::NoModifier);

//          this->mousePressEvent(pressEvent);
//      }
//      else if (event->type() == QEvent::MouseButtonRelease)
//      {
//          // disable drag mode if dragging is finished
//          this->setDragMode(QGraphicsView::DragMode::NoDrag);
//      }

//      Q_UNUSED(object)
//      return false;
//  }
//}

void CExtendedGraphicsView::doTranslate(qreal rx, qreal ry)
{
    QTransform old_transform = transform();

    QRectF scene_rect = scene()->sceneRect();
    QRectF new_scene_rect(scene_rect.x()-rx,
                          scene_rect.y()-ry,
                          scene_rect.width(),
                          scene_rect.height());
    scene()->setSceneRect(new_scene_rect);

    setTransform(old_transform);
}

void CExtendedGraphicsView::alignOriginLeft()
{
    QTransform old_transform = transform();
    old_transform.translate(-old_transform.dx(), 0);
    setTransform(old_transform);
}
