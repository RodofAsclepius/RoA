#include "groupslist.h"
#include "utils.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>

CGroupsList::CGroupsList()
    :QList<CGroup>()
{
    qRegisterMetaType<CGroupsList>("CGroupsList");
//    qRegisterMetaTypeStreamOperators<CGroupsList>("CGroupsList");
}

CGroupsList::CGroupsList(const CGroupsList& lstGroups)
    : QList<CGroup>(), CPropertiesInterface()
{
    this->operator =(lstGroups);
}


CGroupsList::~CGroupsList()
{

}


CGroupsList& CGroupsList::operator=(const CGroupsList& lstGroups)
{
    if (this != &lstGroups)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CGroup>::operator =(lstGroups);
        mutLocker.unlock();
        setID(lstGroups.getID());
    }
    return* this;
}


bool CGroupsList::operator==(const CGroupsList& lstGroups)
{
    return (_lID == lstGroups.getID());
}


bool CGroupsList::operator<(const CGroupsList& lstGroups) const
{
    return (getID() < lstGroups.getID());
}

void CGroupsList::append(const CGroup &group)
{
    QMutexLocker mutLocker(&_mutex);

    if(!contains(group))
    {
        QList<CGroup>::append(group);
    }
    else
    {
        replace(group);
    }
}


void CGroupsList::append(const CGroupsList &lstGroups)
{
    foreach(CGroup group, lstGroups)
    {
        append(group);
    }
}


void CGroupsList::replace(int iIndex, const CGroup& group)
{
    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, group);
}


void CGroupsList::replace(const CGroup& group)
{
    int iIndex = indexOf(group);
    if(iIndex != -1)
    {
        replace(iIndex, group);
    }
}


void CGroupsList::replace(const CGroupsList& lstGroups)
{
    foreach(CGroup group, lstGroups)
    {
        replace(group);
    }
}

QJsonObject CGroupsList::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CGroup group = at(iIndex);
        jsonObjectResult[group.name()] = group.toJsonObject(bIncludeProperties);
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonObjectResult;
}

QJsonArray CGroupsList::toJsonArray(bool bIncludeProperties) const
{
    QJsonArray jsonArrayResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CGroup group = at(iIndex);
        jsonArrayResult.append(group.toJsonObject(bIncludeProperties));
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonArrayResult;
}

void CGroupsList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CGroup group;
        group.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(group);
    }
}

void CGroupsList::fromJsonObject(const QJsonObject &jsonObj)
{
    clear();

    QStringList lstKeys = jsonObj.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObj[strKey];
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CGroup group;
        group.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(group);
    }
}

CGroup CGroupsList::byName(const QString &strName) const
{
    for(int i=0; i<count(); ++i)
    {
        CGroup group = at(i);
        if(group.name() == strName)
        {
            return group;
        }
    }

    return CGroup();
}

CGroup CGroupsList::byID(quint32 id) const
{
    for(int i=0; i<count(); ++i)
    {
        CGroup group = at(i);
        if(group.getID() == id)
        {
            return group;
        }
    }

    return CGroup();
}

QStringList CGroupsList::groupsNames() const
{
    QStringList lstNames;

    for(int i=0; i<count(); ++i)
    {
        lstNames << at(i).name();
    }

    return lstNames;
}

CGroupsList CGroupsList::groupsEnabled() const
{
    CGroupsList lstGroupsEnabled;

    foreach(CGroup group, *this)
    {
        if(group.enabled())
        {
            lstGroupsEnabled.append(group);
        }
    }

    return lstGroupsEnabled;
}

CGroupsList CGroupsList::groupsDisabled() const
{
    CGroupsList lstGroupsDisabled;

    foreach(CGroup group, *this)
    {
        if(!group.enabled())
        {
            lstGroupsDisabled.append(group);
        }
    }

    return lstGroupsDisabled;
}

QDataStream& operator<<(QDataStream& out, const CGroupsList& lstGroups)
{
    out << static_cast <const QList<CGroup>& >( lstGroups );
    out << lstGroups.getID();
    out << lstGroups.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CGroupsList& lstGroups)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CGroup>& >( lstGroups );
    in >> lID;
    in >> mapProperties;

    lstGroups.setID(lID);
    lstGroups.setProperties(mapProperties);

    return in;
}

