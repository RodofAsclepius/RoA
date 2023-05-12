#include "graph.h"
#include "utils.h"

#include <QMutexLocker>
#include <QRandomGenerator>
#include <QFile>

using namespace utils;

CGraph::CGraph()
    : CPropertiesInterface()
{
    qRegisterMetaType<CGraph>("CGraph");
//    qRegisterMetaTypeStreamOperators<CGraph>("CGraph");
}

CGraph::CGraph(const CGraph& graph)
    : CGraph()
{
    this->operator =(graph);
}

CGraph::~CGraph()
{
}

CGraph& CGraph::operator=(const CGraph& graph)
{
    if (this != &graph)
    {
        setID(graph.getID());
        setProperties(graph.getProperties());

        setNodes(graph.nodes());
        setEdges(graph.edges());
        setData(graph.data());
        setIsVisible(graph.isVisible());
    }
    return *this;
}

bool CGraph::operator<(const CGraph& graph) const
{
    return (getID() < graph.getID());
}


QJsonObject CGraph::toJsonObject(bool bIncludeProperties) const
{
//    DEBUG_OBJ_INFO
    QJsonObject jsonObjectResult;

    /* Id */
    jsonObjectResult["id"] = QString::number(getID());

    /* Nodes */
    jsonObjectResult["nodes"] = nodes().toJsonObject();

    /* Edges */
    jsonObjectResult["edges"] = edges().toJsonObject();

    /* Visible */
    jsonObjectResult["visisble"] = isVisible();

    /* Properties */
    if(bIncludeProperties)
    {
        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObjectResult;
}

void CGraph::fromJsonObject(const QJsonObject &jsonObject)
{
//    DEBUG_OBJ_INFO
    clear();

    QJsonObject jsonObjectResult;

    /* ID */
    QString strID = jsonObject["id"].toString();

    bool ok;
    quint32 lID = strID.toUInt(&ok, 10);
    if(lID != 0)
    {
        setID(lID);
    }

    /* Nodes */
    QJsonValue valNodes = jsonObject["nodes"];
    CNodesList lstNodes;

    if(valNodes.isObject())
    {
        QJsonObject objNodes = valNodes.toObject();
        lstNodes.fromJsonObject(objNodes);
    }
    setNodes(lstNodes);

    /* Edges */
    QJsonValue valEdges = jsonObject["edges"];
    CEdgesList lstEdges;
    if(valEdges.isObject())
    {
        lstEdges.fromJsonObject(valEdges.toObject());
    }
    setEdges(lstEdges);

    /* Visible */
    bool bVisible = jsonObject["visible"].toBool();
    setIsVisible(bVisible);

    /* Properties */
    if(jsonObject["properties"].isObject())
    {
        QJsonObject jsonObjectProperties = jsonObject["properties"].toObject();
        foreach(QString strPropertyName, jsonObjectProperties.keys())
        {
            /* Object */
            if(jsonObjectProperties[strPropertyName].isObject())
            {
                QJsonObject jsonObjectProperty = jsonObjectProperties[strPropertyName].toObject();
                QJsonDocument jsonDoc(jsonObjectProperty);
                QString strJsonData(jsonDoc.toJson(QJsonDocument::Compact));
                addProperty(strPropertyName, QVariant::fromValue(strJsonData));
            }

            /* Array */
            if(jsonObjectProperties[strPropertyName].isArray())
            {
                QJsonArray jsonArrayProperty = jsonObjectProperties[strPropertyName].toArray();
                QJsonDocument jsonDoc(jsonArrayProperty);
                QString strJsonData(jsonDoc.toJson(QJsonDocument::Compact));
                addProperty(strPropertyName, QVariant::fromValue(strJsonData));
            }


//            qDebug() << jsonObjectProperties[strPropertyName];
            /* String */
            if(jsonObjectProperties[strPropertyName].isString())
            {
                QJsonValue jsonValue = jsonObjectProperties[strPropertyName];
//                QJsonDocument jsonDoc(jsonArrayProperty);
//                QString strJsonData(jsonDoc.toJson(QJsonDocument::Compact));

                addProperty(strPropertyName, jsonValue.toString());
            }

            /* Double */
            if(jsonObjectProperties[strPropertyName].isDouble())
            {
                QJsonValue jsonValue = jsonObjectProperties[strPropertyName];
                addProperty(strPropertyName, jsonValue.toDouble());
            }
        }
    }

}

CNodesList CGraph::parents(const CNode &node) const
{
//    DEBUG_OBJ_INFO
    CNodesList lstParentNodes;

    foreach(CEdge edge, edges())
    {
        if(edge.targetNode().label() == node.label())
        {
            CNode nodeTarget = edge.sourceNode();
            lstParentNodes.append(edge.sourceNode());
        }
    }

    return lstParentNodes;
}

CNodesList CGraph::children(const CNode &node) const
{
//    DEBUG_OBJ_INFO
    CNodesList lstChildNodes;

    foreach(CEdge edge, edges())
    {
        if(edge.sourceNode().label() == node.label())
        {
            CNode nodeTarget = edge.targetNode();
            lstChildNodes.append(edge.targetNode());
        }
    }

    return lstChildNodes;
}

CNodesList CGraph::connectedNodes(const CNodesList &lstNodes, int iDepth)
{
//    DEBUG_OBJ_INFO

    CNodesList lstNodesWorking = lstNodes;
    CNodesList lstNodesTemp;

    for(int i=0; i<iDepth; ++i)
    {
        foreach(CNode node, lstNodesWorking)
        {
            /* Get all parents and children */
            CNodesList lstParentNodes;
            lstParentNodes = parents(node);
            lstNodesTemp.append(lstParentNodes);
            CNodesList lstChildNodes = children(node);
            lstNodesTemp.append(lstChildNodes);
        }

        lstNodesWorking.append(lstNodesTemp);
        /* remove dubs */
//        QSet<CNode> setjesNodes = QSet<CNode>(lstNodesWorking.begin(), lstNodesWorking.end());
//        QList<CNode> lstConvert = setjesNodes.values();

//        lstNodesWorking.clear();
//        foreach(CNode nodeConvert, lstConvert)
//        {
//            lstNodesWorking.append(nodeConvert);
//        }
    }

    /* Prep results */
    CNodesList lstNodesResult;
    foreach(CNode node, lstNodesWorking)
    {
        if(!lstNodes.contains(node) && !lstNodesResult.contains(node))
        {
            lstNodesResult.append(node);
        }
    }

    return lstNodesResult;
}

void CGraph::addRandom(int iNodes, int iMaxEdges, bool bNoCycles, bool bNoSelfLoops)
{
    QMutexLocker mutLockers(&_mutex);

    for(int i=0; i<iNodes; ++i)
    {
        lstNodes_.append(CNode());
    }

    for(int j=0; j<iMaxEdges; ++j)
    {
        int iRandom1 = QRandomGenerator::global()->bounded(lstNodes_.count());
        int iRandom2 = QRandomGenerator::global()->bounded(lstNodes_.count());

        if(bNoSelfLoops && iRandom1 == iRandom2)
            continue;

        CNode nodeSource = lstNodes_.at(iRandom1);
        CNode nodeTarget = lstNodes_.at(iRandom2);
        CEdge edge(nodeSource, nodeTarget);
        lstEdges_.append(edge);

        if(bNoCycles)
        {
            mutLockers.unlock();
            if(isCyclic())
            {
                lstEdges_.removeLast();
            }
            mutLockers.relock();
        }
    }
}

bool CGraph::isVisible() const
{
    return bIsVisisble_;
}

void CGraph::setIsVisible(bool bIsVisisble)
{
    bIsVisisble_ = bIsVisisble;
}

void CGraph::addNode(const CNode &node)
{
    QMutexLocker mutLockers(&_mutex);
    if(!lstNodes_.contains(node))
    {
        lstNodes_.append(node);
    }
}

void CGraph::addEdge(const CEdge &edge, bool bInsertFront)
{
    QMutexLocker mutLockers(&_mutex);
    if(!lstEdges_.contains(edge))
    {
        if(bInsertFront)
        {
            lstEdges_.prepend(edge);
        }
        else
        {
            lstEdges_.append(edge);
        }
    }
}

void CGraph::removeNode(const CNode &node)
{
    QMutexLocker mutLockers(&_mutex);
    lstNodes_.removeOne(node);
}

void CGraph::removeEdge(const CEdge &edge)
{
    QMutexLocker mutLockers(&_mutex);
    lstEdges_.removeOne(edge);
}

bool CGraph::removeEdge(const CNode &nodeSource, const CNode &nodeTarget)
{
    for(int i=0; i<edges().count(); ++i)
    {
        CEdge edge = edges().at(i);
        if(edge.sourceNode() == nodeSource && edge.targetNode() == nodeTarget)
        {
            removeEdge(edge);
            return true;
        }
    }

    return false;
}

void CGraph::clearNodes()
{
    QMutexLocker mutLockers(&_mutex);
    lstNodes_.clear();
    mutLockers.unlock();
    clearEdges();
}

void CGraph::clearEdges()
{
    QMutexLocker mutLockers(&_mutex);
    lstEdges_.clear();
}

void CGraph::clear()
{
    clearEdges();
    clearNodes();
}

void CGraph::importEdges(const CGraph &graph)
{
//    DEBUG_OBJ_INFO

    QList<CEdge> edges = graph.edges();

    foreach(CEdge edge, edges)
    {
        CNode sourceNode, targetNode;
        if(nodeByLabel(edge.sourceNode().label(), sourceNode) &&
                nodeByLabel(edge.targetNode().label(), targetNode))
        {
            addEdge(CEdge(sourceNode, targetNode));
        }
    }
}

bool CGraph::nodeByLabel(const QString &strName, CNode& nodeResult)
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

CNodesList CGraph::nodesByLabel(const QStringList &lstNodeNames)
{
    CNodesList lstNodes;

    foreach(QString strLabel, lstNodeNames)
    {
        CNode node;
        if(nodeByLabel(strLabel, node))
        {
            lstNodes.append(node);
        }
    }

    return lstNodes;
}

QList<CNode> CGraph::adj(const CNode& node)
{
    QList<CNode> lstNodesAdj;

    foreach(CEdge edge, lstEdges_)
    {
        if(edge.sourceNode() ==node)
            lstNodesAdj.append(edge.targetNode());
    }

    return lstNodesAdj;
}

bool CGraph::importCSV(const QString &strFilename)
{
    QFile file(strFilename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.errorString();
        return false;
    }

    clear();

    QTextStream s1(&file);
    while (!s1.atEnd())
    {
        QString strLline = s1.readLine();
        QStringList lstColumns = strLline.split(',');
        QString strSource = lstColumns.at(0);
        QString strTarget = lstColumns.at(1);

        CNode nodeSource;
        if(!nodeByLabel(strSource, nodeSource))
        {
            nodeSource = CNode(strSource);
            addNode(nodeSource);
        }
        CNode nodeTarget;
        if(!nodeByLabel(strTarget, nodeTarget))
        {
            nodeTarget = CNode(strTarget);
            addNode(nodeTarget);
        }
        addEdge(CEdge(nodeSource, nodeTarget));
    }

    file.close();

    return true;
}

bool CGraph::exportCSV(const QString &strFilename)
{
    QFile file(strFilename);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << file.errorString();
        return false;
    }

    /* Stream out */
    QTextStream stream(&file);
    foreach(CEdge edge, edges())
    {
        CNode nodeSource = edge.sourceNode();
        CNode nodeTarget = edge.targetNode();

        stream << nodeSource.label() << "," << nodeTarget.label() << Qt::endl;
    }

    file.close();

    return true;
}

