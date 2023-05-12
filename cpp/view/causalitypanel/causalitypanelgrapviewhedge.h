#ifndef CAUSALITYPANELGRAPHEDGE_H
#define CAUSALITYPANELGRAPHEDGE_H

#include <QGraphicsItem>
#include "group.h"
#include "multiedge.h"

class CCausalityPanelGraphNode;

class CCausalityPanelGraphEdge : public QGraphicsItem
{
public:
    CCausalityPanelGraphEdge(CCausalityPanelGraphNode *sourceNode, CCausalityPanelGraphNode *targetNode,
                             const QString& strlabel = QString());
    virtual ~CCausalityPanelGraphEdge() override;

    CCausalityPanelGraphNode *sourceNode() const;
    CCausalityPanelGraphNode *targetNode() const;

    QString label() const;
    void setLabel(const QString &label);

    QColor color() const;
    void setColor(const QColor &color);

    QPainterPath lines() const;
    void setLines(const QPainterPath &lines);

    void renderGeometry();

    CMultiEdge multiEdge() const;
    void setMultiEdge(const CMultiEdge &multiEdge);

    void setDataItem(const QString &strKey, const QVariant &data);
    QVariant dataItem(const QString &strKey);

    QPointF sourceAnchor() const;
    void setSourceAnchor(const QPointF &pntSourceAnchor);

    QPointF targetAnchor() const;
    void setTargetAnchor(const QPointF &pntTargetAnchor);

    QPainterPath shape() const override;
protected:
    CCausalityPanelGraphNode *pSourceNode_;
    CCausalityPanelGraphNode *pTargetNode_;

    QPointF pntSourceAnchor_;
    QPointF pntTargetAnchor_;

    CMultiEdge multiEdge_;
    QMap<QString, QVariant> mapData_;
    QLineF lineEdge_;
    QMap<QString, QPainterPath> mapPainterPaths_;
    QPainterPath pathEdge_;
    QPainterPath pathArrow_;
    QColor colorArrowBorder_ = Qt::transparent;
    qreal rArrowBorderWidth_ = 1;
    QColor clrMinedGraph_ = Qt::darkGray;//QColor(224, 224, 224);
//    QColor clrMinedGraph_ = Qt::black;//QColor(224, 224, 224);
    QColor clrEdgesAgree_ = Qt::white;

    qreal arrowLength_ = 32;
    qreal arrowWidth_ = 8;
    qreal edgeWidth_ = 4;
    QString strLabel_;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void paintArrow(QPainter *painter, const QString &strTargetLabel, const QColor &color, const QColor &colorBorder = Qt::white);
    void paintBiDirectionalArrow(QPainter *painter, const QString &strTargetLabel, const QString &strTargetLabel2,
                                 const QColor &color, const QColor &color2, const QColor &colorBorder = Qt::white);

    QList<QPointF> computeAnchorPoints(const CCausalityPanelGraphNode* pNode) const;
    virtual void keyPressEvent(QKeyEvent *event) override;

    bool bConnectsToHighlightedNode() const;
};

#endif // CAUSALITYPANELGRAPHEDGE_H

