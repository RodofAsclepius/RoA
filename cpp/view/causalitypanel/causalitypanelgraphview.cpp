#include "causalitypanelgraphview.h"
#include "causalitypanelgraphviewnode.h"
#include "causalitypanelgrapviewhedge.h"
#include "utils.h"
#include "colorandfontmap.h"
#include "propertiesinterface.h"
#include <QMutexLocker>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include <QStack>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QtMath>

#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/planarity/PlanarSubgraphFast.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/SubgraphPlanarizer.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>

#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>

#include <ogdf/energybased/FMMMLayout.h>
#include <causalitypanelgraphviewlegend.h>
#include <causalitypanelgraphviewscene.h>
#include "maincontroller.h"
#include "causalitypanelgrapviewarrowitem.h"

CCausalityPanelGraphView::CCausalityPanelGraphView(QWidget *parent)
    : CExtendedGraphicsView(),
      mapColorsAndFonts_(CColorAndFontMap::instance()),
      pScene_(new CCausalityPanelGraphViewScene(0, 0, 99, 99, this)),
      pLegend_(new CCausalityPanelGraphViewLegend(this))
{

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    pScene_->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    setViewportUpdateMode(FullViewportUpdate);
    setScene(pScene_);
    connect(pScene_, &QGraphicsScene::changed, this, &CCausalityPanelGraphView::onSceneChanged);
    connect(pScene_, &QGraphicsScene::selectionChanged, this, &CCausalityPanelGraphView::onSceneSelectionChanged);

    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::TextAntialiasing);
    setCacheMode(CacheBackground);
    setFocusPolicy(Qt::StrongFocus);

    QColor clrBackGround = mapColorsAndFonts_.getColor("widget/chart");
    setBackgroundBrush(clrBackGround);
    setMouseTracking(true);

    connect(&_timerItemsSettled, &QTimer::timeout, this, &CCausalityPanelGraphView::itemsSettledTimeOut);
    connect(&_timerItemsSettled, &QTimer::timeout, this, &CCausalityPanelGraphView::itemsSettledTimeOut);

    pLegend_->show();
}

CCausalityPanelGraphView::~CCausalityPanelGraphView()
{

}

void CCausalityPanelGraphView::initialize(const CCausalModel &causalModelGroup, bool bUseMinedGraphForGroup)
{
    setUseCausalModelGroupMinedGraph(bUseMinedGraphForGroup);
    setCausalModelGroup(causalModelGroup);
}

void CCausalityPanelGraphView::initialize(const CCausalModel &causalModelGroup, const CCausalModel &causalModelComparisonGroup,
                                          bool bUseMinedGraphForGroup, bool bUseMinedGraphForComparisonGroup)
{
    setUseCausalModelGroupMinedGraph(bUseMinedGraphForGroup);
    setUseCausalModelComparisonGroupMinedGraph(bUseMinedGraphForComparisonGroup);
    setCausalModelGroup(causalModelGroup);
    setCausalModelComparisonGroup(causalModelComparisonGroup);
}

void CCausalityPanelGraphView::addNode(CCausalityPanelGraphNode *pNode)
{
//    DEBUG_OBJ_INFO

//    pNode->setSize(QSize(iNodeWidth_, iNodeHeight_));
    pNode->setGroup(causalModelGroup_.group());
    scene()->addItem(pNode);
    vecNodes_.append(pNode);
}

void CCausalityPanelGraphView::clear()
{
    scene()->clear();
    clearNodes();
    clearEdges();
    mmapNodeEdges_.clear();
}

QVector<CCausalityPanelGraphNode *> CCausalityPanelGraphView::nodes() const
{
    return vecNodes_;
}

void CCausalityPanelGraphView::clearNodes()
{
    vecNodes_.clear();
    mapDataNodes_.clear();
}

void CCausalityPanelGraphView::addEdgeToCausalModelGroup(CCausalityPanelGraphNode *pNodeSource, CCausalityPanelGraphNode *pNodeTarget)
{
//    DEBUG_OBJ_INFO
    CNode node1 = pNodeSource->data(NODE_DATA_VARIABLE).value<CNode>();
    CNode node2 = pNodeTarget->data(NODE_DATA_VARIABLE).value<CNode>();

    CGraph graphUpdated = bUseCausalModelGroupMinedGraph_ ? causalModelGroup_.graphAlg() : causalModelGroup_.graphUser();
    CEdge edge(node1, node2);
    graphUpdated.addEdge(edge);

    if(bUseCausalModelGroupMinedGraph_)
    {
        causalModelGroup_.setGraphAlg(graphUpdated);
    }
    else
    {
        causalModelGroup_.setGraphUser(graphUpdated);
    }

    onPrimaryCausalModelUpdated();
    emit causalModelUpdated(causalModelGroup_);
}

