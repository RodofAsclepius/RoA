#include "multigraph.h"
#include "utils.h"

#include <QMutexLocker>
#include <QRandomGenerator>
#include <QFile>

using namespace utils;

CMultiGraph::CMultiGraph()
    : CPropertiesInterface()
{
    qRegisterMetaType<CMultiGraph>("CMultiGraph");
//    qRegisterMetaTypeStreamOperators<CMultiGraph>("CMultiGraph");
}

CMultiGraph::CMultiGraph(const CMultiGraph& graph)
    : CMultiGraph()
{
    this->operator =(graph);
}

CMultiGraph::~CMultiGraph()
{
}

CMultiGraph& CMultiGraph::operator=(const CMultiGraph& graph)
{
    if (this != &graph)
    {
        setID(graph.getID());
        setProperties(graph.getProperties());

        setNodes(graph.nodes());
        setMultiEdges(graph.multiEdges());
        setData(graph.data());
        setIsVisible(graph.isVisible());
    }
    return *this;
}


bool CMultiGraph::operator<(const CMultiGraph& graph) const
{
    return (getID() < graph.getID());
}

bool CMultiGraph::isVisible() const
{
    return bIsVisisble_;
}

void CMultiGraph::setIsVisible(bool bIsVisisble)
{
    bIsVisisble_ = bIsVisisble;
}

void CMultiGraph::addNode(const CNode &node)
{
    QMutexLocker mutLockers(&_mutex);
    if(!lstNodes_.contains(node))
    {
        lstNodes_.append(node);
    }
}

void CMultiGraph::addMultiEdge(const CMultiEdge &multiEdge, bool bInsertFront)
{
    QMutexLocker mutLockers(&_mutex);
    if(!lstMultiEdges_.contains(multiEdge))
    {
        if(bInsertFront)
        {
            lstMultiEdges_.prepend(multiEdge);
        }
        else
        {
            lstMultiEdges_.append(multiEdge);
        }
    }
}

void CMultiGraph::removeNode(const CNode &node)
{
    QMutexLocker mutLockers(&_mutex);
    lstNodes_.removeOne(node);
}

void CMultiGraph::removeMultiEdge(const CMultiEdge &multiEdge)
{
    QMutexLocker mutLockers(&_mutex);
    lstMultiEdges_.removeOne(multiEdge);
}

void CMultiGraph::addNodes(const QList<CNode> &lstNodes, bool bAllowDupliates)
{
    foreach(CNode node, lstNodes)
    {
        CNode nodeResult;
        bool bNodePresent = nodeByLabel(node.label(), nodeResult);
        if(!bAllowDupliates && bNodePresent)
        {
            // Skip
        }
        else
        {
            lstNodes_.append(node);
        }
    }
}

void CMultiGraph::clearNodes()
{
    QMutexLocker mutLockers(&_mutex);
    lstNodes_.clear();
    mutLockers.unlock();
    clearEdges();
}

void CMultiGraph::clearEdges()
{
    QMutexLocker mutLockers(&_mutex);
    lstMultiEdges_.clear();
}

void CMultiGraph::clear()
{
    clearEdges();
    clearNodes();
}

void CMultiGraph::addEdge(const CEdge &edge, bool bAllowDuplicates)
{
    bool bFound = false;

    QList<CMultiEdge> lstMultiEdgesNew = lstMultiEdges_;

    /* find correct multi edge and append */
    foreach(CMultiEdge multiEdge, lstMultiEdges_)
    {
        if(multiEdge.belongs(edge))
        {
                multiEdge.addEdge(edge, bAllowDuplicates);
                lstMultiEdgesNew.removeOne(multiEdge);
                lstMultiEdgesNew.append(multiEdge);
                bFound = true;
                break;
        }
    }

    /* Or create new multi edge */
    if(!bFound)
    {
        CMultiEdge multiEdgeNew;
        multiEdgeNew.setFirstNode(edge.sourceNode());
        multiEdgeNew.setSecondNode(edge.targetNode());
        multiEdgeNew.addEdge(edge);
        lstMultiEdgesNew.append(multiEdgeNew);
    }

    lstMultiEdges_ = lstMultiEdgesNew;
}

void CMultiGraph::removeEdge(const CEdge &edge)
{

}

void CMultiGraph::importGraph(const CGraph &graph, const QVariant &varData)
{
//    DEBUG_OBJ_INFO

    importNodes(graph, varData);
    importEdges(graph, varData);
}

