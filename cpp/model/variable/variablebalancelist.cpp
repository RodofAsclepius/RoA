#include "variablebalancelist.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>

CVariableBalanceList::CVariableBalanceList()
    :QList<CVariableBalance>()
{
    qRegisterMetaType<CVariableBalanceList>("CVariableBalanceList");
//    qRegisterMetaTypeStreamOperators<CVariableBalanceList>("CVariableBalanceList");
}

CVariableBalanceList::CVariableBalanceList(const CVariableBalanceList& lstVariables)
    : QList<CVariableBalance>(), CPropertiesInterface()
{
    this->operator =(lstVariables);
}


CVariableBalanceList::~CVariableBalanceList()
{

}


CVariableBalanceList& CVariableBalanceList::operator=(const CVariableBalanceList& lstVariables)
{
    if (this != &lstVariables)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CVariableBalance>::operator =(lstVariables);
        mutLocker.unlock();
        setID(lstVariables.getID());
    }
    return* this;
}


bool CVariableBalanceList::operator==(const CVariableBalanceList& lstVariables)
{
    return (_lID == lstVariables.getID());
}


bool CVariableBalanceList::operator<(const CVariableBalanceList& lstVariables) const
{
    return (getID() < lstVariables.getID());
}

void CVariableBalanceList::append(const CVariableBalance &variable)
{
    QMutexLocker mutLocker(&_mutex);

    if(!contains(variable))
    {
        QList<CVariableBalance>::append(variable);
    }
    else
    {
        replace(variable);
    }
}


void CVariableBalanceList::append(const CVariableBalanceList &lstVariables)
{
    foreach(CVariableBalance variable, lstVariables)
    {
        append(variable);
    }
}


void CVariableBalanceList::replace(int iIndex, const CVariableBalance& variable)
{
    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, variable);
}


void CVariableBalanceList::replace(const CVariableBalance& variable)
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


void CVariableBalanceList::replace(const CVariableBalanceList& lstVariables)
{
    foreach(CVariableBalance variable, lstVariables)
    {
        replace(variable);
    }
}


void CVariableBalanceList::update(const CVariableBalanceList &lstVariables, bool bAppendNewVariable)
{
    /* Add or update items */
    foreach(CVariableBalance variable, lstVariables)
    {
        /* Get current one based on ID */
        int iIndex = indexOf(variable);

        if(iIndex == -1)
        {
            if(bAppendNewVariable)
            {
                append(variable);
            }
        }
        else
        {
            /* Merge all properties */
            CVariableBalance currentVariable = at(iIndex);
            currentVariable.update(variable);
            replace(iIndex, currentVariable);
        }
    }
}


QJsonObject CVariableBalanceList::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CVariableBalance variable = at(iIndex);
        jsonObjectResult[variable.name()] = variable.toJsonObject(bIncludeProperties);
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonObjectResult;
}

void CVariableBalanceList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObjectVariable = jsonValue.toObject();

        CVariableBalance variable;
        variable.fromJsonObject(jsonObjectVariable);

        /* Combine result */
        append(variable);
    }
}

void CVariableBalanceList::fromJsonObject(const QJsonObject &jsonObject)
{
    clear();

    QStringList lstKeys = jsonObject.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObject[strKey];
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CVariableBalance variable;
        variable.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(variable);
    }
}

QStringList CVariableBalanceList::variablesNames() const
{
    QStringList lstNames;

    for(int i=0; i<count(); ++i)
    {
        lstNames << at(i).name();
    }

    return lstNames;
}

CVariableBalance CVariableBalanceList::byName(const QString &strName) const
{
    for(int i=0; i<count(); ++i)
    {
        CVariableBalance variable = at(i);
        if(variable.name() == strName)
        {
            return variable;
        }
    }

    return CVariableBalance();
}

int CVariableBalanceList::indexByName(const QString &strName) const
{
    for(int i=0; i<count(); ++i)
    {
        CVariableBalance variable = at(i);
        if(variable.name() == strName)
        {
            return indexOf(variable);
        }
    }

    return -1;
}

QDataStream& operator<<(QDataStream& out, const CVariableBalanceList& lstVariables)
{
    out << static_cast <const QList<CVariableBalance>& >( lstVariables );
    out << lstVariables.getID();
    out << lstVariables.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CVariableBalanceList& lstVariables)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CVariableBalance>& >( lstVariables );
    in >> lID;
    lstVariables.setID(lID);
    in >> mapProperties;

    lstVariables.setProperties(mapProperties);

    return in;
}