void CCausalityPanelGraphView::removeEdgeFromCausalModelGroup(CCausalityPanelGraphEdge *pEdge)
{
    CNode node1 = pEdge->sourceNode()->data(NODE_DATA_VARIABLE).value<CNode>();
    CNode node2 = pEdge->targetNode()->data(NODE_DATA_VARIABLE).value<CNode>();

    CGraph graphUpdated = bUseCausalModelGroupMinedGraph_ ? causalModelGroup_.graphAlg() : causalModelGroup_.graphUser();
//    CEdge edge(node1, node2);
    graphUpdated.removeEdge(node1, node2);

    if(bUseCausalModelGroupMinedGraph_)
    {
        causalModelGroup_.setGraphAlg(graphUpdated);
    }
    else
    {
        causalModelGroup_.setGraphUser(graphUpdated);
    }

    onPrimaryCausalModelUpdated();
    emit causalModelUpdated(causalModelGroup_);
}

void CCausalityPanelGraphView::clearEdges()
{
    vecEdges_.clear();
    mapDataEdges_.clear();
}

bool CCausalityPanelGraphView::multiGroupEnabled() const
{
    return bMultiGroupEnabled_;
}

void CCausalityPanelGraphView::setMultiGroupEnabled(bool bMultiGroupEnabled)
{
    bMultiGroupEnabled_ = bMultiGroupEnabled;

    updateLegend();
}

CCausalityPanelGraphView::Mode CCausalityPanelGraphView::mode() const
{
    return mode_;
}

void CCausalityPanelGraphView::setMode(const CCausalityPanelGraphView::Mode &mode)
{
    mode_ = mode;
}

void CCausalityPanelGraphView::startAddEdgeMode(CCausalityPanelGraphNode *pSourceNodeAddEdgeMode)
{
//    DEBUG_OBJ_INFO

    pSourceNodeAddEdgeMode_ = pSourceNodeAddEdgeMode;
    if(pArrowItemAddEdgeMode_)
    {
        pArrowItemAddEdgeMode_->show();
    }

    QList<QGraphicsItem*> lstItems = scene()->items();
    if(!lstItems.contains(pArrowItemAddEdgeMode_))
        scene()->addItem(pArrowItemAddEdgeMode_);

    mode_ = AddEdge;
}

void CCausalityPanelGraphView::stopAddEdgeMode()
{
//    DEBUG_OBJ_INFO
    mode_ = Normal;

    pArrowItemAddEdgeMode_->hide();
}

void CCausalityPanelGraphView::addEdge(CCausalityPanelGraphEdge *pEdge)
{
    scene()->addItem(pEdge);
    vecEdges_.append(pEdge);
    mmapNodeEdges_.insert(pEdge->sourceNode(), pEdge);
    pEdge->renderGeometry();
}

void CCausalityPanelGraphView::removeEdge(CCausalityPanelGraphEdge *pEdge)
{
    scene()->removeItem(pEdge);
    vecEdges_.removeOne(pEdge);
    mmapNodeEdges_.remove(pEdge->sourceNode(), pEdge);
}

void CCausalityPanelGraphView::removeSelectedEdge()
{
    QList<QGraphicsItem*> lstSelectedItems = scene()->selectedItems();
    if(!lstSelectedItems.isEmpty())
    {
        CCausalityPanelGraphEdge* pSelectedEdge = dynamic_cast<CCausalityPanelGraphEdge*>(lstSelectedItems.first());

        if(pSelectedEdge)
            removeEdgeFromCausalModelGroup(pSelectedEdge);
    }

    repaint();
}

void CCausalityPanelGraphView::itemsSettledTimeOut()
{
    repaint();
}

void CCausalityPanelGraphView::keyPressEvent(QKeyEvent *event)
{
//    DEBUG_OBJ_INFO

    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Delete:
            removeSelectedEdge();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

#if QT_CONFIG(wheelevent)
void CCausalityPanelGraphView::wheelEvent(QWheelEvent *event)
{
//    scaleView(pow((double)2, event->delta() / 240.0));
    qreal rValue = qPow<qreal, qreal>(2, event->angleDelta().ry() / 360.0);
    scaleViewAnchored(rValue, event->scenePosition());
}
#endif

void CCausalityPanelGraphView::scaleView(qreal scaleFactor)
{
    qreal  rScaleFactor_ = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
//    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (rScaleFactor_ < 0.07 || rScaleFactor_ > 100)
        return;

    scale(rScaleFactor_, rScaleFactor_);
}

void CCausalityPanelGraphView::scaleViewAnchored(qreal scaleFactor, QPointF point)
{
    if (scaleFactor < 0.07 || scaleFactor > 100)
        return;

    scale(scaleFactor, scaleFactor);
    QPointF targetPos = mapToScene(point.toPoint());
    centerOn(targetPos);
    QPointF delta_viewport_pos = targetPos - QPointF(viewport()->width() / 2.0, viewport()->height() / 2.0);
    QPointF viewport_center = mapFromScene(targetPos) - delta_viewport_pos;
    centerOn(mapToScene(viewport_center.toPoint()));
}

void CCausalityPanelGraphView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);

    updateLegendPosition();
}

