#include "causalmodelcachelist.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>
#include "utils.h"

using namespace utils;

CCausalModelCacheList::CCausalModelCacheList()
    :QList<CCausalModelCache>()
{
    qRegisterMetaType<CCausalModelCacheList>("CCausalModelCacheList");
//    qRegisterMetaTypeStreamOperators<CCausalModelCacheList>("CCausalModelCacheList");
}

CCausalModelCacheList::CCausalModelCacheList(const CCausalModelCacheList& lstCausalModelsCaches)
    : QList<CCausalModelCache>(), CPropertiesInterface()
{
    this->operator =(lstCausalModelsCaches);
}

CCausalModelCacheList::~CCausalModelCacheList()
{

}

CCausalModelCacheList& CCausalModelCacheList::operator=(const CCausalModelCacheList& lstCausalModelsCaches)
{
    if (this != &lstCausalModelsCaches)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CCausalModelCache>::operator =(lstCausalModelsCaches);
        mutLocker.unlock();
        setID(lstCausalModelsCaches.getID());
    }
    return* this;
}

bool CCausalModelCacheList::operator==(const CCausalModelCacheList& lstCausalModelsCaches)
{
    return (_lID == lstCausalModelsCaches.getID());
}

bool CCausalModelCacheList::operator<(const CCausalModelCacheList& lstCausalModelsCaches) const
{
    return (getID() < lstCausalModelsCaches.getID());
}

void CCausalModelCacheList::append(const CCausalModelCache &causalModel)
{
    QMutexLocker mutLocker(&_mutex);

    if(!contains(causalModel))
    {
        QList<CCausalModelCache>::append(causalModel);
    }
    else
    {
        replace(causalModel);
    }
}

void CCausalModelCacheList::append(const CCausalModelCacheList &lstCausalModelsCaches)
{
    foreach(CCausalModelCache causalModelsCache, lstCausalModelsCaches)
    {
        append(causalModelsCache);
    }
}

void CCausalModelCacheList::replace(int iIndex, const CCausalModelCache& causalModelsCache)
{
    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, causalModelsCache);
}

void CCausalModelCacheList::replace(const CCausalModelCache& causalModelsCache)
{
    int iIndex =indexOf(causalModelsCache);
    if(iIndex != -1)
    {
        replace(iIndex, causalModelsCache);
    }
    else
    {
        append(causalModelsCache);
    }
}


void CCausalModelCacheList::replace(const CCausalModelCacheList& lstCausalModelsCaches)
{
    foreach(CCausalModelCache causalModelsCache, lstCausalModelsCaches)
    {
        replace(causalModelsCache);
    }
}

QJsonObject CCausalModelCacheList::toJsonObject(bool bIncludeProperties) const
{
//    DEBUG_OBJ_INFO

    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CCausalModelCache causalModel = at(iIndex);
        jsonObjectResult[QString::number(causalModel.getID())] = causalModel.toJsonObject(bIncludeProperties);
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonObjectResult;
}

void CCausalModelCacheList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObjectcausalModelsCache = jsonValue.toObject();

        CCausalModelCache causalModelsCache;
        causalModelsCache.fromJsonObject(jsonObjectcausalModelsCache);

        /* Combine result */
        append(causalModelsCache);
    }
}

void CCausalModelCacheList::fromJsonObject(const QJsonObject &jsonObject)
{
    clear();

    QStringList lstKeys = jsonObject.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObject[strKey];
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CCausalModelCache causalModelsCache;
        causalModelsCache.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(causalModelsCache);
    }
}

QDataStream& operator<<(QDataStream& out, const CCausalModelCacheList& lstCausalModelsCaches)
{
    out << static_cast <const QList<CCausalModelCache>& >( lstCausalModelsCaches );
    out << lstCausalModelsCaches.getID();
    out << lstCausalModelsCaches.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CCausalModelCacheList& lstCausalModelsCaches)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CCausalModelCache>& >( lstCausalModelsCaches );
    in >> lID;
    lstCausalModelsCaches.setID(lID);
    in >> mapProperties;

    lstCausalModelsCaches.setProperties(mapProperties);

    return in;
}

