#include "adjustmentsetlist.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>
#include "utils.h"

using namespace utils;

CAdjustmentSetList::CAdjustmentSetList()
    :QList<CAdjustmentSet>()
{
    qRegisterMetaType<CAdjustmentSetList>("CAdjustmentSetList");
//    qRegisterMetaTypeStreamOperators<CAdjustmentSetList>("CAdjustmentSetList");
}


CAdjustmentSetList::CAdjustmentSetList(const CAdjustmentSetList& lstCausalModels)
    : QList<CAdjustmentSet>(), CPropertiesInterface()
{
    this->operator =(lstCausalModels);
}


CAdjustmentSetList::~CAdjustmentSetList()
{

}


CAdjustmentSetList& CAdjustmentSetList::operator=(const CAdjustmentSetList& lstCausalModels)
{
    if (this != &lstCausalModels)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CAdjustmentSet>::operator =(lstCausalModels);
        mutLocker.unlock();
        setID(lstCausalModels.getID());
    }
    return* this;
}

bool CAdjustmentSetList::operator==(const CAdjustmentSetList& lstCausalModels)
{
    return (_lID == lstCausalModels.getID());
}

bool CAdjustmentSetList::operator<(const CAdjustmentSetList& lstCausalModels) const
{
    return (getID() < lstCausalModels.getID());
}

void CAdjustmentSetList::append(const CAdjustmentSet &causalModel)
{
    QMutexLocker mutLocker(&_mutex);

    if(!contains(causalModel))
    {
        QList<CAdjustmentSet>::append(causalModel);
    }
    else
    {
        replace(causalModel);
    }
}

void CAdjustmentSetList::append(const CAdjustmentSetList &lstCausalModels)
{
    foreach(CAdjustmentSet adjustmentSet, lstCausalModels)
    {
        append(adjustmentSet);
    }
}

void CAdjustmentSetList::replace(int iIndex, const CAdjustmentSet& adjustmentSet)
{
    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, adjustmentSet);
}

void CAdjustmentSetList::replace(const CAdjustmentSet& adjustmentSet)
{
    int iIndex =indexOf(adjustmentSet);
    if(iIndex != -1)
    {
        replace(iIndex, adjustmentSet);
    }
}


void CAdjustmentSetList::replace(const CAdjustmentSetList& lstCausalModels)
{
    foreach(CAdjustmentSet adjustmentSet, lstCausalModels)
    {
        replace(adjustmentSet);
    }
}

QJsonObject CAdjustmentSetList::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CAdjustmentSet causalModel = at(iIndex);
        jsonObjectResult[QString::number(causalModel.getID())] = causalModel.toJsonObject(bIncludeProperties);
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonObjectResult;
}

void CAdjustmentSetList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObjectadjustmentSet = jsonValue.toObject();

        CAdjustmentSet adjustmentSet;
        adjustmentSet.fromJsonObject(jsonObjectadjustmentSet);

        /* Combine result */
        append(adjustmentSet);
    }
}

void CAdjustmentSetList::fromJsonObject(const QJsonObject &jsonObject)
{
    clear();

    QStringList lstKeys = jsonObject.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObject[strKey];
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CAdjustmentSet adjustmentSet;
        adjustmentSet.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(adjustmentSet);
    }
}

QDataStream& operator<<(QDataStream& out, const CAdjustmentSetList& lstCausalModels)
{
    out << static_cast <const QList<CAdjustmentSet>& >( lstCausalModels );
    out << lstCausalModels.getID();
    out << lstCausalModels.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CAdjustmentSetList& lstCausalModels)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CAdjustmentSet>& >( lstCausalModels );
    in >> lID;
    lstCausalModels.setID(lID);
    in >> mapProperties;

    lstCausalModels.setProperties(mapProperties);

    return in;
}

