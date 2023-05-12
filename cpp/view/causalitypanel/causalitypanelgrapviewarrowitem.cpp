#include "causalitypanelgrapviewarrowitem.h"
#include "utils.h"

#include <QGraphicsItem>
#include <qmath.h>
#include <QPainter>
#include <QObject>
#include <QDebug>
#include <QRandomGenerator>
#include <QVector2D>

CCausalityPanelGraphViewArrowItem::CCausalityPanelGraphViewArrowItem()
    : QGraphicsItem()
{
    setZValue(99);
}

CCausalityPanelGraphViewArrowItem::~CCausalityPanelGraphViewArrowItem()
{

}

QRectF CCausalityPanelGraphViewArrowItem::boundingRect() const
{
    qreal penWidth = 1;
    qreal extra = 20*(penWidth + arrowWidth_) / 2.0;

    return QRectF(pntSourceAnchor_, QSizeF(pntTargetAnchor_.x() - pntSourceAnchor_.x(),
                                      pntTargetAnchor_.y() - pntSourceAnchor_.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void CCausalityPanelGraphViewArrowItem::paintArrow(QPainter *painter, const QColor &color1)
{
    QLineF lineEdge = QLineF(pntSourceAnchor_, pntTargetAnchor_);
    QPointF pntSource = lineEdge.p1();
    QPointF pntTarget = lineEdge.p2();
    QVector2D vecDirSourceTarget = QVector2D(pntTarget - pntSource);
    vecDirSourceTarget.normalize();
    QVector2D vecDirOrth = QVector2D(QPointF(-lineEdge.dy(), lineEdge.dx()));
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
    QPen penCurve = QPen(Qt::white, 1, Qt::PenStyle::DotLine);
    colorArrowBorder_ = Qt::white;
    painter->setPen(penCurve);
    painter->setBrush(brush);
    painter->drawPolygon(polyGon);

    /* Arrow target */
    QPen penArrowTarget= QPen(colorArrowBorder_, rArrowBorderWidth_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(penArrowTarget);
    QBrush brushArrowTarget(brush);
    brushArrowTarget.setStyle(Qt::BrushStyle::SolidPattern);
    painter->setBrush(brushArrowTarget);

    QPainterPath pathArrowTarget = QPainterPath();
    QVector2D vecDirToSource = QVector2D(lineEdge.p1() - lineEdge.p2());
    vecDirToSource.normalize();
    QVector2D vecDirOrthoTarget = QVector2D(QPointF(-lineEdge.dy(), lineEdge.dx()));
    vecDirOrthoTarget.normalize();

    QPointF pntArrowBaseTarget = pntTargetAnchor_ + (arrowLength_ * vecDirToSource.toPointF());
    QPointF pntArrowBaseLeftTarget = pntArrowBaseTarget - (arrowWidth_ * vecDirOrthoTarget.toPointF());
    QPointF pntArrowBaseRightTarget = pntArrowBaseTarget + (arrowWidth_ * vecDirOrthoTarget.toPointF());
    painter->drawPolygon(QPolygonF() << pntTargetAnchor_ << pntArrowBaseLeftTarget <<
                               pntArrowBaseRightTarget << pntTargetAnchor_);
}
void CCausalityPanelGraphViewArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    paintArrow(painter, clrArrow_);
}

QPointF CCausalityPanelGraphViewArrowItem::sourceAnchor() const
{
    return pntSourceAnchor_;
}

void CCausalityPanelGraphViewArrowItem::setSourceAnchor(const QPointF &pntSourceAnchor)
{
    pntSourceAnchor_ = pntSourceAnchor;
    update();
}

QPointF CCausalityPanelGraphViewArrowItem::targetAnchor() const
{
    return pntTargetAnchor_;
}

void CCausalityPanelGraphViewArrowItem::setTargetAnchor(const QPointF &pntTargetAnchor)
{
    pntTargetAnchor_ = pntTargetAnchor;
    update();
}
