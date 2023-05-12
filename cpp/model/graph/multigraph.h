#ifndef MULTIGRAPH_H
#define MULTIGRAPH_H

#include "propertiesinterface.h"
#include "node.h"
#include "multiedge.h"
#include "group.h"
#include "graph.h"

#include <QList>
#include <QString>
#include <QMutex>
#include <QMetaType>
#include <QDataStream>
#include <QColor>

class CMultiGraph : public CPropertiesInterface
{
    Q_OBJECT
public:
    CMultiGraph();
    CMultiGraph(const QString& strName, const QString& strQuery);
    CMultiGraph(const CMultiGraph& Group);
    virtual ~CMultiGraph();

    CMultiGraph& operator=(const CMultiGraph& Group);
    virtual bool operator==(const CMultiGraph& Group) const;
    virtual bool operator<(const CMultiGraph& Group) const;
    friend QDataStream& operator>>(QDataStream& in, CMultiGraph& group);

    QList<CNode> nodes() const;
    void setNodes(const QList<CNode> &nodes);

    QList<CMultiEdge> multiEdges() const;
    void setMultiEdges(const QList<CMultiEdge> &multiEdges);

    bool isVisible() const;
    void setIsVisible(bool isVisible);

    void addNode(const CNode& node);
    void addMultiEdge(const CMultiEdge& multiEdge, bool bInsertFront = false);
    void removeNode(const CNode& node);
    void removeMultiEdge(const CMultiEdge& multiEdge);
    void addNodes(const QList<CNode>& lstNodes, bool bAllowDupliates=false);

    void clearNodes();
    void clearEdges();
    void clear();

    void addEdge(const CEdge &edge, bool bAllowDuplicates = true);
    void removeEdge(const CEdge &edge);

    QMap<QString, QVariant> data() const;
    void setData(const QMap<QString, QVariant> &data);
    void setDataItem(const QString &strKey, const QVariant &data);
    QVariant dataItem(const QString &strKey);
    void clearData();

    /* Merge other graphs */
    void importGraph(const CGraph& graph, const QVariant &varData);
    void importNodes(const CGraph& graph, const QVariant &varData);
    void importEdges(const CGraph& graph, const QVariant &varData);

    bool nodeByLabel(const QString &strName, CNode& nodeResult);

    QList<CNode> adj(const CNode& node);
    void addRandom(int iNodes, int iMaxEdges, bool bNoCycles = false, bool bNoSelfLoops = true);
protected:
    QList<CNode> lstNodes_;
    QList<CMultiEdge> lstMultiEdges_;
    QMap<QString, QVariant> mapData_;
    bool bIsVisisble_;

    CMultiEdge multiEdgeFromEdge(const CEdge& edge);
private:
};

Q_DECLARE_METATYPE(CMultiGraph)

QDataStream& operator<<(QDataStream& out, const CMultiGraph& graph);
QDataStream& operator>>(QDataStream& in, CMultiGraph& graph);
QDebug operator<<(QDebug dbg, const CMultiGraph& graph);

#endif // MULTIGRAPH_H
