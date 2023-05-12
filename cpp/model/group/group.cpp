#include "group.h"
#include "groupslist.h"
#include "utils.h"

#include <QMutexLocker>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

using namespace utils;

CGroup::CGroup()
    : CPropertiesInterface()
{
    qRegisterMetaType<CGroup>("CGroup");
//    qRegisterMetaTypeStreamOperators<CGroup>("CGroup");

    setName("All");
    setFilterQuery("TRUE");
}

CGroup::CGroup(const QString& strName, const QString& strQuery)
    : CGroup()
{
    setName(strName);
    setFilterQuery(strQuery);
}


CGroup::CGroup(const CGroup& Group)
    : CGroup()
{
    this->operator =(Group);
}

CGroup::~CGroup()
{
}

CGroup& CGroup::operator=(const CGroup& group)
{
    if (this != &group)
    {
        setName(group.name());
        setFilterQuery(group.filterQuery());
        setSize(group.size());
        setColorPrimary(group.colorPrimary());
        setColorSecondary(group.colorSecondary());
        setEnabled(group.enabled());

        setID(group.getID());
        setProperties(group.getProperties());
    }
    return *this;
}


bool CGroup::operator<(const CGroup& Group) const
{
    return (getID() < Group.getID());
}


bool CGroup::operator==(const CGroup& Group) const
{
    return (getID() == Group.getID());
}


QString CGroup::name() const
{
    return _strName;
}

void CGroup::setName(const QString& strName)
{
    QMutexLocker mutLocker(&_mutex);
    if(!strName.isEmpty())
    {
        _strName = strName;
    }
    else
    {
        _strName = QString("Group_") + QString::number(_lID);
    }
}

QString CGroup::filterQuery() const
{
    return _strFilterQuery;
}

void CGroup::setFilterQuery(const QString &strQuery)
{
    QMutexLocker mutLocker(&_mutex);
    _strFilterQuery = strQuery;
}

