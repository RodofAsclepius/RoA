#include "extendedgraphicsscene.h"

CExtendedGraphicsScene::CExtendedGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{

}

CExtendedGraphicsScene::CExtendedGraphicsScene(const QRectF &sceneRect, QObject *parent)
    : QGraphicsScene(sceneRect, parent)
{

}

CExtendedGraphicsScene::CExtendedGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(x, y, width, height, parent)
{

}

CExtendedGraphicsScene::~CExtendedGraphicsScene()
{

}

void CExtendedGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
//    if (mouseEvent->button() != Qt::LeftButton)
//        return;

//    DiagramItem *item;
//    switch (myMode) {
//        case InsertItem:
//            item = new DiagramItem(myItemType, myItemMenu);
//            item->setBrush(myItemColor);
//            addItem(item);
//            item->setPos(mouseEvent->scenePos());
//            emit itemInserted(item);
//            break;
////! [6] //! [7]
//        case InsertLine:
//            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
//                                        mouseEvent->scenePos()));
//            line->setPen(QPen(myLineColor, 2));
//            addItem(line);
//            break;
////! [7] //! [8]
//        case InsertText:
//            textItem = new DiagramTextItem();
//            textItem->setFont(myFont);
//            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
//            textItem->setZValue(1000.0);
//            connect(textItem, &DiagramTextItem::lostFocus,
//                    this, &DiagramScene::editorLostFocus);
//            connect(textItem, &DiagramTextItem::selectedChange,
//                    this, &DiagramScene::itemSelected);
//            addItem(textItem);
//            textItem->setDefaultTextColor(myTextColor);
//            textItem->setPos(mouseEvent->scenePos());
//            emit textInserted(textItem);
////! [8] //! [9]
//    default:
//        ;
//    }
//    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CExtendedGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
//    if (myMode == InsertLine && line != nullptr) {
//        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
//        line->setLine(newLine);
//    } else if (myMode == MoveItem) {
//        QGraphicsScene::mouseMoveEvent(mouseEvent);
//    }
}

void CExtendedGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
//    if (line != nullptr && myMode == InsertLine) {
//        QList<QGraphicsItem *> startItems = items(line->line().p1());
//        if (startItems.count() && startItems.first() == line)
//            startItems.removeFirst();
//        QList<QGraphicsItem *> endItems = items(line->line().p2());
//        if (endItems.count() && endItems.first() == line)
//            endItems.removeFirst();

//        removeItem(line);
//        delete line;
////! [11] //! [12]

//        if (startItems.count() > 0 && endItems.count() > 0 &&
//            startItems.first()->type() == DiagramItem::Type &&
//            endItems.first()->type() == DiagramItem::Type &&
//            startItems.first() != endItems.first()) {
//            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
//            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
//            Arrow *arrow = new Arrow(startItem, endItem);
//            arrow->setColor(myLineColor);
//            startItem->addArrow(arrow);
//            endItem->addArrow(arrow);
//            arrow->setZValue(-1000.0);
//            addItem(arrow);
//            arrow->updatePosition();
//        }
//    }
//! [12] //! [13]
    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

//bool CExtendedGraphicsScene::isItemChange(int type) const
//{
//    const QList<QGraphicsItem *> items = selectedItems();
//    const auto cb = [type](const QGraphicsItem *item) { return item->type() == type; };
//    return std::find_if(items.begin(), items.end(), cb) != items.end();
//}