void CCausalityPanelGraphView::updateLegendPosition()
{
    /* Set position */
    QPoint pntLegend = QPoint(rect().right() - pLegend_->width() -20, rect().bottom() - pLegend_->height()-20); // Bottom right
//    QPoint pntLegend = QPoint(rect().right() - pLegend_->width() -20, 20); // Top right
//    QPoint pntLegend = QPoint(20, rect().bottom() - pLegend_->height()-20); // Bottom right // Bottom left
    pLegend_->move(pntLegend);
}

void CCausalityPanelGraphView::updateLegend()
{
    CGroup group1 = causalModelGroup().group();
    CGroup group2 = causalModelComparisonGroup().group();

    pLegend_->setGroup1(group1);
    pLegend_->setGroup2(group2);

//    bool bMultiGroupEnabled = group1.name() != group2.name();
    pLegend_->setMultipGroupEnabled(bMultiGroupEnabled_);

    pLegend_->setFirstGroupIsMinedNetwork(useCausalModelGroupMinedGraph());
    pLegend_->setSecondGroupIsMinedNetwork(useCausalModelComparisonGroupMinedGraph());

    updateLegendPosition();
}

void CCausalityPanelGraphView::zoomIn()
{
    scaleView(1.01);
}

void CCausalityPanelGraphView::zoomOut()
{
    scaleView(1 / qreal(1.01));
}

void CCausalityPanelGraphView::onSceneChanged(const QList<QRectF> &region)
{
//    DEBUG_OBJ_INFO

//    QRectF rectBoundingScene = scene()->itemsBoundingRect();
//    rectScene.adjust(rectScene.width(), rectScene.height(), rectScene.width(), rectScene.height());
//    rectBoundingScene.adjust(0, 0, -3*rectBoundingScene.width(), -3*rectBoundingScene.height());
    //    setSceneRect(rectBoundingScene);
}

void CCausalityPanelGraphView::onSceneSelectionChanged()
{
//    DEBUG_OBJ_INFO

//    QList<QGraphicsItem*> lstItems = scene()->selectedItems();
//    qDebug() << lstItems;
}

void CCausalityPanelGraphView::mousePressEvent(QMouseEvent* event)
{
    switch (mode_)
    {
        case AddEdge:
        {
            if(pArrowItemAddEdgeMode_ != nullptr)
            {
                QList<QGraphicsItem *> endItems = scene()->items(mapToScene(event->pos()));
                if(!endItems.isEmpty())
                {
                    if (endItems.first() == pArrowItemAddEdgeMode_)
                        endItems.removeFirst();

                    if(!endItems.isEmpty())
                    {
                        QGraphicsItem * pGraphItemEnd = endItems.first();
                        CCausalityPanelGraphNode *pNodeTarget = dynamic_cast<CCausalityPanelGraphNode *>(pGraphItemEnd);
                        if(pNodeTarget)
                        {
                            addEdgeToCausalModelGroup(pSourceNodeAddEdgeMode_, pNodeTarget);
                        }
                    }
                }
            }
            stopAddEdgeMode();
        }
        break;
        case Normal:
        {
            if (event->button() == Qt::MiddleButton)
            {
                fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
            }

            if (event->button() == Qt::RightButton)
            {
                _pntMousePrev = event->pos();
                setCursor(Qt::DragMoveCursor);
            }

            if (event->button() == Qt::LeftButton)
            {
                _pntMousePrev = event->pos();
                setCursor(Qt::ArrowCursor);
            }
            QGraphicsView::mousePressEvent(event);
        }
        break;
    }
}

void CCausalityPanelGraphView::mouseMoveEvent(QMouseEvent* event)
{
//    DEBUG_OBJ_INFO
    setCursor(Qt::ArrowCursor);

    switch (mode_)
    {
        case AddEdge:
        {
            if (pSourceNodeAddEdgeMode_ != nullptr)
            {
                QPointF pntTarget = mapToScene(event->pos());
                QPointF pntSource = pSourceNodeAddEdgeMode_->edgeOutAnchor(pntTarget);

                /* Check if node is hit ... */
                QList<QGraphicsItem *> endItems = scene()->items(pntTarget);
                if (endItems.count() && endItems.first() == pArrowItemAddEdgeMode_)
                    endItems.removeFirst();
                if(!endItems.isEmpty())
                {
                    CCausalityPanelGraphNode *pTargetNode = dynamic_cast<CCausalityPanelGraphNode*>(endItems.first());
                    if(pTargetNode)
                    {
                        pntTarget = pTargetNode->edgeInAnchor(pntTarget);
                    }
                }

                pArrowItemAddEdgeMode_->setSourceAnchor(pntSource);
                pArrowItemAddEdgeMode_->setTargetAnchor(pntTarget);
            }
        }
        break;
        case Normal:
        {
            if (event->buttons() & Qt::RightButton)
            {
                setCursor(Qt::DragMoveCursor);
                QPointF oldp = _pntMousePrev;
                QPointF newP = event->pos();
                QPointF translation = newP - oldp;

                qreal rVAcceleration = 1 * 1;
                qreal rHAcceleration = 1;//.6 * rVAcceleration;
                qreal rDeltaX = translation.x() * rHAcceleration*2;
                qreal rDeltaY = translation.y() * rVAcceleration;
                doTranslate(rDeltaX, rDeltaY);

                _pntMousePrev = event->pos();
            }
        }
        break;

    default:
        ;
    }



    QGraphicsView::mouseMoveEvent(event);
}