QJsonObject CGroup::toJsonObject(bool bIncludeProperties) const
{
//    DEBUG_OBJ_INFO

    QJsonObject jsonObject;

    jsonObject["id"] = QString::number(getID());
    jsonObject["name"] = name();
    jsonObject["size"] = size();
    jsonObject["query"] = filterQuery();
    jsonObject["colorPrimary"] = colorPrimary().name();
    jsonObject["colorSecondary"] = colorSecondary().name();
    jsonObject["enabled"] = enabled();

    if(bIncludeProperties)
    {
        jsonObject["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObject;
}


void CGroup::fromJsonObject(const QJsonObject& jsonObject)
{
//    DEBUG_OBJ_INFO

    QString strID = jsonObject["id"].toString();
    QString strName = jsonObject["name"].toString();
    QString strQuery = jsonObject["query"].toString();
    QString strColorPrimary = jsonObject["colorPrimary"].toString();
    QString strColorSecondary = jsonObject["colorSecondary"].toString();
    bool bEnabled = jsonObject["enabled"].toBool();
    int iSize = jsonObject["size"].toInt(0);

    bool ok;
    quint32 lID = strID.toUInt(&ok, 10);
    if(lID != 0)
    {
        setID(lID);
    }

    setName(strName);
    setFilterQuery(strQuery);
    QColor colorPrimary; colorPrimary.setNamedColor(strColorPrimary);
    setColorPrimary(colorPrimary);

    QColor colorSecondary; colorSecondary.setNamedColor(strColorSecondary);
    setColorSecondary(colorSecondary);

    setEnabled(bEnabled);
    setSize(iSize);

    /* Properties */
    if(jsonObject["properties"].isObject())
    {
        QJsonObject jsonObjectProperties = jsonObject["properties"].toObject();
        foreach(QString strPropertyName, jsonObjectProperties.keys())
        {
            /* Object */
            if(jsonObjectProperties[strPropertyName].isObject())
            {
                QJsonObject jsonObjectProperty = jsonObjectProperties[strPropertyName].toObject();
                QJsonDocument jsonDoc(jsonObjectProperty);
                QString strJsonData(jsonDoc.toJson(QJsonDocument::Compact));
                addProperty(strPropertyName, QVariant::fromValue(strJsonData));
            }

            /* Array */
            if(jsonObjectProperties[strPropertyName].isArray())
            {
                QJsonArray jsonArrayProperty = jsonObjectProperties[strPropertyName].toArray();
                QJsonDocument jsonDoc(jsonArrayProperty);
                QString strJsonData(jsonDoc.toJson(QJsonDocument::Compact));
                addProperty(strPropertyName, QVariant::fromValue(strJsonData));
            }

//            qDebug() << jsonObjectProperties[strPropertyName];
            /* String */
            if(jsonObjectProperties[strPropertyName].isString())
            {
                QJsonValue jsonValue = jsonObjectProperties[strPropertyName];
//                QJsonDocument jsonDoc(jsonArrayProperty);
//                QString strJsonData(jsonDoc.toJson(QJsonDocument::Compact));

                addProperty(strPropertyName, jsonValue.toString());
            }

            /* Double */
            if(jsonObjectProperties[strPropertyName].isDouble())
            {
                QJsonValue jsonValue = jsonObjectProperties[strPropertyName];
                addProperty(strPropertyName, jsonValue.toDouble());
            }
        }
    }
}

QColor CGroup::colorSecondary() const
{
    return colorSecondary_;
}

void CGroup::setColorSecondary(const QColor &colorSecondary)
{
    colorSecondary_ = colorSecondary;
}

int CGroup::size() const
{
    return _iSize;
}

void CGroup::setSize(int iSize)
{
    _iSize = iSize;
}

QColor CGroup::colorPrimary() const
{
    return clrPrimary_;
}

void CGroup::setColorPrimary(const QColor &color)
{
    clrPrimary_ = color;
}

bool CGroup::enabled() const
{
    return bEnabled_;
}

void CGroup::setEnabled(bool bEnabled)
{
    bEnabled_ = bEnabled;
}

QDataStream& operator<<(QDataStream& out, const CGroup& group)
{
    out << group.getID()
        << group.name()
        << group.filterQuery()
        << group.size()
        << group.colorPrimary().name()
        << group.colorSecondary().name()
        << group.enabled()
        << group.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CGroup& group)
{
    quint32 lID;
    QString strName;
    QString strQuery;
    int iSize;
    QString strColorNamePrimary;
    QString strColorNameSecondary;
    bool bEnabled;

    QMap<QString, QVariant> mapProperties;

    in >> lID
        >> strName
        >> strQuery
        >> iSize
        >> strColorNamePrimary
        >> strColorNameSecondary
        >> bEnabled
        >> mapProperties;

    group.setID(lID);
    group.setName(strName);
    group.setFilterQuery(strQuery);
    group.setSize(iSize);

    QColor colorPrimary; colorPrimary.setNamedColor(strColorNamePrimary);
    group.setColorPrimary(colorPrimary);

    QColor colorSecondary; colorSecondary.setNamedColor(strColorNameSecondary);
    group.setColorSecondary(colorSecondary);

    group.setEnabled(bEnabled);
    group.setProperties(mapProperties);

    return in;
}

QDebug operator<<(QDebug dbg, const CGroup& group)
{
    dbg.nospace()   << g_spacer
                    << "CGroup(" << group.getID()
                    << ", " << group.name()
                    << ", " << group.filterQuery()
                    << ", " << group.size()
                    << ", " << group.colorPrimary().name()
                    << ", " << group.colorSecondary().name()
                    << ", " << group.enabled()
                    << ")" << "\n";

        g_spacer.indent();
        dbg.nospace() << g_spacer << "Properties:" << "\n";
        QMapIterator<QString, QVariant> mapIterator(group.getProperties());
        while (mapIterator.hasNext())
        {
              mapIterator.next();
              dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
        }
        g_spacer.unindent();

    g_spacer.unindent();

    return dbg.maybeSpace();
}


