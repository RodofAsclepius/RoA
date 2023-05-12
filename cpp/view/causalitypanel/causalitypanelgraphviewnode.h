#ifndef CCAUSALITYPANELGRAPHNODE_H
#define CCAUSALITYPANELGRAPHNODE_H

#include <QGraphicsItem>
#include <QBrush>
#include <QFont>
#include "variable.h"
#include "group.h"

static const int NODE_DATA_VARIABLE = 0;

class CMainController;
class CCausalityPanelGraphView;
class CCausalityPanelGraphEdge;
class CColorAndFontMap;

class CCausalityPanelGraphNode : public QGraphicsItem
{
public:
    CCausalityPanelGraphNode(CCausalityPanelGraphView *pGraph, QGraphicsItem* pParentItem = nullptr);
    virtual ~CCausalityPanelGraphNode() override;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
//    virtual QRectF textRect() const;

    virtual QPointF edgeInAnchor(const QPointF &pntTarget) const;
    virtual QPointF edgeOutAnchor(const QPointF &pntTarget) const;

    virtual void addEdgeOut(CCausalityPanelGraphEdge* pEdge);
    virtual void removeEdgeOut(CCausalityPanelGraphEdge* pEdge);
    virtual QList<CCausalityPanelGraphEdge*> edgesOut() const;
    virtual void clearEdgesOut();

    virtual QList<CCausalityPanelGraphEdge *> edgesIn() const;
    virtual void addEdgeIn(CCausalityPanelGraphEdge* pEdge);
    virtual void removeEdgeIn(CCausalityPanelGraphEdge* pEdge);
    virtual void clearEdgesIn();

    virtual void clearEdges();

    QSize size() const;
    void setSize(const QSize &size);

    bool visited() const;
    void setVisited(bool visited);

    CVariable variable() const;
    void setVariable(const CVariable &variable);

    CGroup group() const;
    void setGroup(const CGroup &group);

    bool isTreatment() const;
    void setIsTreatment(bool value);

    bool isEffect() const;
    void setIsEffect(bool value);

    void calculateForces();
    bool advancePosition();

    bool IsInMarkovSet() const;
    void setIsInMarkovSet(bool IsInMarkovSet);

    bool IsInMarkovPlusSet() const;
    void setIsInMarkovPlusSet(bool IsInMarkovPlusSet);

    bool highlighted() const;
    void setIsHighlighted(bool highlighted);

    QString text() const;
    void setText(const QString &text);

    bool isInMinedGraph() const;
    void setIsInMinedGraph(bool bIsInMinedGraph);

    void setPos(const QPointF &pos);
    inline void setPos(qreal x, qreal y);

protected:
    CColorAndFontMap &mapColorsAndFonts_;
    CCausalityPanelGraphView * _pGraphView;
    bool bVisited_;
    CVariable variable_;
    CGroup group_;
    bool bIsTreatment_ = false;
    bool bIsEffect_ = false;
    bool bIsInMarkovSet_ = false;
    bool bIsInMarkovPlusSet_ = true;
    bool bIsHighlighted_ = false;
    bool bIsInMinedGraph_ = false;
    QPointF newPos;
    QColor clrOnPath_ = Qt::red;
    QString text_;

    QPointF _pntPreviousPosition;
    QFont font_ = QFont("Liberation Serif", 12);
    qreal scaleFactor = 1.0;
    QSize _size = QSize(scaleFactor*125, scaleFactor*60);
    int iDefaultTextSize_ = 14;
    int iSpecialTextSize_ = 26;

    /* UI */
    bool _bItemMoved = false;

    QList<CCausalityPanelGraphEdge*> _lstEdgesOut;
    QList<CCausalityPanelGraphEdge*> _lstEdgesIn;

    void fitToText();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // CCAUSALITYPANELGRAPHNODE_H