void CCausalityPanelGraphView::updateLayout(const QString &strLayoutAlgo, const QVariantMap &mapParameters)
{
//    DEBUG_OBJ_INFO
    try {
        ogdf::Graph graphResult;

        QMap<int, ogdf::node> mapNodes;
        for(int i=0; i<vecNodes_.count(); ++i)
        {
            mapNodes.insert(i, graphResult.newNode(i));
        }

        ogdf::GraphAttributes GA(graphResult, ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::nodeType |
             ogdf::GraphAttributes::edgeGraphics | ogdf::GraphAttributes::edgeType | ogdf::GraphAttributes::nodeTemplate);

        for(ogdf::node v : graphResult.nodes)
        {
            GA.width(v) = iNodeWidth_;
            GA.height(v) = iNodeHeight_;
        }

        QMap<int, ogdf::edge> mapEdges;
        for(int iEdgeIndex=0; iEdgeIndex < vecEdges_.count(); ++iEdgeIndex)
        {
            CCausalityPanelGraphEdge *pEdge = vecEdges_.at(iEdgeIndex);
            CCausalityPanelGraphNode *pSourceNode = pEdge->sourceNode();
            CCausalityPanelGraphNode *pTargetNode = pEdge->targetNode();
            int iSourceIndex = vecNodes_.indexOf(pSourceNode);
            int iTargetIndex = vecNodes_.indexOf(pTargetNode);

            ogdf::node nodeSource = mapNodes.value(iSourceIndex);
            ogdf::node nodeTarget = mapNodes.value(iTargetIndex);

            ogdf::edge e = graphResult.newEdge(nodeSource, nodeTarget, iEdgeIndex);
            mapEdges.insert(iEdgeIndex, e);
        }

        /* Ortho layout */
        if(strLayoutAlgo == "Orthogonally")
        {
            int iRuns = mapParameters.value("runs", 100).toInt();
            qreal rSeperation = mapParameters.value("seperation", 60.0).toDouble();
            qreal coverhang = mapParameters.value("seperation", 0.4).toDouble();
            int iNodecenter2bendsCount = mapParameters.value("runs", 3).toInt();

            ogdf::PlanarizationLayout pl;
            ogdf::SubgraphPlanarizer crossMin;
            auto* ps = new ogdf::PlanarSubgraphFast<int>;
            ps->runs(iRuns);
            ogdf::VariableEmbeddingInserter *ves = new ogdf::VariableEmbeddingInserter;
            ves->removeReinsert(ogdf::RemoveReinsertType::All);
            crossMin.setSubgraph(ps);
            crossMin.setInserter(ves);
            ogdf::EmbedderMinDepthMaxFaceLayers *emb = new ogdf::EmbedderMinDepthMaxFaceLayers;
            pl.setEmbedder(emb);
            ogdf::OrthoLayout *ol = new ogdf::OrthoLayout;
            ol->separation(rSeperation);
            ol->cOverhang(coverhang);
            pl.setPlanarLayouter(ol);
            pl.call(GA);
            GA.addNodeCenter2Bends(iNodecenter2bendsCount);
            GA.removeUnnecessaryBendsHV();
        }

        /* Sugiyama layout */
        if(strLayoutAlgo == "Sugiyama")
        {
            qreal rLayerdistance = mapParameters.value("layerdistance", 60.0).toDouble();
            qreal rNodedistance = mapParameters.value("nodedistance", 50.0).toDouble();
            qreal rWeightBalancing = mapParameters.value("weightbalancing", 0.8).toDouble();

            ogdf::SugiyamaLayout SL;
            SL.setRanking(new ogdf::OptimalRanking);
            SL.setCrossMin(new ogdf::MedianHeuristic);
            ogdf::OptimalHierarchyLayout *ohl = new ogdf::OptimalHierarchyLayout;
            ohl->layerDistance(rLayerdistance);
            ohl->nodeDistance(rNodedistance);
            ohl->weightBalancing(rWeightBalancing);
            SL.setLayout(ohl);
            SL.call(GA);
        }

        /* Energy based */
        if(strLayoutAlgo == "Energy")
        {
            qreal rUnitedgelength_energy = mapParameters.value("unitedgelength", 75.0).toDouble();
            bool bUsehighleveloptions_energy = mapParameters.value("usehighleveloptions", true).toBool();
            bool bInitialPlacement_energy = mapParameters.value("newinitialplacement", true).toBool();

            ogdf::FMMMLayout fmmm;
            fmmm.useHighLevelOptions(bUsehighleveloptions_energy);
            fmmm.unitEdgeLength(rUnitedgelength_energy);
            fmmm.newInitialPlacement(bInitialPlacement_energy);
            fmmm.qualityVersusSpeed(ogdf::FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);
            fmmm.call(GA);
        }
    //    GA.node

        qreal scaleFactor = 1.0;
        GA.scale(scaleFactor, scaleFactor);

        /* Store result in causal model */
        for(ogdf::node v : graphResult.nodes)
        {
            vecNodes_.at(v->index())->setPos(QPointF(GA.x(v), GA.y(v)));
        }

//        for(ogdf::edge e : graphResult.edges)
//        {
//            GA.e
//            QPainterPath pathBends;

//            ogdf::DPolyline polyline = GA.bends(e);
//            QPointF start = QPointF(GA.x(e->source()), GA.y(e->source()));
//            QPointF end = QPointF(GA.x(e->target()), GA.y(e->target()));

//            pathBends.moveTo(start);
////            for(ogdf::DPoint p : polyline)
////            {
////                pathBends.lineTo(p.m_x, p.m_y);
////            }
////            pathBends.lineTo(end);
//            pathBends.cubicTo(end + QPointF(10, 0), end + QPointF(0, 10), end);

//            vecEdges_.at(e->index())->setLines(pathBends);
//        }
    } catch (ogdf::AlgorithmFailureException e)
    {
        qDebug() << QString::number((int)e.exceptionCode());
    }

//    foreach(CCausalityPanelGraphEdge* pEdge, vecEdges_)
//    {
//        pEdge->adjust();
    //    }
}

