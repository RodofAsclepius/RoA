#ifndef CAUSALMODELCACHELIST_H
#define CAUSALMODELCACHELIST_H

#include "causalmodelcache.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

class CCausalModelCacheList : public QList<CCausalModelCache>, public CPropertiesInterface
{
public:
    CCausalModelCacheList();
    ~CCausalModelCacheList();
    CCausalModelCacheList(const CCausalModelCacheList& lstCausalModelsCaches);
    CCausalModelCacheList& operator=(const CCausalModelCacheList& lstCausalModelsCaches);
    virtual bool operator==(const CCausalModelCacheList& lstCausalModelsCaches);
    virtual bool operator<(const CCausalModelCacheList& lstCausalModelsCaches) const;
    friend QDataStream& operator>>(QDataStream& in, CCausalModelCacheList& lstCausalModelsCaches);

    void append(const CCausalModelCache& causalModelCache);
    void append(const CCausalModelCacheList& lstCausalModelsCaches);

    void replace(int iIndex, const CCausalModelCache& causalModelCache);
    void replace(const CCausalModelCache& causalModelCache);
    void replace(const CCausalModelCacheList& lstCausalModelsCaches);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObject);

protected:

};

Q_DECLARE_METATYPE(CCausalModelCacheList)

QDataStream& operator<<(QDataStream& out, const CCausalModelCacheList& lstCausalModelsCaches);
QDataStream& operator>>(QDataStream& in, CCausalModelCacheList& lstCausalModelsCaches);

#endif // CAUSALMODELCACHELIST_H
