#ifndef GRAPH_H
#define GRAPH_H

#include "propertiesinterface.h"
#include "nodeslist.h"
#include "edgeslist.h"
#include "group.h"

#include <QList>
#include <QString>
#include <QMutex>
#include <QMetaType>
#include <QDataStream>
#include <QColor>

class CGraph : public CPropertiesInterface
{
    Q_OBJECT
public:
    CGraph();
    CGraph(const QString& strName, const QString& strQuery);
    CGraph(const CGraph& Group);
    virtual ~CGraph();

    CGraph& operator=(const CGraph& Group);
    virtual bool operator==(const CGraph& Group) const;
    virtual bool operator<(const CGraph& Group) const;
    friend QDataStream& operator>>(QDataStream& in, CGraph& group);

    CNodesList nodes() const;
    void setNodes(const CNodesList &lstNodes);

    CEdgesList edges() const;
    void setEdges(const CEdgesList &lstEdges);

    bool isVisible() const;
    void setIsVisible(bool isVisible);

    void addNode(const CNode& node);
    void addEdge(const CEdge& edge, bool bInsertFront = false);
    void removeNode(const CNode& node);
    void removeEdge(const CEdge& edge);
    bool removeEdge(const CNode& nodeSource, const CNode& nodeTarget);

    void clearNodes();
    void clearEdges();
    void clear();

    void importEdges(const CGraph& graph);

    bool nodeByLabel(const QString &strName, CNode& nodeResult);
    CNodesList nodesByLabel(const QStringList &lstNodeNames);

    QList<CNode> adj(const CNode& node);

    //    int filterNodes(const QString& strFilter) const;

    /* Merge other graphs */

    /* Import and export */
    bool importCSV(const QString &strFilename);
    bool exportCSV(const QString &strFilename);

    /* Detect cycles and sorting */

    void addRandom(int iNodes, int iMaxEdges, bool bNoCycles = false, bool bNoSelfLoops = true);

    bool isCyclic();
    QList<QList<CNode>> topologicalSort();

    QMap<QString, QVariant> data() const;
    void setData(const QMap<QString, QVariant> &data);
    QVariant dataItem(const QString &strKey);
    void setDataItem(const QString &strkey, const QVariant &varData);
    void clearData();

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonObject(const QJsonObject &jsonObject);

    CNodesList connectedNodes(const CNodesList &lstNodes, int iDepth=1);
    CNodesList parents(const CNode &node) const;
    CNodesList children(const CNode &node) const;
protected:
    CNodesList lstNodes_;
    CEdgesList lstEdges_;
    QMap<QString, QVariant> mapData_;
    bool bIsVisisble_;
    bool isCyclicUtil(const CNode& node);
private:
    QMap<CNode, bool> mapVisistedNodes_;
    QMap<CNode, bool> mapNodesRecStack_;
};

Q_DECLARE_METATYPE(CGraph)

QDataStream& operator<<(QDataStream& out, const CGraph& graph);
QDataStream& operator>>(QDataStream& in, CGraph& graph);
QDebug operator<<(QDebug dbg, const CGraph& graph);

#endif // GRAPH_H