void CCausalityPanelGraphView::nodeItemPositionUpdated(CCausalityPanelGraphNode* pNode, QPointF pos)
{
//    DEBUG_OBJ_INFO
    if(!bIgnoreNodePositionChanges_)
    {
        storeNodePositionsToCache();
    }
}

CGraph CCausalityPanelGraphView::graphPrimary() const
{
    return graphPrimary_;
}

void CCausalityPanelGraphView::setGraphPrimary(const CGraph &graphPrimary)
{
    graphPrimary_ = graphPrimary;
}

CGraph CCausalityPanelGraphView::graphSecondary() const
{
    return graphSecondary_;
}

void CCausalityPanelGraphView::setGraphSecondary(const CGraph &graphSecondary)
{
    graphSecondary_ = graphSecondary;
}

void CCausalityPanelGraphView::onPrimaryCausalModelUpdated()
{
//    DEBUG_OBJ_INFO

    graphPrimary_ = bUseCausalModelGroupMinedGraph_ ? causalModelGroup_.graphAlg() : causalModelGroup_.graphUser();
    graphPrimary_.setDataItem("group", QVariant::fromValue(causalModelGroup_.group()));
    graphPrimary_.setDataItem("isMinedGraph", QVariant::fromValue(bUseCausalModelGroupMinedGraph_));
    onPrimaryGraphUpdated();
}

void CCausalityPanelGraphView::onSecondaryCausalModelUpdated()
{
//    DEBUG_OBJ_INFO
    graphSecondary_ = bUseCausalModelComparisonGroupMinedGraph_ ? causalModelComparisonGroup_.graphAlg() : causalModelComparisonGroup_.graphUser();
    graphSecondary_.setDataItem("group", QVariant::fromValue(causalModelComparisonGroup_.group()));
    graphSecondary_.setDataItem("isMinedGraph", QVariant::fromValue(bUseCausalModelComparisonGroupMinedGraph_));
    onSecondaryGraphUpdated();
}

void CCausalityPanelGraphView::onPrimaryGraphUpdated()
{
    updateMultiGraph();
}

void CCausalityPanelGraphView::onSecondaryGraphUpdated()
{
    updateMultiGraph();
}

void CCausalityPanelGraphView::onMultiGraphUpdated()
{

}

void CCausalityPanelGraphView::onMarkovModelUpdated()
{
//    DEBUG_OBJ_INFO
}

void CCausalityPanelGraphView::onViewUpdated()
{
   centerTreatmentEffect();
}

CCausalModel CCausalityPanelGraphView::causalModelGroup() const
{
    return causalModelGroup_;
}

void CCausalityPanelGraphView::setCausalModelGroup(const CCausalModel &causalModelPriamry)
{
//    DEBUG_OBJ_INFO

    causalModelGroup_ = causalModelPriamry;

    onPrimaryCausalModelUpdated();
}

CCausalModel CCausalityPanelGraphView::causalModelComparisonGroup() const
{
    return causalModelComparisonGroup_;
}

void CCausalityPanelGraphView::setCausalModelComparisonGroup(const CCausalModel &causalModelSecondary)
{
//    DEBUG_OBJ_INFO

    causalModelComparisonGroup_ = causalModelSecondary;

    onSecondaryCausalModelUpdated();
}

bool CCausalityPanelGraphView::useCausalModelGroupMinedGraph() const
{
    return bUseCausalModelGroupMinedGraph_;
}

