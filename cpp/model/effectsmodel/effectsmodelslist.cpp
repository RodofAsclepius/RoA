#include "effectsmodelslist.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>

CEffectsModelsList::CEffectsModelsList()
    :QList<CEffectsModel>()
{
    qRegisterMetaType<CEffectsModelsList>("CEffectsModelsList");
//    qRegisterMetaTypeStreamOperators<CEffectsModelsList>("CEffectsModelsList");
}


CEffectsModelsList::CEffectsModelsList(const CEffectsModelsList& lstEffectsModels)
    : QList<CEffectsModel>(), CPropertiesInterface()
{
    this->operator =(lstEffectsModels);
}


CEffectsModelsList::~CEffectsModelsList()
{

}


CEffectsModelsList& CEffectsModelsList::operator=(const CEffectsModelsList& lstEffectsModels)
{
    if (this != &lstEffectsModels)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CEffectsModel>::operator =(lstEffectsModels);
        mutLocker.unlock();
        setID(lstEffectsModels.getID());
    }
    return* this;
}


bool CEffectsModelsList::operator==(const CEffectsModelsList& lstEffectsModels)
{
    return (_lID == lstEffectsModels.getID());
}


bool CEffectsModelsList::operator<(const CEffectsModelsList& lstEffectsModels) const
{
    return (getID() < lstEffectsModels.getID());
}

void CEffectsModelsList::append(const CEffectsModel &effectsModel)
{
    QMutexLocker mutLocker(&_mutex);

    if(!contains(effectsModel))
    {
        QList<CEffectsModel>::append(effectsModel);
    }
    else
    {
        replace(effectsModel);
    }
}


void CEffectsModelsList::append(const CEffectsModelsList &lstEffectsModels)
{
    foreach(CEffectsModel effectsModel, lstEffectsModels)
    {
        append(effectsModel);
    }
}


void CEffectsModelsList::replace(int iIndex, const CEffectsModel& effectsModel)
{
    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, effectsModel);
}


void CEffectsModelsList::replace(const CEffectsModel& effectsModel)
{
    int iIndex =indexOf(effectsModel);
    if(iIndex != -1)
    {
        replace(iIndex, effectsModel);
    }
}


void CEffectsModelsList::replace(const CEffectsModelsList& lstEffectsModels)
{
    foreach(CEffectsModel effectsModel, lstEffectsModels)
    {
        replace(effectsModel);
    }
}


//void CEffectsModelsList::update(const CEffectsModelsList &lstEffectsModels, bool bAppendNeweffectsModel)
//{
//    /* Add or update items */
//    foreach(CEffectsModel effectsModel, lstEffectsModels)
//    {
//        /* Get current one based on ID */
//        int iIndex = indexOf(effectsModel);

//        if(iIndex == -1)
//        {
//            if(bAppendNeweffectsModel)
//            {
//                append(effectsModel);
//            }
//        }
//        else
//        {
//            /* Merge all properties */
//            CEffectsModel currenteffectsModel = at(iIndex);
//            currenteffectsModel.update(effectsModel);
//            replace(iIndex, currenteffectsModel);
//        }
//    }
//}


QJsonObject CEffectsModelsList::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CEffectsModel effectsModel = at(iIndex);
        jsonObjectResult[QString::number(effectsModel.getID())] = effectsModel.toJsonObject(bIncludeProperties);
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonObjectResult;
}

void CEffectsModelsList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObjecteffectsModel = jsonValue.toObject();

        CEffectsModel effectsModel;
        effectsModel.fromJsonObject(jsonObjecteffectsModel);

        /* Combine result */
        append(effectsModel);
    }
}

void CEffectsModelsList::fromJsonObject(const QJsonObject &jsonObject)
{
    clear();

    QStringList lstKeys = jsonObject.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObject[strKey];
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CEffectsModel effectsModel;
        effectsModel.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(effectsModel);
    }
}

QDataStream& operator<<(QDataStream& out, const CEffectsModelsList& lstEffectsModels)
{
    out << static_cast <const QList<CEffectsModel>& >( lstEffectsModels );
    out << lstEffectsModels.getID();
    out << lstEffectsModels.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CEffectsModelsList& lstEffectsModels)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CEffectsModel>& >( lstEffectsModels );
    in >> lID;
    lstEffectsModels.setID(lID);
    in >> mapProperties;

    lstEffectsModels.setProperties(mapProperties);

    return in;
}

