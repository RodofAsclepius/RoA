#include "variableeffectlist.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>

CVariableEffectList::CVariableEffectList()
    :QList<CVariableEffect>()
{
    qRegisterMetaType<CVariableEffectList>("CVariableEffectList");
//    qRegisterMetaTypeStreamOperators<CVariableEffectList>("CVariableEffectList");
}

CVariableEffectList::CVariableEffectList(const CVariableEffectList& lstVariables)
    : QList<CVariableEffect>(), CPropertiesInterface()
{
    this->operator =(lstVariables);
}


CVariableEffectList::~CVariableEffectList()
{

}


CVariableEffectList& CVariableEffectList::operator=(const CVariableEffectList& lstVariables)
{
    if (this != &lstVariables)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CVariableEffect>::operator =(lstVariables);
        mutLocker.unlock();
        setID(lstVariables.getID());
    }
    return* this;
}


bool CVariableEffectList::operator==(const CVariableEffectList& lstVariables)
{
    return (_lID == lstVariables.getID());
}


bool CVariableEffectList::operator<(const CVariableEffectList& lstVariables) const
{
    return (getID() < lstVariables.getID());
}

void CVariableEffectList::append(const CVariableEffect &variable)
{
    QMutexLocker mutLocker(&_mutex);

    if(!contains(variable))
    {
        QList<CVariableEffect>::append(variable);
    }
    else
    {
        replace(variable);
    }
}


void CVariableEffectList::append(const CVariableEffectList &lstVariables)
{
    foreach(CVariableEffect variable, lstVariables)
    {
        append(variable);
    }
}


void CVariableEffectList::replace(int iIndex, const CVariableEffect& variable)
{
    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, variable);
}


void CVariableEffectList::replace(const CVariableEffect& variable)
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


void CVariableEffectList::replace(const CVariableEffectList& lstVariables)
{
    foreach(CVariableEffect variable, lstVariables)
    {
        replace(variable);
    }
}


void CVariableEffectList::update(const CVariableEffectList &lstVariables, bool bAppendNewVariable)
{
    /* Add or update items */
    foreach(CVariableEffect variable, lstVariables)
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
            CVariableEffect currentVariable = at(iIndex);
            currentVariable.update(variable);
            replace(iIndex, currentVariable);
        }
    }
}


QJsonObject CVariableEffectList::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CVariableEffect variable = at(iIndex);
        jsonObjectResult[variable.name()] = variable.toJsonObject(bIncludeProperties);
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonObjectResult;
}

void CVariableEffectList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObjectVariable = jsonValue.toObject();

        CVariableEffect variable;
        variable.fromJsonObject(jsonObjectVariable);

        /* Combine result */
        append(variable);
    }
}

void CVariableEffectList::fromJsonObject(const QJsonObject &jsonObject)
{
    clear();

    QStringList lstKeys = jsonObject.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObject[strKey];
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CVariableEffect variable;
        variable.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(variable);
    }
}

QStringList CVariableEffectList::variablesNames() const
{
    QStringList lstNames;

    for(int i=0; i<count(); ++i)
    {
        lstNames << at(i).name();
    }

    return lstNames;
}

CVariableEffect CVariableEffectList::byName(const QString &strName) const
{
    for(int i=0; i<count(); ++i)
    {
        CVariableEffect variable = at(i);
        if(variable.name() == strName)
        {
            return variable;
        }
    }

    return CVariableEffect();
}

int CVariableEffectList::indexByName(const QString &strName) const
{
    for(int i=0; i<count(); ++i)
    {
        CVariableEffect variable = at(i);
        if(variable.name() == strName)
        {
            return indexOf(variable);
        }
    }

    return -1;
}

QDataStream& operator<<(QDataStream& out, const CVariableEffectList& lstVariables)
{
    out << static_cast <const QList<CVariableEffect>& >( lstVariables );
    out << lstVariables.getID();
    out << lstVariables.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CVariableEffectList& lstVariables)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CVariableEffect>& >( lstVariables );
    in >> lID;
    lstVariables.setID(lID);
    in >> mapProperties;

    lstVariables.setProperties(mapProperties);

    return in;
}

