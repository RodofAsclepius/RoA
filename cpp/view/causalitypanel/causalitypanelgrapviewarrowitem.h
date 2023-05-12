#ifndef CAUSALITYPANELGRAPHARROWITEM_H
#define CAUSALITYPANELGRAPHARROWITEM_H

#include <QGraphicsItem>
#include "group.h"
#include "multiedge.h"
#include <QBrush>

class CCausalityPanelGraphViewArrowItem : public QGraphicsItem
{
public:
    CCausalityPanelGraphViewArrowItem();
    virtual ~CCausalityPanelGraphViewArrowItem() override;

    QColor color() const;
    void setColor(const QColor &color);

    QPointF sourceAnchor() const;
    void setSourceAnchor(const QPointF &pntSourceAnchor);

    QPointF targetAnchor() const;
    void setTargetAnchor(const QPointF &pntTargetAnchor);

    void paintArrow(QPainter *painter, const QColor &color1);
protected:
    QPointF pntSourceAnchor_;
    QPointF pntTargetAnchor_;

    QColor colorArrowBorder_ = Qt::black;
    qreal rArrowBorderWidth_ = 2;
    QColor clrArrow_ = Qt::black;

    qreal arrowLength_ = 32;
    qreal arrowWidth_ = 6;
    qreal edgeWidth_ = 4;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // CAUSALITYPANELGRAPHARROWITEM_H
