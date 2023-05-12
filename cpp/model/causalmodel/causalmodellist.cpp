#include "causalmodellist.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>
#include "utils.h"

using namespace utils;

CCausalModelList::CCausalModelList()
    :QList<CCausalModel>()
{
    qRegisterMetaType<CCausalModelList>("CCausalModelList");
//    qRegisterMetaTypeStreamOperators<CCausalModelList>("CCausalModelList");
}


CCausalModelList::CCausalModelList(const CCausalModelList& lstCausalModels)
    : QList<CCausalModel>(), CPropertiesInterface()
{
    this->operator =(lstCausalModels);
}


CCausalModelList::~CCausalModelList()
{

}


CCausalModelList& CCausalModelList::operator=(const CCausalModelList& lstCausalModels)
{
    if (this != &lstCausalModels)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CCausalModel>::operator =(lstCausalModels);
        mutLocker.unlock();
        setID(lstCausalModels.getID());
    }
    return* this;
}

bool CCausalModelList::operator==(const CCausalModelList& lstCausalModels)
{
    return (_lID == lstCausalModels.getID());
}

bool CCausalModelList::operator<(const CCausalModelList& lstCausalModels) const
{
    return (getID() < lstCausalModels.getID());
}

void CCausalModelList::append(const CCausalModel &causalModel)
{
    QMutexLocker mutLocker(&_mutex);

    if(!contains(causalModel))
    {
        QList<CCausalModel>::append(causalModel);
    }
    else
    {
        replace(causalModel);
    }
}

void CCausalModelList::append(const CCausalModelList &lstCausalModels)
{
    foreach(CCausalModel balanceModel, lstCausalModels)
    {
        append(balanceModel);
    }
}

void CCausalModelList::replace(int iIndex, const CCausalModel& balanceModel)
{
    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, balanceModel);
}

void CCausalModelList::replace(const CCausalModel& balanceModel)
{
    int iIndex =indexOf(balanceModel);
    if(iIndex != -1)
    {
        replace(iIndex, balanceModel);
    }
}


void CCausalModelList::replace(const CCausalModelList& lstCausalModels)
{
    foreach(CCausalModel balanceModel, lstCausalModels)
    {
        replace(balanceModel);
    }
}

QJsonObject CCausalModelList::toJsonObject(bool bIncludeProperties) const
{
//    DEBUG_OBJ_INFO

    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CCausalModel causalModel = at(iIndex);
        jsonObjectResult[QString::number(causalModel.getID())] = causalModel.toJsonObject(bIncludeProperties);
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonObjectResult;
}

void CCausalModelList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObjectbalanceModel = jsonValue.toObject();

        CCausalModel balanceModel;
        balanceModel.fromJsonObject(jsonObjectbalanceModel);

        /* Combine result */
        append(balanceModel);
    }
}

void CCausalModelList::fromJsonObject(const QJsonObject &jsonObject)
{
    clear();

    QStringList lstKeys = jsonObject.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObject[strKey];
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CCausalModel balanceModel;
        balanceModel.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(balanceModel);
    }
}

QDataStream& operator<<(QDataStream& out, const CCausalModelList& lstCausalModels)
{
    out << static_cast <const QList<CCausalModel>& >( lstCausalModels );
    out << lstCausalModels.getID();
    out << lstCausalModels.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CCausalModelList& lstCausalModels)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CCausalModel>& >( lstCausalModels );
    in >> lID;
    lstCausalModels.setID(lID);
    in >> mapProperties;

    lstCausalModels.setProperties(mapProperties);

    return in;
}

