#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "causalitypanelgraphview.h"
#include "causalitypanelgraphviewnode.h"
#include "causalitypanelgrapviewhedge.h"
#include <QApplication>

#include "maincontroller.h"
#include "utils.h"
#include <QVector2D>
#include "colorandfontmap.h"

using namespace utils;

CCausalityPanelGraphNode::CCausalityPanelGraphNode(CCausalityPanelGraphView *pGraph, QGraphicsItem* pParentItem)
    : QGraphicsItem(pParentItem),
      mapColorsAndFonts_(CColorAndFontMap::instance()),
    _pGraphView(pGraph)
{
    clrOnPath_ = mapColorsAndFonts_.getColor("generic/alert");
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(2);
}

CCausalityPanelGraphNode::~CCausalityPanelGraphNode()
{

}

void CCausalityPanelGraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRect rectBorder = option->rect;

    painter->setRenderHint(QPainter::Antialiasing);
    QColor colorBorder = Qt::black;

    /* Draw background ******* */

    /* Treatment and effect */
    if(bIsTreatment_ || bIsEffect_)
    {
        /* Double border */
        QPen penBorder(colorBorder, 6);
        penBorder.setStyle(Qt::PenStyle::SolidLine);
        painter->setPen(penBorder);
        painter->fillRect(rectBorder, QBrush(group_.colorPrimary()));
        painter->drawRect(rectBorder);
    }
    else
    {
        QPen penBorder(colorBorder, 3);

        //    bIsHighlighted_ = true;
        if(bIsHighlighted_)
        {
            penBorder = QPen(Qt::red, 5);
        }

        QPainterPath path;
        path.addRoundedRect(rectBorder, 8, 8);
        /* Markov blanket */
        if(bIsInMarkovPlusSet_)
        {
            /* Dashed border */
            penBorder.setStyle(Qt::PenStyle::DashLine);
            painter->setPen(penBorder);
            painter->fillPath(path, QBrush(group_.colorPrimary()));
            painter->drawRoundedRect(rectBorder, 8, 8);
        }
        else
        {
            /* Markov blanket */
            if(bIsInMarkovSet_)
            {
                /* Normal border */
                penBorder.setStyle(Qt::PenStyle::SolidLine);
                painter->setPen(penBorder);
                painter->fillPath(path, QBrush(group_.colorPrimary()));
                painter->drawRoundedRect(rectBorder, 8, 8);
            }
            else
            {
                /* Normal border */
                penBorder.setStyle(Qt::PenStyle::SolidLine);
                painter->setPen(penBorder);
                painter->fillPath(path, QBrush(group_.colorPrimary()));
                painter->drawRoundedRect(rectBorder, 8, 8);
            }
        }
    }

    /* Draw label ******** */
    int iMargin = 6;
    QRect rect = option->rect.adjusted(iMargin, iMargin, -iMargin, -iMargin);
    painter->setFont(font_);
    QFont font = painter->font();
    QPen penFont(mapColorsAndFonts_.contrastColor(group().colorPrimary()), 1);
    painter->setPen(penFont);
    font.setPointSize(iDefaultTextSize_);
    font.setBold(true);
    if(bIsTreatment_ || bIsEffect_)
    {
        font.setPointSize(iSpecialTextSize_);
    }
    painter->setFont(font);
    painter->drawText(rect, text(), QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
}

QRectF CCausalityPanelGraphNode::boundingRect() const
{
    return QRectF(0, 0, _size.width(), _size.height());
}

QPainterPath CCausalityPanelGraphNode::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(0, 0, _size.width(), _size.height()));
    return path;
}