void CCausalityPanelGraphView::setUseCausalModelGroupMinedGraph(bool value)
{
    bUseCausalModelGroupMinedGraph_ = value;
    updateMultiGraph();
}

bool CCausalityPanelGraphView::useCausalModelComparisonGroupMinedGraph() const
{
    return bUseCausalModelComparisonGroupMinedGraph_;
}

void CCausalityPanelGraphView::setUseCausalModelComparisonGroupMinedGraph(bool value)
{
    bUseCausalModelComparisonGroupMinedGraph_ = value;
    updateMultiGraph();
}

void CCausalityPanelGraphView::updateMultiGraph()
{
//    DEBUG_OBJ_INFO

    multiGraph_.clear();

    QVariantList lstVariants1;
    lstVariants1.append(graphPrimary_.dataItem("group"));
    lstVariants1.append(graphPrimary_.dataItem("isMinedGraph"));
    multiGraph_.importGraph(graphPrimary_, QVariant::fromValue(lstVariants1));
    multiGraph_.setDataItem("primaryGroup", graphPrimary_.dataItem("group"));

    QVariantList lstVariants2;
    lstVariants2.append(graphSecondary_.dataItem("group"));
    lstVariants2.append(graphSecondary_.dataItem("isMinedGraph"));
    multiGraph_.importGraph(graphSecondary_, QVariant::fromValue(lstVariants2));
    multiGraph_.setDataItem("secondaryGroup", graphSecondary_.dataItem("group"));

    updateView();
}

void CCausalityPanelGraphView::updateView()
{
//    DEBUG_OBJ_INFO

    clear();
    bIgnoreNodePositionChanges_ = true;
//    QColor clrBackGround = mapColorsAndFonts_.getColor("widget/chart");
//    setBackgroundBrush(clrBackGround);

    /* Add selection arrow */
    pArrowItemAddEdgeMode_ = new CCausalityPanelGraphViewArrowItem();
    scene()->addItem(pArrowItemAddEdgeMode_);

    QList<CNode> lstMultiGraphNodes = multiGraph_.nodes();

    /* Place nodes */
    qreal x = 0, y = 0, dx = 100, dy = 0;
    qreal yOffset = scene()->sceneRect().center().ry();
    qreal xOffset = scene()->sceneRect().center().rx() +(lstMultiGraphNodes.count()/2) * dx ;
    foreach(CNode nodePrimary, lstMultiGraphNodes)
    {

        CCausalityPanelGraphNode* pNodeGraphNew = new CCausalityPanelGraphNode(this);
        pNodeGraphNew->setText(nodePrimary.label());
        pNodeGraphNew->setPos(QPointF(xOffset, yOffset) + QPointF(x, y));
        pNodeGraphNew->setData(NODE_DATA_VARIABLE, QVariant::fromValue(nodePrimary));
        addNode(pNodeGraphNew);
        pNodeGraphNew->setZValue(2);

        mapDataNodes_.insert(nodePrimary, pNodeGraphNew);
        x += dx;
    }

    /* Add Edges */
    QList<CMultiEdge> multiEdges = multiGraph_.multiEdges();
    foreach(CMultiEdge multiEdge, multiEdges)
    {
        QList<CEdge> edges = multiEdge.edges();

        if(!edges.isEmpty())
        {
            CEdge edgePrimary = edges.first();
            CNode nodeSource = edgePrimary.sourceNode();
            CNode nodeTarget = edgePrimary.targetNode();

            if(mapDataNodes_.contains(edgePrimary.sourceNode()) && mapDataNodes_.contains(nodeTarget))
            {
                CCausalityPanelGraphNode* pNodeGraphSource = mapDataNodes_.value(nodeSource);
                CCausalityPanelGraphNode* pNodeGraphTarget = mapDataNodes_.value(nodeTarget);

                CCausalityPanelGraphEdge *pNewGraphEdge = new CCausalityPanelGraphEdge(pNodeGraphSource, pNodeGraphTarget);
                pNewGraphEdge->setMultiEdge(multiEdge);
                pNewGraphEdge->setDataItem("primaryGroup", multiGraph_.dataItem("primaryGroup"));
                pNewGraphEdge->setDataItem("secondaryGroup", multiGraph_.dataItem("secondaryGroup"));
                addEdge(pNewGraphEdge);
                pNewGraphEdge->setZValue(1);

                mapDataEdges_.insert(edgePrimary, pNewGraphEdge);
            }
            else
            {

            }
        }
    }

    /* Read node positions from cache */
    updateNodePositionsFromCache();

    postProcessView();

    updateLegend();

    bIgnoreNodePositionChanges_ = false;
}

