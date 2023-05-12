#ifndef NODE_H
#define NODE_H

#include "propertiesinterface.h"
#include <QPointF>

class CNode : public CPropertiesInterface
{
public:
    CNode();
    CNode(const QString& strLabel, const QVariant& data, const QString& strType = QString("normal"));

    CNode(const QString& strLabel, const QString& strType = QString("normal"));
    CNode(const CNode& node);
    virtual ~CNode();

    CNode& operator=(const CNode& node);
    virtual bool operator==(const CNode& node) const;
    virtual bool operator<(const CNode& node) const;
    friend QDataStream& operator>>(QDataStream& in, CNode& node);

    int nodeID() const;
    void setNodeID(int nodeID);

    QString label() const;
    void setLabel(const QString &label);

    QPointF posAlg() const;
    void setPosAlg(const QPointF &pntPosAlg);

    QPointF posUser() const;
    void setPosUser(const QPointF &posUser);

    QVariant data() const;
    void setData(const QVariant &data);

    QString type() const;
    void setType(const QString &type);

    bool IsVisible() const;
    void setIsVisible(bool IsVisible);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonObject(const QJsonObject &jsonObject);

    void update(const CNode &node);

protected:
    int iNodeID_;
    QString strLabel_;
    QString strType_;
    QPointF pntPosAlg_;
    QPointF pntPosUser_;
    QVariant data_;
    bool bIsVisible_;
};

Q_DECLARE_METATYPE(CNode)

QDataStream& operator<<(QDataStream& out, const CNode& node);
QDataStream& operator>>(QDataStream& in, CNode& node);
QDebug operator<<(QDebug dbg, const CNode& node);

inline uint qHash(const CNode &key, uint seed = 0)
{
    Q_UNUSED(seed)

    return qHash(key.getID());
}

#endif // NODE_H
