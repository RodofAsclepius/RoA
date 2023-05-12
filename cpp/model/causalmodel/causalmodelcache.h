#ifndef causalModelCacheCACHE_H
#define causalModelCacheCACHE_H

#include "propertiesinterface.h"
#include "group.h"
#include "treatment.h"
#include "effect.h"
#include <QPointF>

class CCausalModelCache : public CPropertiesInterface
{
    Q_OBJECT
public:
    CCausalModelCache();
    CCausalModelCache(const CGroup &group, const CTreatment &treatment, const CEffect &effect);
    CCausalModelCache(const CCausalModelCache& causalModelCache);
    virtual ~CCausalModelCache();

    CCausalModelCache& operator=(const CCausalModelCache& causalModelCache);
    virtual bool operator==(const CCausalModelCache& causalModelCache) const;
    virtual bool operator<(const CCausalModelCache& causalModelCache) const;
    friend QDataStream& operator>>(QDataStream& in, CCausalModelCache& causalModelCache);

    void fromJsonObject(const QJsonObject& jsonObject);
    QJsonObject toJsonObject(bool bIncludeProperties = true) const;

    CGroup group() const;
    void setGroup(const CGroup &group);

    CTreatment treatment() const;
    void setTreatment(const CTreatment &treatment);

    CEffect effect() const;
    void setEffect(const CEffect &effect);

    QMap<QString, QPointF> nodePositionsUserGraph() const;
    void setNodePositionsUserGraph(const QMap<QString, QPointF> &mapNodePosition);

    QMap<QString, QPointF> nodePositionsMinedGraph() const;
    void setNodePositionsMinedGraph(const QMap<QString, QPointF> &mapNodePositionMinedGraph);

protected:
    CGroup group_;
    CTreatment treatment_;
    CEffect effect_;
    QMap<QString, QPointF> mapNodePositionsUserGraph_;
    QMap<QString, QPointF> mapNodePositionMinedGraph_;
};

Q_DECLARE_METATYPE(CCausalModelCache)

QDataStream& operator<<(QDataStream& out, const CCausalModelCache& causalModelCache);
QDataStream& operator>>(QDataStream& in, CCausalModelCache& causalModelCache);
QDebug operator<<(QDebug dbg, const CCausalModelCache& causalModelCache);

#endif // causalModelCacheCACHE_H


