#ifndef EDGE_H
#define EDGE_H

#include "propertiesinterface.h"
#include "node.h"
#include <QPointF>

class CEdge : public CPropertiesInterface
{
public:
    CEdge();

    CEdge(const CNode& nodeSource, const CNode& nodeTarget, const QString& strLabel = QString());
    CEdge(const CEdge& edge);
    virtual ~CEdge();

    CEdge& operator=(const CEdge& edge);
    virtual bool operator==(const CEdge& edge) const;
    virtual bool operator<(const CEdge& edge) const;
    friend QDataStream& operator>>(QDataStream& in, CEdge& edge);

    int edgeID() const;
    void setEdgeID(int nodeID);

    QString label() const;
    void setLabel(const QString &label);

    QVariant data() const;
    void setData(const QVariant &data);

    CNode sourceNode() const;
    void setSourceNode(const CNode& sourceNode);

    CNode targetNode() const;
    void setTargetNode(const CNode& targetNode);

    QString type() const;
    void setType(const QString &type);

    QPair<QPointF, QPointF> endPointsAlg() const;
    void setEndPointsAlg(const QPair<QPointF, QPointF> &endPointsAlg);

    QList<QPointF> controlPointsAlg() const;
    void setControlPointsAlg(const QList<QPointF> &controlPointsAlg);

    QPair<QPointF, QPointF> endPointsUser() const;
    void setEndPointsUser(const QPair<QPointF, QPointF> &endPointsUser);

    QList<QPointF> controlPointsUser() const;
    void setControlPointsUser(const QList<QPointF> &controlPointsUser);

    bool isVisible() const;
    void setVisible(bool isVisible);

    QPair<CNode, CNode> nodes() const;
    void setNodes(const QPair<CNode, CNode> &pairNodes);

    QJsonObject toJsonObject(bool bIncludeProperties) const;
    void fromJsonObject(const QJsonObject &jsonObject);

    void update(const CEdge &edge);

    QJsonObject pointToJsonObject(const QPointF& pnt) const;
    QPointF pointFromJsonObject(const QJsonObject &objPoint) const;

protected:
    int iEdgeID_;
    QPair<CNode, CNode> pairNodes_;
    QString strLabel_;
    QString strType_;
    QPair<QPointF, QPointF> pairEndPointsAlg_;
    QList<QPointF> lstControlPointsAlg_;
    QPair<QPointF, QPointF> pairEndPointsUser_;
    QList<QPointF> lstControlPointsUser_;

    QVariant data_;
    bool bIsVisible_;
};

Q_DECLARE_METATYPE(CEdge)

QDataStream& operator<<(QDataStream& out, const CEdge& edge);
QDataStream& operator>>(QDataStream& in, CEdge& edge);
QDebug operator<<(QDebug dbg, const CEdge& edge);

inline uint qHash(const CEdge &key, uint seed = 0)
{
    Q_UNUSED(seed)

    return qHash(key.getID());
}

#endif // EDGE_H