QPointF CCausalityPanelGraphNode::edgeInAnchor(const QPointF &pntTarget) const
{
    QPointF pntTopCenter = pos() + QPointF(boundingRect().center().rx(), boundingRect().top());
    QPointF pntBottomCenter = pos() +QPointF(boundingRect().center().rx(), boundingRect().bottom());
    QPointF pntLeftCenter =  pos() + QPointF(boundingRect().left(), boundingRect().center().ry());
    QPointF pntRightCenter =pos() + QPointF(boundingRect().right(), boundingRect().center().ry());

    int disTop = (int) qAbs(QVector2D(pntTopCenter - pntTarget).length());
    int disBottom = (int) qAbs(QVector2D(pntBottomCenter - pntTarget).length());
    int disLeft = (int) qAbs(QVector2D(pntLeftCenter - pntTarget).length());
    int disRight =(int) qAbs(QVector2D(pntRightCenter - pntTarget).length());

    QPointF pntAnchor;
    if(disTop <= qMin(disBottom, qMin(disLeft, disRight)))
    {
        pntAnchor = pntTopCenter;
    }
    if(disBottom <= qMin(disTop, qMin(disLeft, disRight)))
    {
        pntAnchor = pntBottomCenter;
    }
    if(disLeft <= qMin(disTop, qMin(disBottom, disRight)))
    {
        pntAnchor = pntLeftCenter;
    }
    if(disRight <= qMin(disBottom, qMin(disLeft, disTop)))
    {
        pntAnchor = pntRightCenter;
    }

    return pntAnchor;
}

QPointF CCausalityPanelGraphNode::edgeOutAnchor(const QPointF &pntTarget) const
{
    QPointF pntTopCenter = pos() + QPointF(boundingRect().center().rx(), boundingRect().top());
    QPointF pntBottomCenter = pos() +QPointF(boundingRect().center().rx(), boundingRect().bottom());
    QPointF pntLeftCenter =  pos() + QPointF(boundingRect().left(), boundingRect().center().ry());
    QPointF pntRightCenter =pos() + QPointF(boundingRect().right(), boundingRect().center().ry());

    int disTop = (int) qAbs(QVector2D(pntTopCenter - pntTarget).length());
    int disBottom = (int) qAbs(QVector2D(pntBottomCenter - pntTarget).length());
    int disLeft = (int) qAbs(QVector2D(pntLeftCenter - pntTarget).length());
    int disRight =(int) qAbs(QVector2D(pntRightCenter - pntTarget).length());

    QPointF pntAnchor;
    if(disTop <= qMin(disBottom, qMin(disLeft, disRight)))
    {
        pntAnchor = pntTopCenter;
    }
    if(disBottom <= qMin(disTop, qMin(disLeft, disRight)))
    {
        pntAnchor = pntBottomCenter;
    }
    if(disLeft <= qMin(disTop, qMin(disBottom, disRight)))
    {
        pntAnchor = pntLeftCenter;
    }
    if(disRight <= qMin(disBottom, qMin(disLeft, disTop)))
    {
        pntAnchor = pntRightCenter;
    }

    return pntAnchor;
}

void CCausalityPanelGraphNode::addEdgeOut(CCausalityPanelGraphEdge *pEdge)
{
    _lstEdgesOut.append(pEdge);
}

void CCausalityPanelGraphNode::removeEdgeOut(CCausalityPanelGraphEdge *pEdge)
{
    if(scene())
    {
            scene()->removeItem(pEdge);
    }
    _lstEdgesOut.removeOne(pEdge);
}

QList<CCausalityPanelGraphEdge *> CCausalityPanelGraphNode::edgesOut() const
{
    return _lstEdgesOut;
}

void CCausalityPanelGraphNode::clearEdgesOut()
{
    if(scene())
    {
        foreach(CCausalityPanelGraphEdge* pEdge, _lstEdgesOut)
        {
            scene()->removeItem(pEdge);
        }
    }

    _lstEdgesOut.clear();
}

QList<CCausalityPanelGraphEdge *> CCausalityPanelGraphNode::edgesIn() const
{
    return _lstEdgesIn;
}

