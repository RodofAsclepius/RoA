#ifndef MULTIEDGE_H
#define MULTIEDGE_H

#include "propertiesinterface.h"
#include "edge.h"
#include "node.h"
#include <QPointF>

class CMultiEdge : public CPropertiesInterface
{
public:
    CMultiEdge();
    CMultiEdge(const CMultiEdge& multiEdge);
    virtual ~CMultiEdge();

    CMultiEdge& operator=(const CMultiEdge& multiEdge);
    virtual bool operator==(const CMultiEdge& multiEdge) const;
    virtual bool operator<(const CMultiEdge& multiEdge) const;
    friend QDataStream& operator>>(QDataStream& in, CMultiEdge& multiEdge);

    CNode firstNode() const;
    void setFirstNode(const CNode &firstNode);

    CNode secondNode() const;
    void setSecondNode(const CNode &secondNode);
    void setNodes(const CNode &nodeFirst, const CNode &nodeSecond);

    QList<CEdge> edges() const;
    void setEdges(const QList<CEdge> &value);

    void addEdge(const CEdge &edge, bool bAllowDuplicates = true);
    void removeEdge(const CEdge &edge);
    bool contains(const CEdge &aEdge) const;
    void clearEdges();

    bool isVisible() const;
    void setIsVisible(bool isVisible);

    QVariant dataFirstEdge() const;
    void setDataFirstEdge(const QVariant &dataFirstEdge);

    QVariant dataSecondEdge() const;
    void setDataSecondEdge(const QVariant &dataSecondEdge);

    bool belongs(const CEdge& edge) const;

protected:
    CNode firstNode_;
    CNode secondNode_;

    QList<CEdge> lstEdges_;

    QVariant dataFirstEdge_;
    QVariant dataSecondEdge_;

    bool bIsVisible_;
};

Q_DECLARE_METATYPE(CMultiEdge)

QDataStream& operator<<(QDataStream& out, const CMultiEdge& multiEdge);
QDataStream& operator>>(QDataStream& in, CMultiEdge& multiEdge);
QDebug operator<<(QDebug dbg, const CMultiEdge& multiEdge);

inline uint qHash(const CMultiEdge &key, uint seed = 0)
{
    Q_UNUSED(seed)

    return qHash(key.getID());
}

#endif // MULTIEDGE_H
