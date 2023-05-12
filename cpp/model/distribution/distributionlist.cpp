#include "distributionlist.h"
#include "utils.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>

using namespace utils;

CDistributionsList::CDistributionsList()
    :QList<CDistribution>()
{
    qRegisterMetaType<CDistributionsList>("CDistributionsList");
//    qRegisterMetaTypeStreamOperators<CDistributionsList>("CDistributionsList");
}


CDistributionsList::CDistributionsList(const CDistributionsList& lstDistributions)
    : QList<CDistribution>(), CPropertiesInterface()
{
    this->operator =(lstDistributions);
}


CDistributionsList::~CDistributionsList()
{

}


CDistributionsList& CDistributionsList::operator=(const CDistributionsList& lstDistributions)
{
    if (this != &lstDistributions)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CDistribution>::operator =(lstDistributions);
        mutLocker.unlock();
        setID(lstDistributions.getID());
    }
    return* this;
}


bool CDistributionsList::operator==(const CDistributionsList& lstDistributions)
{
    return (_lID == lstDistributions.getID());
}


bool CDistributionsList::operator<(const CDistributionsList& lstDistributions) const
{
    return (getID() < lstDistributions.getID());
}

void CDistributionsList::append(const CDistribution &variable)
{
    QMutexLocker mutLocker(&_mutex);

    if(!contains(variable))
    {
        QList<CDistribution>::append(variable);
    }
    else
    {
        replace(variable);
    }
}


void CDistributionsList::append(const CDistributionsList &lstDistributions)
{
    foreach(CDistribution variable, lstDistributions)
    {
        append(variable);
    }
}


void CDistributionsList::replace(int iIndex, const CDistribution& variable)
{
//    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, variable);
}


void CDistributionsList::replace(const CDistribution& variable)
{
    int iIndex =indexOf(variable);
    if(iIndex != -1)
    {
        replace(iIndex, variable);
    }
    else
    {
        append(variable);
    }
}


void CDistributionsList::replace(const CDistributionsList& lstDistributions)
{
    foreach(CDistribution variable, lstDistributions)
    {
        replace(variable);
    }
}


void CDistributionsList::update(const CDistributionsList &lstDistributions, bool bAppendNewDistribution)
{
    /* Add or update items */
    foreach(CDistribution distribution, lstDistributions)
    {
        /* Get current one based on ID */
        int iIndex = indexOf(distribution);

        if(iIndex == -1)
        {
            if(bAppendNewDistribution)
            {
                append(distribution);
            }
        }
        else
        {
            /* Merge all properties */
            CDistribution currentDistribution = at(iIndex);
//            currentDistribution.update(distribution);
            replace(iIndex, currentDistribution);
        }
    }
}

//QJsonArray CDistributionsList::toJsonArray(bool bIncludeProperties) const
//{
//    QJsonArray jsonArrayResult;

//    for(int iIndex = 0; iIndex < count(); ++iIndex)
//    {
//        CDistribution distribution = at(iIndex);
//        jsonArrayResult.append(distribution.toJsonObject(bIncludeProperties));
//    }

////    if(bIncludeProperties)
////    {
////        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
////    }

//    return jsonArrayResult;
//}


QJsonObject CDistributionsList::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CDistribution distribution = at(iIndex);
        jsonObjectResult[QString::number(distribution.getID())] = distribution.toJsonObject(bIncludeProperties);
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonObjectResult;
}

void CDistributionsList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObjectVariable = jsonValue.toObject();

        CDistribution variable;
        variable.fromJsonObject(jsonObjectVariable);

        /* Combine result */
        append(variable);
    }
}

void CDistributionsList::fromJsonObject(const QJsonObject &jsonObject)
{
    clear();

    QStringList lstKeys = jsonObject.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObject[strKey];
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CDistribution variable;
        variable.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(variable);
    }
}

//QStringList CDistributionsList::variablesNames() const
//{
//    QStringList lstNames;

//    for(int i=0; i<count(); ++i)
//    {
//        lstNames << at(i).name();
//    }

//    return lstNames;
//}

//CDistribution CDistributionsList::byName(const QString &strName) const
//{
//    for(int i=0; i<count(); ++i)
//    {
//        CDistribution variable = at(i);
//        if(variable.name() == strName)
//        {
//            return variable;
//        }
//    }

//    return CDistribution();
//}

//int CDistributionsList::indexByName(const QString &strName) const
//{
//    for(int i=0; i<count(); ++i)
//    {
//        CDistribution variable = at(i);
//        if(variable.name() == strName)
//        {
//            return indexOf(variable);
//        }
//    }

//    return -1;
//}

QDataStream& operator<<(QDataStream& out, const CDistributionsList& lstDistributions)
{
    out << static_cast <const QList<CDistribution>& >( lstDistributions );
    out << lstDistributions.getID();
    out << lstDistributions.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CDistributionsList& lstDistributions)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CDistribution>& >( lstDistributions );
    in >> lID;
    lstDistributions.setID(lID);
    in >> mapProperties;

    lstDistributions.setProperties(mapProperties);

    return in;
}