void CCausalityPanelGraphNode::addEdgeIn(CCausalityPanelGraphEdge *pEdge)
{
    _lstEdgesIn.append(pEdge);
}

void CCausalityPanelGraphNode::removeEdgeIn(CCausalityPanelGraphEdge *pEdge)
{
    if(scene())
    {
        scene()->removeItem(pEdge);
    }
    _lstEdgesOut.removeOne(pEdge);
}

void CCausalityPanelGraphNode::clearEdgesIn()
{
    if(scene())
    {
        foreach(CCausalityPanelGraphEdge* pEdge, _lstEdgesIn)
        {
            scene()->removeItem(pEdge);
        }
    }
    _lstEdgesIn.clear();
}

void CCausalityPanelGraphNode::clearEdges()
{
    clearEdgesIn();
    clearEdgesOut();
}

QSize CCausalityPanelGraphNode::size() const
{
    return _size;
}

void CCausalityPanelGraphNode::setSize(const QSize &size)
{
    _size = size;
}

bool CCausalityPanelGraphNode::visited() const
{
    return bVisited_;
}

void CCausalityPanelGraphNode::setVisited(bool bVisited)
{
    bVisited_ = bVisited;
}

CVariable CCausalityPanelGraphNode::variable() const
{
    return variable_;
}

void CCausalityPanelGraphNode::setVariable(const CVariable &variable)
{
    variable_ = variable;
}

CGroup CCausalityPanelGraphNode::group() const
{
    return group_;
}

void CCausalityPanelGraphNode::setGroup(const CGroup &group)
{
    group_ = group;
}

bool CCausalityPanelGraphNode::isTreatment() const
{
    return bIsTreatment_;
}

void CCausalityPanelGraphNode::setIsTreatment(bool value)
{
    bIsTreatment_ = value;
    fitToText();
}

bool CCausalityPanelGraphNode::isEffect() const
{
    return bIsEffect_;
}

void CCausalityPanelGraphNode::setIsEffect(bool value)
{
    bIsEffect_ = value;
    fitToText();
}

void CCausalityPanelGraphNode::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }
//! [2]

//! [3]
    // Sum up all forces pushing this item away
    qreal xvel = 0;
    qreal yvel = 0;
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items) {
        CCausalityPanelGraphNode *node = qgraphicsitem_cast<CCausalityPanelGraphNode *>(item);
        if (!node)
            continue;

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0) {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }
//! [3]

//! [4]
    // Now subtract all forces pulling items together
    double weightFactor = 20;
    double weight = (_lstEdgesOut.size() + _lstEdgesIn.size() + 1) * weightFactor;
    for (const CCausalityPanelGraphEdge *edge : qAsConst(_lstEdgesOut))
    {
        QPointF vec;
        vec = mapToItem(edge->targetNode(), 0, 0);
        xvel = yvel = 0;
        if(vec.x()*vec.x() + vec.y() + vec.y() > 400*400)
        {
            xvel -= vec.x() / weight;
            yvel -= vec.y() / weight;
        }
    }

    for (const CCausalityPanelGraphEdge *edge : qAsConst(_lstEdgesIn))
    {
        QPointF vec;
        vec = mapToItem(edge->sourceNode(), 0, 0);
        xvel = yvel = 0;
        if(vec.x()*vec.x() + vec.y() + vec.y() > 400*400)
        {
            xvel -= vec.x() / weight;
            yvel -= vec.y() / weight;
        }
    }
//    double weight = (edges.size() + 1) * 10;
//    for (const Edge *edge : qAsConst(edges)) {
//        QPointF vec;
//        if (edge->sourceNode() == this)
//            vec = mapToItem(edge->destNode(), 0, 0);
//        else
//            vec = mapToItem(edge->sourceNode(), 0, 0);
//        xvel -= vec.x() / weight;
//        yvel -= vec.y() / weight;
//    }
//! [4]

//! [5]
    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;
//! [5]

