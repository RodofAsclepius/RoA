#ifndef CAUSALITYPANELGRAPH_H
#define CAUSALITYPANELGRAPH_H

#include "causalitypanel.h"
#include "applicationdata.h"
#include <QMargins>
#include <QGraphicsView>
#include <QTimer>
#include "multigraph.h"
#include "markovmodel.h"
#include "exgraphicsview/extendedgraphicsview.h"

class CColorAndFontMap;
class CCausalityPanelGraphNode;
class CCausalityPanelGraphEdge;
class CCausalityPanelGraphViewLegend;
class CCausalityPanelGraphViewScene;
class CCausalityPanelGraphViewArrowItem;

class CCausalityPanelGraphView : public CExtendedGraphicsView
{
    Q_OBJECT

public:
    enum Mode { Normal, AddEdge };

    CCausalityPanelGraphView(QWidget *parent = 0);
    virtual ~CCausalityPanelGraphView() override;

    void initialize(const CCausalModel &causalModelGroup, bool bUseMinedGraphForGroup = false);
    void initialize(const CCausalModel &causalModelGroup, const CCausalModel &causalModelComparisonGroup,
                    bool bUseMinedGraphForGroup = false, bool bUseMinedGraphForComparisonGroup = false);

    /* Data */
    CCausalModel causalModelGroup() const;
    void setCausalModelGroup(const CCausalModel &causalModelPrimary);

    CCausalModel causalModelComparisonGroup() const;
    void setCausalModelComparisonGroup(const CCausalModel &causalModelSecondary);

    CMarkovModel markovModel() const;
    void setMarkovModel(const CMarkovModel &markovModel);

    CGraph graphPrimary() const;
    void setGraphPrimary(const CGraph &graphPrimary);

    CGraph graphSecondary() const;
    void setGraphSecondary(const CGraph &graphSecondary);

    /* View settings */
    bool useCausalModelGroupMinedGraph() const;
    void setUseCausalModelGroupMinedGraph(bool value);

    bool useCausalModelComparisonGroupMinedGraph() const;
    void setUseCausalModelComparisonGroupMinedGraph(bool value);

    bool showMarkovBlanketPlus() const;
    void setShowMarkovBlanketPlus(bool showMarkovBlanketPlus);

    int markovBlanketPlusDistance() const;
    void setMarkovBlanketPlusDistance(int markovBlanketPlusDistance);

    void setShowUnconnectedNodes(bool bShow);

    /* UI */
    void clear();
    void updateMultiGraph();
    void saveChanges();
    void updateView();
    void postProcessView();
    void highlightNodes(const QStringList &lstNodeNames);
    void clearHighlights();
    void updateNodePositionsFromCache();
    void storeNodePositionsToCache();

    void topologicalSort();
    void updateLayout(const QString& strLayoutAlgo, const QVariantMap &mapParameters);
    void nodeItemPositionUpdated(CCausalityPanelGraphNode* pNode, QPointF pos);
    void centerTreatmentEffect();

    /* Edit data */
    void addNode(CCausalityPanelGraphNode* pNode);
    void nodeMoved(CCausalityPanelGraphNode* pNode);
    void nodeDoubleClicked(CCausalityPanelGraphNode* pNode);
    void removeNode(CCausalityPanelGraphNode* pNode);
    QVector<CCausalityPanelGraphNode *> nodes() const;
    void clearNodes();

    void addEdgeToCausalModelGroup(CCausalityPanelGraphNode* pNodeSource, CCausalityPanelGraphNode* pNodeTarget);
    void removeEdgeFromCausalModelGroup(CCausalityPanelGraphEdge* pEdge);

    void addEdge(CCausalityPanelGraphEdge* pEdge);
    void removeEdge(CCausalityPanelGraphEdge* pEdge);
    void removeSelectedEdge();
    QVector<CCausalityPanelGraphEdge *> edges() const;
    void clearEdges();

    bool multiGroupEnabled() const;
    void setMultiGroupEnabled(bool bMultiGroupEnabled);

    CCausalityPanelGraphView::Mode mode() const;
    void setMode(const CCausalityPanelGraphView::Mode &mode);

    void startAddEdgeMode(CCausalityPanelGraphNode *pSourceNodeAddEdgeMode);
    void stopAddEdgeMode();

protected:
    mutable QMutex _mutex;
    CColorAndFontMap &mapColorsAndFonts_;
    CCausalityPanelGraphViewScene* pScene_;
    CCausalityPanelGraphViewLegend *pLegend_;

    /* Data */
    CCausalModel causalModelGroup_;
    CCausalModel causalModelComparisonGroup_;
    CMarkovModel markovModel_;

    /* View settings */
    bool bUseCausalModelGroupMinedGraph_ = false;
    bool bUseCausalModelComparisonGroupMinedGraph_ = false;
    bool bShowUnconnectedNodes_ = false;
    bool bShowMarkovBlanketPlus_ = false;
    int iMarkovBlanketPlusDistance_ = 1;
    int iNodeWidth_ = 150;
    int iNodeHeight_ = 40;
    bool bIgnoreNodePositionChanges_ = false;
    bool bMultiGroupEnabled_ = false;
    CCausalityPanelGraphView::Mode mode_ = Normal;
    CCausalityPanelGraphViewArrowItem *pArrowItemAddEdgeMode_ = nullptr;
    CCausalityPanelGraphNode *pSourceNodeAddEdgeMode_ = nullptr;

    /* Geom Data */
    QVector<CCausalityPanelGraphNode*> vecNodes_;
    QVector<CCausalityPanelGraphEdge*> vecEdges_;
    QMap<CNode, CCausalityPanelGraphNode*> mapDataNodes_;
    QMap<CEdge, CCausalityPanelGraphEdge*> mapDataEdges_;
    CCausalityPanelGraphNode *pTreatmentNode_ = nullptr;
    CCausalityPanelGraphNode *pEffectNode_ = nullptr;
    CCausalityPanelGraphEdge *pTreatmentEffectEdge_ = nullptr;
    CGraph graphPrimary_;
    CGraph graphSecondary_;
    CMultiGraph multiGraph_;
    QPoint _pntMousePrev;
    QTimer _timerItemsSettled;
    QMultiMap<CCausalityPanelGraphNode *, CCausalityPanelGraphEdge *> mmapNodeEdges_;
    QVector<QPointF> vecNodePositions_;

    void onPrimaryCausalModelUpdated();
    void onSecondaryCausalModelUpdated();
    void onPrimaryGraphUpdated();
    void onSecondaryGraphUpdated();
    void onMultiGraphUpdated();
    void onMarkovModelUpdated();
    void onViewUpdated();
    void contextMenuEvent(QContextMenuEvent *event) override;

    void updateLegendPosition();
    void updateLegend();
protected slots:
    void zoomIn();
    void zoomOut();
    void onSceneChanged(const QList<QRectF> &region);
    void onSceneSelectionChanged();

    virtual void keyPressEvent(QKeyEvent *event) override;

#if QT_CONFIG(wheelevent)
    virtual void wheelEvent(QWheelEvent *event) override;
#endif

    void scaleView(qreal scaleFactor);
    void scaleViewAnchored(qreal scaleFactor, QPointF point);
    virtual void resizeEvent(QResizeEvent *event) override;

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    void itemsSettledTimeOut();
    void timerEvent(QTimerEvent *event) override;
signals:
    void selectionCleared();
    void causalModelUpdated(const CCausalModel &causalModel);
    void variableSelected(const QString &strVariableName);
};

#endif // CAUSALITYPANELGRAPH_H