CEdgesList CGraph::edges() const
{
    return lstEdges_;
}

void CGraph::setEdges(const CEdgesList &lstEdges)
{
    lstEdges_ = lstEdges;
}

CNodesList CGraph::nodes() const
{
    return lstNodes_;
}

void CGraph::setNodes(const CNodesList &lstNodes)
{
    lstNodes_ = lstNodes;
}

//QMap<CNode, QList<CEdge> > CGraph::nodesChilds() const
//{
//    return mapNodesChilds_;
//}

//void CGraph::setNodeChilds(const QMap<CNode, QList<CEdge> > &mapChilds)
//{
//    mapNodesChilds_ = mapChilds;
//}

//QMap<CNode, QList<CEdge> > CGraph::nodesParents() const
//{
//    return mapNodesParents_;
//}

//void CGraph::setNodesParents(const QMap<CNode, QList<CEdge> > &mapParents)
//{
//    mapNodesParents_ = mapParents;
//}


bool CGraph::operator==(const CGraph& graph) const
{
    return (getID() == graph.getID());
}

// This function is a variation of DFSUtil() in https://www.geeksforgeeks.org/archives/18212
bool CGraph::isCyclicUtil(const CNode& node)
{
    if(mapVisistedNodes_[node] == false)
    {
        // Mark the current node as visited and part of recursion stack
        mapVisistedNodes_[node] = true;
        mapNodesRecStack_[node] = true;

//        // Recur for all the vertices adjacent to this vertex
//        list<int>::iterator i;
//        for(i = adj[v].begin(); i != adj[v].end(); ++i)
//        {
//            if ( !visited[*i] && isCyclicUtil(*i, visited, recStack) )
//                return true;
//            else if (recStack[*i])
//                return true;
//        }

        foreach(CNode nodeAdj, adj(node))
        {
            if ( !mapVisistedNodes_.value(nodeAdj) && isCyclicUtil(nodeAdj) )
                return true;
            else if (mapNodesRecStack_.value(nodeAdj))
                return true;
        }

    }

    mapNodesRecStack_[node] = false;  // remove the vertex from recursion stack
    return false;
}