//! [6]
//!
    int iValue = 100;
    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + iValue), sceneRect.right() - iValue));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + iValue), sceneRect.bottom() - iValue));
}

bool CCausalityPanelGraphNode::advancePosition()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

bool CCausalityPanelGraphNode::IsInMarkovSet() const
{
    return bIsInMarkovSet_;
}

void CCausalityPanelGraphNode::setIsInMarkovSet(bool bIsInMarkovSet)
{
    bIsInMarkovSet_ = bIsInMarkovSet;
}

bool CCausalityPanelGraphNode::IsInMarkovPlusSet() const
{
    return bIsInMarkovPlusSet_;
}

void CCausalityPanelGraphNode::setIsInMarkovPlusSet(bool bIsInMarkovPlusSet)
{
    bIsInMarkovPlusSet_ = bIsInMarkovPlusSet;
}

bool CCausalityPanelGraphNode::highlighted() const
{
    return bIsHighlighted_;
}

void CCausalityPanelGraphNode::setIsHighlighted(bool bIsHighlighted)
{
    bIsHighlighted_ = bIsHighlighted;
}

QString CCausalityPanelGraphNode::text() const
{
    return text_;
}

void CCausalityPanelGraphNode::setText(const QString &text)
{
    text_ = text;
    fitToText();
}

bool CCausalityPanelGraphNode::isInMinedGraph() const
{
    return bIsInMinedGraph_;
}

void CCausalityPanelGraphNode::setIsInMinedGraph(bool bIsInMinedGraph)
{
    bIsInMinedGraph_ = bIsInMinedGraph;
}

void CCausalityPanelGraphNode::setPos(const QPointF &pos)
{

    if(_pGraphView)
        _pGraphView->nodeItemPositionUpdated(this, pos);
    QGraphicsItem::setPos(pos);
}

void CCausalityPanelGraphNode::setPos(qreal x, qreal y)
{
    if(_pGraphView)
        _pGraphView->nodeItemPositionUpdated(this, QPointF(x, y));
    QGraphicsItem::setPos(x, y);
}

void CCausalityPanelGraphNode::fitToText()
{
    font_.setPointSize(iDefaultTextSize_);
    font_.setBold(true);
    if(bIsTreatment_ || bIsEffect_)
    {
        font_.setPointSize(iSpecialTextSize_);
    }

    QFontMetrics fm(font_);
    setSize(QSize(fm.horizontalAdvance(text()) + 16, fm.height() + 8));

}

void CCausalityPanelGraphNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    
    _pntPreviousPosition = pos();
}

void CCausalityPanelGraphNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);

    if(_bItemMoved)
    {
        _pGraphView->nodeMoved(this);
    }
}

QVariant CCausalityPanelGraphNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
//    qDebug() << "CCausalityPanelGraphNode::itemchanged";
    switch (change)
    {
        case ItemPositionHasChanged:
        {
            if(_pntPreviousPosition != pos().toPoint())
            {
//                CNode node = data(NODE_DATA_VARIABLE).value<CNode>();
                foreach (CCausalityPanelGraphEdge *edge, edgesIn())
                {
                    CCausalityPanelGraphEdge* pEdge = dynamic_cast<CCausalityPanelGraphEdge*>(edge);
                    pEdge->renderGeometry();
                }
                foreach (CCausalityPanelGraphEdge *edge, edgesOut())
                {
                    CCausalityPanelGraphEdge* pEdge = dynamic_cast<CCausalityPanelGraphEdge*>(edge);
                    pEdge->renderGeometry();
                }
                _bItemMoved = true;
                if(!QApplication::mouseButtons()& Qt::LeftButton)
                {
                    _bItemMoved = false;
                }
                _pGraphView->nodeItemPositionUpdated(this, pos());
            }
        }
        break;
        default:
            break;
        }

    return QGraphicsItem::itemChange(change, value);
}

void CCausalityPanelGraphNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);

    _pGraphView->nodeDoubleClicked(this);
}

