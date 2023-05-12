#include "adjustmentmodellist.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>
#include "utils.h"

using namespace utils;

CAdjustmentModelList::CAdjustmentModelList()
    :QList<CAdjustmentModel>()
{
    qRegisterMetaType<CAdjustmentModelList>("CAdjustmentModelList");
//    qRegisterMetaTypeStreamOperators<CAdjustmentModelList>("CAdjustmentModelList");
}


CAdjustmentModelList::CAdjustmentModelList(const CAdjustmentModelList& lstCausalModels)
    : QList<CAdjustmentModel>(), CPropertiesInterface()
{
    this->operator =(lstCausalModels);
}


CAdjustmentModelList::~CAdjustmentModelList()
{

}


CAdjustmentModelList& CAdjustmentModelList::operator=(const CAdjustmentModelList& lstCausalModels)
{
    if (this != &lstCausalModels)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CAdjustmentModel>::operator =(lstCausalModels);
        mutLocker.unlock();
        setID(lstCausalModels.getID());
    }
    return* this;
}

bool CAdjustmentModelList::operator==(const CAdjustmentModelList& lstCausalModels)
{
    return (_lID == lstCausalModels.getID());
}

bool CAdjustmentModelList::operator<(const CAdjustmentModelList& lstCausalModels) const
{
    return (getID() < lstCausalModels.getID());
}

void CAdjustmentModelList::append(const CAdjustmentModel& adjustmentModel)
{
//    QMutexLocker mutLocker(&_mutex);

    if(!contains(adjustmentModel))
    {
        QList<CAdjustmentModel>::append(adjustmentModel);
    }
    else
    {
        replace(adjustmentModel);
    }
}

void CAdjustmentModelList::append(const CAdjustmentModelList &lstAdjustmentModelList)
{
    foreach(CAdjustmentModel adjustmentModel, lstAdjustmentModelList)
    {
        append(adjustmentModel);
    }
}

void CAdjustmentModelList::replace(int iIndex, const CAdjustmentModel& adjustmentModel)
{
//    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, adjustmentModel);
}

void CAdjustmentModelList::replace(const CAdjustmentModel& adjustmentModel)
{
    int iIndex =indexOf(adjustmentModel);
    if(iIndex != -1)
    {
        replace(iIndex, adjustmentModel);
    }
}


void CAdjustmentModelList::replace(const CAdjustmentModelList& lstAdjustmentModelList)
{
    foreach(CAdjustmentModel adjustmentModel, lstAdjustmentModelList)
    {
        replace(adjustmentModel);
    }
}

QJsonObject CAdjustmentModelList::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CAdjustmentModel adjustmentModel = at(iIndex);
        jsonObjectResult[QString::number(adjustmentModel.getID())] = adjustmentModel.toJsonObject(bIncludeProperties);
    }

    if(bIncludeProperties)
    {
        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObjectResult;
}

void CAdjustmentModelList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValueAdjustmentModel = jsonArray.at(i);
        if(jsonValueAdjustmentModel.isObject())
        {
            CAdjustmentModel adjustmentModel;
            adjustmentModel.fromJsonObject(jsonValueAdjustmentModel.toObject());

            /* Combine result */
            append(adjustmentModel);
        }
    }
}

void CAdjustmentModelList::fromJsonObject(const QJsonObject &jsonObject)
{
    clear();

    QStringList lstKeys = jsonObject.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObject[strKey];
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CAdjustmentModel adjustmentModel;
        adjustmentModel.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(adjustmentModel);
    }
}

QDataStream& operator<<(QDataStream& out, const CAdjustmentModelList& lstAdjustmentModels)
{
    out << static_cast <const QList<CAdjustmentModel>& >( lstAdjustmentModels );
    out << lstAdjustmentModels.getID();
    out << lstAdjustmentModels.getProperties();


    return out;
}


QDataStream& operator>>(QDataStream& in, CAdjustmentModelList& lstAdjustmentModels)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CAdjustmentModel>& >( lstAdjustmentModels );
    in >> lID;
    lstAdjustmentModels.setID(lID);
    in >> mapProperties;

    lstAdjustmentModels.setProperties(mapProperties);

    return in;
}