// Returns true if the graph contains a cycle, else false.
// This function is a variation of DFS() in https://www.geeksforgeeks.org/archives/18212
bool CGraph::isCyclic()
{
    QMutexLocker mutLockers(&_mutex);

    // Mark all the vertices as not visited and not part of recursion
    // stack
    //    bool *visited = new bool[lstNodes_.length()];
    //    bool *recStack = new bool[lstNodes_.length()];
//    for(int i = 0; i < lstNodes_.length(); i++)
//    {
//        visited[i] = false;
//        recStack[i] = false;
//    }
    mapVisistedNodes_.clear();
    mapNodesRecStack_.clear();
    foreach(CNode node, lstNodes_)
    {
        mapVisistedNodes_.insert(node, false);
        mapNodesRecStack_.insert(node, false);
    }

    // Call the recursive helper function to detect cycle in different
    // DFS trees
    foreach(CNode node, lstNodes_)
    {
        if(isCyclicUtil(node))
            return true;
    }

    return false;
}

QList<QList<CNode> > CGraph::topologicalSort()
{

}

QMap<QString, QVariant> CGraph::data() const
{
    return mapData_;
}

void CGraph::setData(const QMap<QString, QVariant> &mapData)
{
    mapData_ = mapData;
}

QVariant CGraph::dataItem(const QString &strKey)
{
    return mapData_.value(strKey);
}

