#include "causalitypanelgrapviewhedge.h"
#include "causalitypanelgraphviewnode.h"
#include "utils.h"

#include <QGraphicsItem>
#include <qmath.h>
#include <QPainter>
#include <QObject>
#include <QDebug>
#include <QRandomGenerator>
#include <QVector2D>
#include <QKeyEvent>
#include <causalitypanelgraphview.h>

CCausalityPanelGraphEdge::CCausalityPanelGraphEdge(CCausalityPanelGraphNode *sourceNode,
                                                   CCausalityPanelGraphNode *destNode,
                                                   const QString& strlabel)
    : QGraphicsItem()
{
    setFlag(ItemIsSelectable);
//    setFlag(QGraphicsItem::ItemIsFocusable);

    setLabel(strlabel);
    pSourceNode_ = sourceNode;
    pTargetNode_ = destNode;

    pSourceNode_->addEdgeOut(this);
    destNode->addEdgeIn(this);

    setZValue(1);
}

CCausalityPanelGraphEdge::~CCausalityPanelGraphEdge()
{

}

CCausalityPanelGraphNode *CCausalityPanelGraphEdge::sourceNode() const
{
    return pSourceNode_;
}

CCausalityPanelGraphNode *CCausalityPanelGraphEdge::targetNode() const
{
    return pTargetNode_;
}

