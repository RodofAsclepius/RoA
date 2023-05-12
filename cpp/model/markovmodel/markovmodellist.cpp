#include "markovmodellist.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>
#include "utils.h"

using namespace utils;

CMarkovModelList::CMarkovModelList()
    :QList<CMarkovModel>()
{
    qRegisterMetaType<CMarkovModelList>("CMarkovModelList");
//    qRegisterMetaTypeStreamOperators<CMarkovModelList>("CMarkovModelList");
}


CMarkovModelList::CMarkovModelList(const CMarkovModelList& lstMarkovModels)
    : QList<CMarkovModel>(), CPropertiesInterface()
{
    this->operator =(lstMarkovModels);
}


CMarkovModelList::~CMarkovModelList()
{

}

CMarkovModelList& CMarkovModelList::operator=(const CMarkovModelList& lstMarkovModels)
{
    if (this != &lstMarkovModels)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CMarkovModel>::operator =(lstMarkovModels);
        mutLocker.unlock();
        setID(lstMarkovModels.getID());
    }
    return* this;
}

bool CMarkovModelList::operator==(const CMarkovModelList& lstMarkovModels)
{
    return (_lID == lstMarkovModels.getID());
}

bool CMarkovModelList::operator<(const CMarkovModelList& lstMarkovModels) const
{
    return (getID() < lstMarkovModels.getID());
}

void CMarkovModelList::append(const CMarkovModel &causalModel)
{
    QMutexLocker mutLocker(&_mutex);

    if(!contains(causalModel))
    {
        QList<CMarkovModel>::append(causalModel);
    }
    else
    {
        replace(causalModel);
    }
}

void CMarkovModelList::append(const CMarkovModelList &lstMarkovModels)
{
    foreach(CMarkovModel balanceModel, lstMarkovModels)
    {
        append(balanceModel);
    }
}

void CMarkovModelList::replace(int iIndex, const CMarkovModel& balanceModel)
{
    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, balanceModel);
}

void CMarkovModelList::replace(const CMarkovModel& balanceModel)
{
    int iIndex =indexOf(balanceModel);
    if(iIndex != -1)
    {
        replace(iIndex, balanceModel);
    }
}


void CMarkovModelList::replace(const CMarkovModelList& lstMarkovModels)
{
    foreach(CMarkovModel balanceModel, lstMarkovModels)
    {
        replace(balanceModel);
    }
}

QJsonObject CMarkovModelList::toJsonObject(bool bIncludeProperties) const
{
//    DEBUG_OBJ_INFO

    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CMarkovModel causalModel = at(iIndex);
        jsonObjectResult[QString::number(causalModel.getID())] = causalModel.toJsonObject(bIncludeProperties);
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonObjectResult;
}

void CMarkovModelList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject objMarkovModel = jsonValue.toObject();

        CMarkovModel markovModel;
        markovModel.fromJsonObject(objMarkovModel);

        /* Combine result */
        append(markovModel);
    }
}

void CMarkovModelList::fromJsonObject(const QJsonObject &jsonObject)
{
    clear();

    QStringList lstKeys = jsonObject.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObject[strKey];
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CMarkovModel balanceModel;
        balanceModel.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(balanceModel);
    }
}

QDataStream& operator<<(QDataStream& out, const CMarkovModelList& lstMarkovModels)
{
    out << static_cast <const QList<CMarkovModel>& >( lstMarkovModels );
    out << lstMarkovModels.getID();
    out << lstMarkovModels.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CMarkovModelList& lstMarkovModels)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CMarkovModel>& >( lstMarkovModels );
    in >> lID;
    lstMarkovModels.setID(lID);
    in >> mapProperties;

    lstMarkovModels.setProperties(mapProperties);

    return in;
}