void CGraph::setDataItem(const QString &strkey, const QVariant &varData)
{
    mapData_.insert(strkey, varData);
}

void CGraph::clearData()
{
    mapData_.clear();
}

QDataStream& operator<<(QDataStream& out, const CGraph& graph)
{
    out << graph.getID()
        << graph.getProperties()
        << graph.nodes()
        << graph.edges()
        << graph.data()
        << graph.isVisible();

    return out;
}

QDataStream& operator>>(QDataStream& in, CGraph& graph)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;

    CNodesList lstNodes;
    CEdgesList lstEdges;
    QMap<QString, QVariant> mapData;
    bool bIsVisible;

    in >> lID >> mapProperties
       >> lstNodes
       >> lstEdges
       >> mapData
       >> bIsVisible;

    CGraph graphNew;
    graphNew.setID(lID);
    graphNew.setProperties(mapProperties);

    graphNew.setNodes(lstNodes);
    graphNew.setEdges(lstEdges);
    graphNew.setData(mapData);
    graphNew.setIsVisible(bIsVisible);

    graph = graphNew;

    return in;
}

QDebug operator<<(QDebug dbg, const CGraph& graph)
{
    dbg.nospace()   << g_spacer
                    << "CGraph(" << graph.getID()
                    << ", " << graph.isVisible()
//                    << ", " << graph.nodes()
                    << ", " << graph.edges()
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