void CMultiGraph::importNodes(const CGraph &graph, const QVariant &varData)
{
    Q_UNUSED(varData)

    addNodes(graph.nodes(), false);
}

void CMultiGraph::importEdges(const CGraph &graph, const QVariant &varData)
{
//    DEBUG_OBJ_INFO

    QList<CEdge> edges = graph.edges();

    foreach(CEdge edge, edges)
    {
        CNode sourceNode, targetNode;
        if(nodeByLabel(edge.sourceNode().label(), sourceNode) &&
                nodeByLabel(edge.targetNode().label(), targetNode))
        {
            CEdge edgeNew(sourceNode, targetNode);
            edgeNew.setData(varData);
            addEdge(edgeNew);
        }
    }
}

bool CMultiGraph::nodeByLabel(const QString &strName, CNode& nodeResult)
{
    foreach(CNode node, lstNodes_)
    {
        if(node.label() == strName)
        {
            nodeResult = node;
            return true;
        }
    }

    return false;
}

QList<CNode> CMultiGraph::adj(const CNode& node)
{
    QList<CNode> lstNodesAdj;

    foreach(CMultiEdge multiEdge, lstMultiEdges_)
    {
        QList<CEdge> edges = multiEdge.edges();

        if(!edges.isEmpty())
        {
            CEdge edgePrimary = edges.first();
            if(edgePrimary.sourceNode() == node)
                lstNodesAdj.append(edgePrimary.targetNode());
        }
    }

    return lstNodesAdj;
}

void CMultiGraph::addRandom(int iNodes, int iMaxEdges, bool bNoCycles, bool bNoSelfLoops)
{
    /* Add nodes */
    for(int i=0; i<iNodes; ++i)
    {

    }
}

QMap<QString, QVariant> CMultiGraph::data() const
{
    return mapData_;
}

void CMultiGraph::setData(const QMap<QString, QVariant> &mapData)
{
    mapData_ = mapData;
}

void CMultiGraph::setDataItem(const QString &strKey, const QVariant &data)
{
    mapData_.insert(strKey, data);
}

QVariant CMultiGraph::dataItem(const QString &strKey)
{
    return mapData_.value(strKey);
}

void CMultiGraph::clearData()
{
    mapData_.clear();
}

QList<CMultiEdge> CMultiGraph::multiEdges() const
{
    return lstMultiEdges_;
}

void CMultiGraph::setMultiEdges(const QList<CMultiEdge> &lstMultiEdges)
{
    lstMultiEdges_ = lstMultiEdges;
}

QList<CNode> CMultiGraph::nodes() const
{
    return lstNodes_;
}

void CMultiGraph::setNodes(const QList<CNode> &lstNodes)
{
    lstNodes_ = lstNodes;
}

bool CMultiGraph::operator==(const CMultiGraph& graph) const
{
    return (getID() == graph.getID());
}

QDataStream& operator<<(QDataStream& out, const CMultiGraph& graph)
{
    out << graph.getID()
        << graph.getProperties()
        << graph.nodes()
        << graph.multiEdges()
        << graph.data()
        << graph.isVisible();

    return out;
}

QDataStream& operator>>(QDataStream& in, CMultiGraph& graph)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;

    QList<CNode> lstNodes;
    QList<CMultiEdge> lstMultiEdges;
    QMap<QString, QVariant> data;
    bool bIsVisible;

    in >> lID >> mapProperties
       >> lstNodes
       >> lstMultiEdges
       >> data
       >> bIsVisible;

    CMultiGraph graphNew;
    graphNew.setNodes(lstNodes);
    graphNew.setMultiEdges(lstMultiEdges);
    graphNew.setData(data);
    graphNew.setIsVisible(bIsVisible);

    graph = graphNew;

    return in;
}

QDebug operator<<(QDebug dbg, const CMultiGraph& graph)
{
    dbg.nospace()   << g_spacer
                    << "CGraph(" << graph.getID()
                    << ", " << graph.isVisible()
                    << ", " << graph.nodes()
                    << ", " << graph.multiEdges()
                    << ", " << graph.data()
                    << ")" << "\n";

        g_spacer.indent();
        dbg.nospace() << g_spacer << "Properties:" << "\n";
        QMapIterator<QString, QVariant> mapIterator(graph.getProperties());
        while (mapIterator.hasNext())
        {
              mapIterator.next();
              dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
        }
        g_spacer.unindent();

    g_spacer.unindent();

    return dbg.maybeSpace();
}


