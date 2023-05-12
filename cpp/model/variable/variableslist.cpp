#include "variableslist.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>

bool VariableNameLessThan(const CVariable &v1, const CVariable &v2)
{
    return v1.name() < v2.name();
}

CVariablesList::CVariablesList()
    :QList<CVariable>()
{
    qRegisterMetaType<CVariablesList>("CVariableList");
//    qRegisterMetaTypeStreamOperators<CVariablesList>("CVariableList");
}

CVariablesList::CVariablesList(const CVariablesList& lstVariables)
    : QList<CVariable>(), CPropertiesInterface()
{
    this->operator =(lstVariables);
}

CVariablesList::~CVariablesList()
{

}

CVariablesList& CVariablesList::operator=(const CVariablesList& lstVariables)
{
    if (this != &lstVariables)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CVariable>::operator =(lstVariables);
        mutLocker.unlock();
        setID(lstVariables.getID());
    }
    return* this;
}


bool CVariablesList::operator==(const CVariablesList& lstVariables)
{
    return (_lID == lstVariables.getID());
}


bool CVariablesList::operator<(const CVariablesList& lstVariables) const
{
    return (getID() < lstVariables.getID());
}

void CVariablesList::append(const CVariable &variable)
{
    QMutexLocker mutLocker(&_mutex);

    if(!contains(variable))
    {
        QList<CVariable>::append(variable);
    }
    else
    {
        replace(variable);
    }
}


void CVariablesList::append(const CVariablesList &lstVariables)
{
    foreach(CVariable variable, lstVariables)
    {
        append(variable);
    }
}


void CVariablesList::replace(int iIndex, const CVariable& variable)
{
    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, variable);
}


void CVariablesList::replace(const CVariable& variable)
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


void CVariablesList::replace(const CVariablesList& lstVariables)
{
    foreach(CVariable variable, lstVariables)
    {
        replace(variable);
    }
}


void CVariablesList::update(const CVariablesList &lstVariables, bool bAppendNewVariable)
{
    /* Add or update items */
    foreach(CVariable variable, lstVariables)
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
            CVariable currentVariable = at(iIndex);
            currentVariable.update(variable);
            replace(iIndex, currentVariable);
        }
    }
}


QJsonObject CVariablesList::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CVariable variable = at(iIndex);
        jsonObjectResult[variable.name()] = variable.toJsonObject(bIncludeProperties);
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonObjectResult;
}

void CVariablesList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObjectVariable = jsonValue.toObject();

        CVariable variable;
        variable.fromJsonObject(jsonObjectVariable);

        /* Combine result */
        append(variable);
    }
}

void CVariablesList::fromJsonObject(const QJsonObject &jsonObject)
{
    clear();

    QStringList lstKeys = jsonObject.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObject[strKey];
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CVariable variable;
        variable.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(variable);
    }
}

QStringList CVariablesList::variablesNames() const
{
    QStringList lstNames;

    for(int i=0; i<count(); ++i)
    {
        lstNames << at(i).name();
    }

    return lstNames;
}

CVariable CVariablesList::byName(const QString &strName) const
{
    for(int i=0; i<count(); ++i)
    {
        CVariable variable = at(i);
        if(variable.name() == strName)
        {
            return variable;
        }
    }

    return CVariable();
}

int CVariablesList::indexByName(const QString &strName) const
{
    for(int i=0; i<count(); ++i)
    {
        CVariable variable = at(i);
        if(variable.name() == strName)
        {
            return indexOf(variable);
        }
    }

    return -1;
}

QDataStream& operator<<(QDataStream& out, const CVariablesList& lstVariables)
{
    out << static_cast <const QList<CVariable>& >( lstVariables );
    out << lstVariables.getID();
    out << lstVariables.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CVariablesList& lstVariables)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CVariable>& >( lstVariables );
    in >> lID;
    lstVariables.setID(lID);
    in >> mapProperties;

    lstVariables.setProperties(mapProperties);

    return in;
}