void CCausalityPanelGraphView::postProcessView()
{
    /* Make all edges visible */
    foreach(CCausalityPanelGraphEdge *pEdge, edges())
    {
        pEdge->setVisible(true);
    }

    /* Hide nodes using Markov Blanket Plus ? */
    if(bShowMarkovBlanketPlus_)
    {
        /* Show plus */
        CVariablesList lstVariablesMBPlus = markovModel_.markovBlanketPlus(iMarkovBlanketPlusDistance_);
        QStringList lstNamesMBPlus = lstVariablesMBPlus.variablesNames();
        foreach(CCausalityPanelGraphNode *pNode, nodes())
        {
            /* In MB+ */
            if(lstNamesMBPlus.contains(pNode->text()))
            {
                pNode->setIsInMarkovPlusSet(true);
                pNode->setVisible(true);
                pNode->update();
            }
            else
            {
                pNode->setIsInMarkovPlusSet(false);
                pNode->setVisible(false);
                pNode->update();
            }
        }

        /* In MB */
        CVariablesList lstVariablesMB = markovModel_.markovBlanket();
        QStringList lstNamesMB = lstVariablesMB.variablesNames();
        foreach(CCausalityPanelGraphNode *pNode, nodes())
        {
            /* In MB+ */
            if(lstNamesMB.contains(pNode->text()))
            {
                pNode->setIsInMarkovSet(true);
                pNode->setVisible(true);
                pNode->update();
            }
            else
            {
                pNode->setIsInMarkovSet(false);
                pNode->update();
//                pNode->setVisible(false);
            }
        }
    }
    else
    {
        /* Full network */
        foreach(CCausalityPanelGraphNode *pNode, nodes())
        {
            pNode->setVisible(true);
            pNode->setIsInMarkovPlusSet(false);
            pNode->setIsInMarkovSet(false);
            pNode->update();
        }
    }

    /* Hide unconnected nodes? */
    foreach(CCausalityPanelGraphNode *pNode, nodes())
    {
        QList<CCausalityPanelGraphEdge *> lstEdges = mmapNodeEdges_.values();
        bool bFound = false;
        foreach(CCausalityPanelGraphEdge *pEdge, lstEdges)
        {
            if(pEdge->sourceNode() == pNode || pEdge->targetNode() == pNode)
                bFound = true;
        }
        if(!bFound)
        {
            if(bShowUnconnectedNodes_)
            {
                 pNode->show();
                 pNode->update();
            }
            else
            {
                 pNode->hide();
            }
        }
    }

    /* Center on treatment and effect */
    CCausalityPanelGraphNode *pNodeTreatment = nullptr;
    CCausalityPanelGraphNode *pNodeEffect = nullptr;
    bool bTreatmentFound = false;
    bool bEffectFound = false;
    QString strTreatmentName = causalModelGroup().treatment().variable().name();
    QString strEffectName = causalModelGroup().effect().variable().name();

    foreach(CCausalityPanelGraphNode *pNode, nodes())
    {
        if(pNode->text() == strTreatmentName)
        {
            bTreatmentFound = true;
            pNodeTreatment = pNode;
            pNode->setIsTreatment(true);
            pNode->setVisible(true);
            pNode->update();
        }
        if(pNode->text() == strEffectName)
        {
            bEffectFound = true;
            pNodeEffect = pNode;
            pNode->setIsEffect(true);
            pNode->setVisible(true);
            pNode->update();
        }
    }

    /* Hide dangling edges */
    foreach(CCausalityPanelGraphEdge *pEdge, edges())
    {
        if(!pEdge->sourceNode()->isVisible() || !pEdge->targetNode()->isVisible())
        {
            pEdge->setVisible(false);
        }

    }

    /* center on */
    if(bTreatmentFound && bEffectFound)
    {
        pTreatmentNode_ = pNodeTreatment;
        pEffectNode_ = pNodeEffect;
        centerOn(pNodeTreatment);
    }
}

void CCausalityPanelGraphView::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
}

void CCausalityPanelGraphView::setShowUnconnectedNodes(bool bShow)
{
//    DEBUG_OBJ_INFO

    bShowUnconnectedNodes_ = bShow;

    postProcessView();
}

bool CCausalityPanelGraphView::showMarkovBlanketPlus() const
{
    return bShowMarkovBlanketPlus_;
}

void CCausalityPanelGraphView::setShowMarkovBlanketPlus(bool bShowMarkovBlanketPlus)
{
    bShowMarkovBlanketPlus_ = bShowMarkovBlanketPlus;

    postProcessView();
}

void CCausalityPanelGraphView::centerTreatmentEffect()
{

}

void CCausalityPanelGraphView::highlightNodes(const QStringList &lstNodeNames)
{
//    DEBUG_OBJ_INFO

    foreach(CCausalityPanelGraphNode *pNode, nodes())
    {
//        QVariant varNode = pNode->data(NODE_DATA_VARIABLE);
        if(lstNodeNames.contains(pNode->text()))
        {
            if(pNode != pTreatmentNode_ && pNode != pEffectNode_)
            {
                pNode->setIsHighlighted(true);
                pNode->update();
            }
        }
        else
        {
            pNode->setIsHighlighted(false);
            pNode->update();
        }
    }
}

void CCausalityPanelGraphView::clearHighlights()
{
    foreach(CCausalityPanelGraphNode *pNode, nodes())
    {
        pNode->setIsHighlighted(false);
        pNode->update();
    }
}

