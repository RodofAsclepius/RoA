#include "effectlist.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>

CEffectList::CEffectList()
    :QList<CVariableEffect>()
{
    qRegisterMetaType<CEffectList>("CEffectList");
//    qRegisterMetaTypeStreamOperators<CEffectList>("CEffectList");
}

CEffectList::CEffectList(const CEffectList& lstEffects)
    : QList<CVariableEffect>(), CPropertiesInterface()
{
    this->operator =(lstEffects);
}


CEffectList::~CEffectList()
{

}


CEffectList& CEffectList::operator=(const CEffectList& lstEffects)
{
    if (this != &lstEffects)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CVariableEffect>::operator =(lstEffects);
        mutLocker.unlock();
        setID(lstEffects.getID());
    }
    return* this;
}


bool CEffectList::operator==(const CEffectList& lstEffects)
{
    return (_lID == lstEffects.getID());
}


bool CEffectList::operator<(const CEffectList& lstEffects) const
{
    return (getID() < lstEffects.getID());
}

void CEffectList::append(const CVariableEffect &variable)
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


void CEffectList::append(const CEffectList &lstEffects)
{
    foreach(CVariableEffect variable, lstEffects)
    {
        append(variable);
    }
}


void CEffectList::replace(int iIndex, const CVariableEffect& variable)
{
    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, variable);
}


void CEffectList::replace(const CVariableEffect& variable)
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


void CEffectList::replace(const CEffectList& lstEffects)
{
    foreach(CVariableEffect variable, lstEffects)
    {
        replace(variable);
    }
}


void CEffectList::update(const CEffectList &lstEffects, bool bAppendNewVariable)
{
    /* Add or update items */
    foreach(CVariableEffect variable, lstEffects)
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


QJsonObject CEffectList::toJsonObject(bool bIncludeProperties) const
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

void CEffectList::fromJsonArray(const QJsonArray &jsonArray)
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

void CEffectList::fromJsonObject(const QJsonObject &jsonObject)
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

QStringList CEffectList::variablesNames() const
{
    QStringList lstNames;

    for(int i=0; i<count(); ++i)
    {
        lstNames << at(i).name();
    }

    return lstNames;
}

CVariableEffect CEffectList::byName(const QString &strName) const
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

int CEffectList::indexByName(const QString &strName) const
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

QDataStream& operator<<(QDataStream& out, const CEffectList& lstEffects)
{
    out << static_cast <const QList<CVariableEffect>& >( lstEffects );
    out << lstEffects.getID();
    out << lstEffects.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CEffectList& lstEffects)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CVariableEffect>& >( lstEffects );
    in >> lID;
    lstEffects.setID(lID);
    in >> mapProperties;

    lstEffects.setProperties(mapProperties);

    return in;
}