QRectF CCausalityPanelGraphEdge::boundingRect() const
{
    if (!pSourceNode_ || !pTargetNode_)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = 1*(penWidth + arrowWidth_) / 2.0;

    return QRectF(pntSourceAnchor_, QSizeF(pntTargetAnchor_.x() - pntSourceAnchor_.x(),
                                      pntTargetAnchor_.y() - pntSourceAnchor_.y()))
        .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

QPainterPath CCausalityPanelGraphEdge::shape() const
{
    QPainterPath path;
    path.addPath(pathArrow_);
    return path;
}

void CCausalityPanelGraphEdge::paintArrow(QPainter *painter, const QString &strTargetLabel,
                                          const QColor &color1, const QColor &colorBorder)
{
    QPointF pntSource = lineEdge_.p1();
    QPointF pntTarget = lineEdge_.p2();
    QVector2D vecDirSourceTarget = QVector2D(pntTarget - pntSource);
    vecDirSourceTarget.normalize();
    QVector2D vecDirOrth = QVector2D(QPointF(-lineEdge_.dy(), lineEdge_.dx()));
    vecDirOrth.normalize();

    QPointF pntSourceOffset = pntSource + (arrowLength_/2.0 * vecDirSourceTarget.toPointF());
    QPointF pntTargetOffset = pntTarget - (arrowLength_/2.0 * vecDirSourceTarget.toPointF());

    QPointF pntSourceOffset2 = pntSourceOffset + (arrowLength_/2.0 * vecDirSourceTarget.toPointF());
    QPointF pntTargetOffset2 = pntTargetOffset - (arrowLength_/2.0 * vecDirSourceTarget.toPointF());

    QPointF pntSourceLeft = pntSourceOffset + vecDirOrth.toPointF() * edgeWidth_/3.0;
    QPointF pntSourceRight = pntSourceOffset + vecDirOrth.toPointF() * -edgeWidth_/3.0;
    QPointF pntTargetLeft = pntTargetOffset + vecDirOrth.toPointF() * edgeWidth_/3.0;
    QPointF pntTargetRight = pntTargetOffset + vecDirOrth.toPointF() * -edgeWidth_/3.0;

    QPointF pntSourceLeft2 = pntSourceOffset2 + vecDirOrth.toPointF() * edgeWidth_/2.0;
    QPointF pntSourceRight2 = pntSourceOffset2 + vecDirOrth.toPointF() * -edgeWidth_/2.0;
    QPointF pntTargetLeft2 = pntTargetOffset2 + vecDirOrth.toPointF() * edgeWidth_/2.0;
    QPointF pntTargetRight2 = pntTargetOffset2 + vecDirOrth.toPointF() * -edgeWidth_/2.0;

    QPolygonF polyGon;
    polyGon << pntSource
            << pntSourceRight
            << pntSourceRight2
            << pntTargetRight2
            << pntTargetRight
            << pntTarget
            << pntTargetLeft
            << pntTargetLeft2
            << pntSourceLeft2
            << pntSourceLeft
            << pntSource;

    QBrush brush(color1);

    /* Edge*/
    QPen penCurve = QPen(colorBorder, 1);
    colorArrowBorder_ = colorBorder;
    if(isSelected())
    {
        penCurve = QPen(Qt::red, 1);
        colorArrowBorder_ = Qt::red;
    }
    painter->setPen(penCurve);
    painter->setBrush(brush);
    painter->drawPolygon(polyGon);
    pathArrow_.addPolygon(polyGon);

    /* Arrow target */
    QPen penArrowTarget= QPen(colorArrowBorder_, rArrowBorderWidth_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(penArrowTarget);
    QBrush brushArrowTarget(brush);
    brushArrowTarget.setStyle(Qt::BrushStyle::SolidPattern);
    painter->setBrush(brushArrowTarget);

    QString strTargetNodeLabel = strTargetLabel;//edge.targetNode().label();
    QPainterPath pathTargetArrow = mapPainterPaths_.value(strTargetNodeLabel);

    painter->drawPath(pathTargetArrow);
    pathArrow_.addPath(pathTargetArrow);
}

void CCausalityPanelGraphEdge::paintBiDirectionalArrow(QPainter *painter, const QString &strTargetLabel,
                                                       const QString &strTargetLabel2, const QColor &color,
                                                       const QColor &color2, const QColor &colorBorder)
{

    qreal edgeWidth = edgeWidth_;
    edgeWidth *= 2;

    /* Nodes disagree */
    QVector2D vecDir = QVector2D(QPointF(-lineEdge_.dy(), lineEdge_.dx()));
    vecDir.normalize();

    QPointF pntSource = lineEdge_.p1();
    QPointF pntTarget = lineEdge_.p2();
    QVector2D vecDirSourceTarget = QVector2D(pntTarget - pntSource);
    vecDirSourceTarget.normalize();
    QVector2D vecDirOrth = QVector2D(QPointF(-lineEdge_.dy(), lineEdge_.dx()));
    vecDirOrth.normalize();

    QPointF pntSourceOffset = pntSource + (arrowLength_/2.0 * vecDirSourceTarget.toPointF());
    QPointF pntTargetOffset = pntTarget - (arrowLength_/2.0 * vecDirSourceTarget.toPointF());

    QPointF pntSourceOffset2 = pntSourceOffset + (arrowLength_/2.0 * vecDirSourceTarget.toPointF());
    QPointF pntTargetOffset2 = pntTargetOffset - (arrowLength_/2.0 * vecDirSourceTarget.toPointF());

    QPointF pntSourceLeft = pntSourceOffset + vecDirOrth.toPointF() * edgeWidth/3.0;
    QPointF pntSourceRight = pntSourceOffset + vecDirOrth.toPointF() * -edgeWidth/3.0;
    QPointF pntTargetLeft = pntTargetOffset + vecDirOrth.toPointF() * edgeWidth/3.0;
    QPointF pntTargetRight = pntTargetOffset + vecDirOrth.toPointF() * -edgeWidth/3.0;

    QPointF pntSourceLeft2 = pntSourceOffset2 + vecDirOrth.toPointF() * edgeWidth/2.0;
    QPointF pntSourceRight2 = pntSourceOffset2 + vecDirOrth.toPointF() * -edgeWidth/2.0;
    QPointF pntTargetLeft2 = pntTargetOffset2 + vecDirOrth.toPointF() * edgeWidth/2.0;
    QPointF pntTargetRight2 = pntTargetOffset2 + vecDirOrth.toPointF() * -edgeWidth/2.0;

    QPolygonF polyGon;
    polyGon << pntSource
            << pntSourceRight
            << pntSourceRight2
            << pntTargetRight2
            << pntTargetRight
            << pntTarget
            << pntTargetLeft
            << pntTargetLeft2
            << pntSourceLeft2
            << pntSourceLeft
            << pntSource;

    QLinearGradient gradient(lineEdge_.center() + vecDir.toPointF() * -edgeWidth,
                             lineEdge_.center() + vecDir.toPointF() * edgeWidth);
    gradient.setColorAt(0.0, color);
    gradient.setColorAt(0.49, color);
    gradient.setColorAt(0.5, color2);
    gradient.setColorAt(1.0, color2);
    QBrush brush(gradient);

    /* Edge*/
    QPen penCurve = QPen(gradient, 1);
    painter->setPen(penCurve);
    painter->setBrush(brush);
    painter->drawPolygon(polyGon);
    QPen dividingLine = QPen(Qt::white, .5);
    painter->setPen(dividingLine);
    painter->drawLine(lineEdge_);

    /* Arrow source */
    QPen penArrowSource = QPen(colorArrowBorder_, rArrowBorderWidth_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(penArrowSource);
    QBrush brushArrowSource(color);
    brushArrowSource.setStyle(Qt::BrushStyle::SolidPattern);
    painter->setBrush(brushArrowSource);

    QPainterPath pathTargetArrow = mapPainterPaths_.value(strTargetLabel);
    painter->drawPath(pathTargetArrow);

    /* Arrow target */
    QPen penArrowTarget= QPen(colorArrowBorder_, rArrowBorderWidth_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(penArrowTarget);
    QBrush brushArrowTarget(color2);
    brushArrowTarget.setStyle(Qt::BrushStyle::SolidPattern);
    painter->setBrush(brushArrowTarget);

    QPainterPath pathTargetArrow2 = mapPainterPaths_.value(strTargetLabel2);
    painter->drawPath(pathTargetArrow2);
}

void CCausalityPanelGraphEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!pSourceNode_ || !pTargetNode_)
        return;

    QList<CEdge> edges = multiEdge().edges();
    QColor clrBorder = Qt::darkGray;

    /* We have a single arrow */
    if(edges.count() == 1)
    {
        CEdge edge = edges.at(0);
        QVariantList lstVariants = edge.data().toList();
        CGroup group = lstVariants.at(0).value<CGroup>();
        bool bIsMinedGraph = lstVariants.at(1).value<bool>();

        QColor color;

        if(bIsMinedGraph)
        {
            color = clrMinedGraph_;
            clrBorder = Qt::lightGray;
        }
        else
        {
            color = group.colorPrimary();
        }

        paintArrow(painter, edge.targetNode().label(), color, clrBorder);
    }
    /* Two groups */
    if(edges.count() == 2)
    {
        CEdge edge = edges.at(0);
        CEdge edge2 = edges.at(1);

        /* Edges agree */
        if(edge.targetNode().label() == edge2.targetNode().label())
        {
            QVariantList lstVariants = edge.data().toList();
            CGroup group = lstVariants.at(0).value<CGroup>();
            clrBorder = Qt::lightGray;

            paintArrow(painter, edge.targetNode().label(), clrEdgesAgree_, clrBorder);
        }
        else /* Edges disagree */
        {
            /* Group 1 */
            QVariantList lstVariants = edge.data().toList();
            CGroup group1 = lstVariants.at(0).value<CGroup>();
            bool bIsMinedGraph1 = lstVariants.at(1).value<bool>();

            QColor color1;
            color1 = group1.colorPrimary();
            if(bIsMinedGraph1)
            {
                color1 = clrMinedGraph_;
                clrBorder = Qt::black;
            }

            /* Group 2 */
            QVariantList lstVariants2 = edge2.data().toList();
            CGroup group2 = lstVariants2.at(0).value<CGroup>();
            bool bIsMinedGraph2 = lstVariants2.at(1).value<bool>();

            QColor color2;
            color2 = group2.colorPrimary();
            if(bIsMinedGraph2)
            {
                color2 = clrMinedGraph_;
                clrBorder = Qt::black;
            }

            if(group1.name() == group2.name())
            {
                color2 = clrMinedGraph_;
//                clrBorder = Qt::black;
            }
            paintBiDirectionalArrow(painter, edge.targetNode().label(), edge2.targetNode().label(), color1, color2, clrBorder);
        }
    }
}

QList<QPointF> CCausalityPanelGraphEdge::computeAnchorPoints(const CCausalityPanelGraphNode *pNode) const
{
    QList<QPointF> lstPoints;

    /* hor, ver */
    lstPoints << pNode->pos() + QPointF(0, pNode->boundingRect().height()/2.0);
    lstPoints << pNode->pos() + QPointF(pNode->boundingRect().width(), pNode->boundingRect().height()/2.0);
    lstPoints << pNode->pos() + QPointF(pNode->boundingRect().width() / 2.0, 0);
    lstPoints << pNode->pos() + QPointF(pNode->boundingRect().width() / 2.0, pNode->boundingRect().height());

    /* diag */
    lstPoints << pNode->pos() + QPointF(0, 0);
    lstPoints << pNode->pos() + QPointF(0, pNode->boundingRect().height());
    lstPoints << pNode->pos() + QPointF(pNode->boundingRect().width(), 0);
    lstPoints << pNode->pos() + QPointF(pNode->boundingRect().width(), pNode->boundingRect().height());

    return lstPoints;
}

void CCausalityPanelGraphEdge::keyPressEvent(QKeyEvent *event)
{
//    qDebug() << "delete edge";

//    if(event->key() == Qt::Key_Delete)
//    {        if(!scene()->views().isEmpty())
//        {            CCausalityPanelGraphView* pView = dynamic_cast<CCausalityPanelGraphView*>(scene()->views().first());
//            if(pView)
//            {
//                pView->removeEdgeFromCausalModelGroup(this);
//            }
//        }
    //    }
}

bool CCausalityPanelGraphEdge::bConnectsToHighlightedNode() const
{

}

QString CCausalityPanelGraphEdge::label() const
{
    return strLabel_;
}

void CCausalityPanelGraphEdge::setLabel(const QString &strLabel)
{
    strLabel_ = strLabel;
}

void CCausalityPanelGraphEdge::renderGeometry()
{
    mapPainterPaths_.clear();
    CNode nodeSource = pSourceNode_->data(NODE_DATA_VARIABLE).value<CNode>();
    CNode nodeTarget = pTargetNode_->data(NODE_DATA_VARIABLE).value<CNode>();

    /* Try out 16 possible arrows */
    QList<QPointF> lstPointsSourceNode = computeAnchorPoints(pSourceNode_);
    QList<QPointF> lstPointsTargetNode = computeAnchorPoints(pTargetNode_);

    qreal rMinLength = 99999;

    /* Edge */
    foreach(QPointF pointSource, lstPointsSourceNode)
    {
        foreach(QPointF pointTarget, lstPointsTargetNode)
        {
            QLineF line = QLineF(pointSource, pointTarget);
            qreal rLength = line.length();

            if(rLength < rMinLength)
            {
                rMinLength = rLength;
                lineEdge_ = line;
                pntSourceAnchor_ = pointSource;
                pntTargetAnchor_ = pointTarget;
            }
        }
    }

    /* Arrow source */
    QPainterPath pathArrowSource = QPainterPath();
    QVector2D vecDirToTarget = QVector2D(lineEdge_.p2() - lineEdge_.p1());
    vecDirToTarget.normalize();
    QVector2D vecDirOrtho = QVector2D(QPointF(-lineEdge_.dy(), lineEdge_.dx()));
    vecDirOrtho.normalize();

    QPointF pntArrowBase = pntSourceAnchor_ + (arrowLength_ * vecDirToTarget.toPointF());
    QPointF pntArrowBaseLeft = pntArrowBase - (arrowWidth_ * vecDirOrtho.toPointF());
    QPointF pntArrowBaseRight = pntArrowBase + (arrowWidth_ * vecDirOrtho.toPointF());
    pathArrowSource.addPolygon(QPolygonF() << pntSourceAnchor_ << pntArrowBaseLeft << pntArrowBaseRight << pntSourceAnchor_);
    mapPainterPaths_.insert(nodeSource.label(), pathArrowSource);

    /* Arrow target */
    QPainterPath pathArrowTarget = QPainterPath();
    QVector2D vecDirToSource = QVector2D(lineEdge_.p1() - lineEdge_.p2());
    vecDirToSource.normalize();
    QVector2D vecDirOrthoTarget = QVector2D(QPointF(-lineEdge_.dy(), lineEdge_.dx()));
    vecDirOrthoTarget.normalize();

    QPointF pntArrowBaseTarget = pntTargetAnchor_ + (arrowLength_ * vecDirToSource.toPointF());
    QPointF pntArrowBaseLeftTarget = pntArrowBaseTarget - (arrowWidth_ * vecDirOrthoTarget.toPointF());
    QPointF pntArrowBaseRightTarget = pntArrowBaseTarget + (arrowWidth_ * vecDirOrthoTarget.toPointF());
    pathArrowTarget.addPolygon(QPolygonF() << pntTargetAnchor_ << pntArrowBaseLeftTarget <<
                               pntArrowBaseRightTarget << pntTargetAnchor_);
    mapPainterPaths_.insert(nodeTarget.label(), pathArrowTarget);

    prepareGeometryChange();
}

CMultiEdge CCausalityPanelGraphEdge::multiEdge() const
{
    return multiEdge_;
}

void CCausalityPanelGraphEdge::setMultiEdge(const CMultiEdge &multiEdge)
{
    multiEdge_ = multiEdge;
}

void CCausalityPanelGraphEdge::setDataItem(const QString &strKey, const QVariant &data)
{
        mapData_.insert(strKey, data);
}

QVariant CCausalityPanelGraphEdge::dataItem(const QString &strKey)
{
    return mapData_.value(strKey);
}

QPointF CCausalityPanelGraphEdge::sourceAnchor() const
{
    return pntSourceAnchor_;
}

void CCausalityPanelGraphEdge::setSourceAnchor(const QPointF &pntSourceAnchor)
{
    pntSourceAnchor_ = pntSourceAnchor;
}

QPointF CCausalityPanelGraphEdge::targetAnchor() const
{
    return pntTargetAnchor_;
}

void CCausalityPanelGraphEdge::setTargetAnchor(const QPointF &pntTargetAnchor)
{
    pntTargetAnchor_ = pntTargetAnchor;
}