void CCausalityPanelGraphView::updateNodePositionsFromCache()
{
    CApplicationData appData= CMainController::instance().applicationData();
    CCausalModelCacheList lstCausalModelCaches = appData.causalModelCaches();

    /* Search for correct cache */
    CCausalModelCache cache;
    bool bFound = false;
    foreach(CCausalModelCache cacheTemp, lstCausalModelCaches)
    {
        if(causalModelGroup_.group() == cacheTemp.group() &&
           causalModelGroup_.treatment().variable().name() == cacheTemp.treatment().variable().name() &&
           causalModelGroup_.effect().variable().name() == cacheTemp.effect().variable().name())
        {
            cache = cacheTemp;
            bFound = true;
        }
    }

    if(bFound)
    {
        QMap<QString, QPointF> mapNodePositionsUserGraph = cache.nodePositionsUserGraph();
        QMap<QString, QPointF> mapNodePositionMinedGraph = cache.nodePositionsMinedGraph();

        QVector<CCausalityPanelGraphNode *> vecNodes = nodes();
        foreach(CCausalityPanelGraphNode *pNode, vecNodes)
        {
            if(bUseCausalModelGroupMinedGraph_)
            {
                QPointF posCache = mapNodePositionMinedGraph.value(pNode->text());
                pNode->setPos(posCache);
            }
            else
            {
                QPointF posCache = mapNodePositionsUserGraph.value(pNode->text());
                pNode->setPos(posCache);
            }
        }
    }
}

void CCausalityPanelGraphView::storeNodePositionsToCache()
{
//    qDebug() << "storing node positions in cache";
    CApplicationData appData = CMainController::instance().applicationData();
    CCausalModelCacheList lstCausalModelCaches = appData.causalModelCaches();

    /* Search for correct cache */
    CCausalModelCache cache;
    bool bFound = false;
    foreach(CCausalModelCache cacheTemp, lstCausalModelCaches)
    {
        if(causalModelGroup_.group() == cacheTemp.group() &&
           causalModelGroup_.treatment().variable().name() == cacheTemp.treatment().variable().name() &&
           causalModelGroup_.effect().variable().name() == cacheTemp.effect().variable().name())
        {
            cache = cacheTemp;
            bFound = true;
        }
    }

    /* No.., Create new one? */
    if(!bFound)
    {
        cache.setGroup(causalModelGroup_.group());
        cache.setTreatment(causalModelGroup_.treatment());
        cache.setEffect(causalModelGroup_.effect());
    }

    /* update the cache */
    QMap<QString, QPointF> mapNodePositionsUserGraph = cache.nodePositionsUserGraph();
    QMap<QString, QPointF> mapNodePositionMinedGraph = cache.nodePositionsMinedGraph();

    QVector<CCausalityPanelGraphNode *> vecNodes = nodes();
    foreach(CCausalityPanelGraphNode *pNode, vecNodes)
    {
        if(bUseCausalModelGroupMinedGraph_)
        {
            mapNodePositionMinedGraph.insert(pNode->text(), pNode->pos());
        }
        else
        {
            mapNodePositionsUserGraph.insert(pNode->text(), pNode->pos());
        }
    }


    cache.setNodePositionsMinedGraph(mapNodePositionMinedGraph);
    cache.setNodePositionsUserGraph(mapNodePositionsUserGraph);
    lstCausalModelCaches.replace(cache);

    /* Store */
    appData.setCausalModelCaches(lstCausalModelCaches);
    appData.registerUpdate(CApplicationData::Updates::CausalModelCaches, true);
    CMainController::instance().setApplicationData(appData, false);
}

int CCausalityPanelGraphView::markovBlanketPlusDistance() const
{
    return iMarkovBlanketPlusDistance_;
}

void CCausalityPanelGraphView::setMarkovBlanketPlusDistance(int iMarketBlanketPlusDistance)
{
    iMarkovBlanketPlusDistance_ = iMarketBlanketPlusDistance;

    postProcessView();
}

QVector<CCausalityPanelGraphEdge *> CCausalityPanelGraphView::edges() const
{
    return vecEdges_;
}

CMarkovModel CCausalityPanelGraphView::markovModel() const
{
    return markovModel_;
}

void CCausalityPanelGraphView::setMarkovModel(const CMarkovModel &markovModel)
{
    markovModel_ = markovModel;
    postProcessView();
}

void CCausalityPanelGraphView::nodeMoved(CCausalityPanelGraphNode *pNode)
{
}

void CCausalityPanelGraphView::nodeDoubleClicked(CCausalityPanelGraphNode *pNode)
{
//    DEBUG_OBJ_INFO
    if(pNode)
    {
        startAddEdgeMode(pNode);
    }
}

void CCausalityPanelGraphView::removeNode(CCausalityPanelGraphNode *pNode)
{
}

void CCausalityPanelGraphView::contextMenuEvent(QContextMenuEvent *event)
{
}
