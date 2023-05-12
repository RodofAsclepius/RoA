#include "balancemodelslist.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>

CBalanceModelsList::CBalanceModelsList()
    :QList<CBalanceModel>()
{
    qRegisterMetaType<CBalanceModelsList>("CBalanceModelList");
//    qRegisterMetaTypeStreamOperators<CBalanceModelsList>("CBalanceModelList");
}


CBalanceModelsList::CBalanceModelsList(const CBalanceModelsList& lstBalanceModels)
    : QList<CBalanceModel>(), CPropertiesInterface()
{
    this->operator =(lstBalanceModels);
}


CBalanceModelsList::~CBalanceModelsList()
{

}


CBalanceModelsList& CBalanceModelsList::operator=(const CBalanceModelsList& lstBalanceModels)
{
    if (this != &lstBalanceModels)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CBalanceModel>::operator =(lstBalanceModels);
        mutLocker.unlock();
        setID(lstBalanceModels.getID());
    }
    return* this;
}


bool CBalanceModelsList::operator==(const CBalanceModelsList& lstBalanceModels)
{
    return (_lID == lstBalanceModels.getID());
}


bool CBalanceModelsList::operator<(const CBalanceModelsList& lstBalanceModels) const
{
    return (getID() < lstBalanceModels.getID());
}

void CBalanceModelsList::append(const CBalanceModel &balanceModel)
{
    QMutexLocker mutLocker(&_mutex);

    if(!contains(balanceModel))
    {
        QList<CBalanceModel>::append(balanceModel);
    }
    else
    {
        replace(balanceModel);
    }
}


void CBalanceModelsList::append(const CBalanceModelsList &lstBalanceModels)
{
    foreach(CBalanceModel balanceModel, lstBalanceModels)
    {
        append(balanceModel);
    }
}


void CBalanceModelsList::replace(int iIndex, const CBalanceModel& balanceModel)
{
    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, balanceModel);
}


void CBalanceModelsList::replace(const CBalanceModel& balanceModel)
{
    int iIndex =indexOf(balanceModel);
    if(iIndex != -1)
    {
        replace(iIndex, balanceModel);
    }
}


void CBalanceModelsList::replace(const CBalanceModelsList& lstBalanceModels)
{
    foreach(CBalanceModel balanceModel, lstBalanceModels)
    {
        replace(balanceModel);
    }
}


//void CBalanceModelsList::update(const CBalanceModelsList &lstBalanceModels, bool bAppendNewbalanceModel)
//{
//    /* Add or update items */
//    foreach(CBalanceModel balanceModel, lstBalanceModels)
//    {
//        /* Get current one based on ID */
//        int iIndex = indexOf(balanceModel);

//        if(iIndex == -1)
//        {
//            if(bAppendNewbalanceModel)
//            {
//                append(balanceModel);
//            }
//        }
//        else
//        {
//            /* Merge all properties */
//            CBalanceModel currentbalanceModel = at(iIndex);
//            currentbalanceModel.update(balanceModel);
//            replace(iIndex, currentbalanceModel);
//        }
//    }
//}


QJsonObject CBalanceModelsList::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CBalanceModel balanceModel = at(iIndex);
        jsonObjectResult[QString::number(balanceModel.getID())] = balanceModel.toJsonObject(bIncludeProperties);
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonObjectResult;
}

void CBalanceModelsList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObjectbalanceModel = jsonValue.toObject();

        CBalanceModel balanceModel;
        balanceModel.fromJsonObject(jsonObjectbalanceModel);

        /* Combine result */
        append(balanceModel);
    }
}

void CBalanceModelsList::fromJsonObject(const QJsonObject &jsonObject)
{
    clear();

    QStringList lstKeys = jsonObject.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObject[strKey];
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CBalanceModel balanceModel;
        balanceModel.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(balanceModel);
    }
}

QDataStream& operator<<(QDataStream& out, const CBalanceModelsList& lstBalanceModels)
{
    out << static_cast <const QList<CBalanceModel>& >( lstBalanceModels );
    out << lstBalanceModels.getID();
    out << lstBalanceModels.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CBalanceModelsList& lstBalanceModels)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CBalanceModel>& >( lstBalanceModels );
    in >> lID;
    lstBalanceModels.setID(lID);
    in >> mapProperties;

    lstBalanceModels.setProperties(mapProperties);

    return in;
}

