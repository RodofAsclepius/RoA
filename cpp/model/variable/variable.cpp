
#include "variable.h"
#include "utils.h"

#include <QMutexLocker>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

using namespace utils;

CVariable::CVariable(const QString& strName, const QString& strType)
    : CPropertiesInterface()
{
    qRegisterMetaType<CVariable>("CVariable");
//    qRegisterMetaTypeStreamOperators<CVariable>("CVariable");

    setName(strName);
    setType(strType);
}


CVariable::CVariable(const CVariable& variable)
    : CVariable()
{
    this->operator =(variable);
}


CVariable::~CVariable()
{
}


CVariable& CVariable::operator=(const CVariable& variable)
{
    if (this != &variable)
    {
        setName(variable.name());
        setType(variable.type());
        setLevels(variable.levels());
        setID(variable.getID());
        setProperties(variable.getProperties());
    }
    return *this;
}


bool CVariable::operator<(const CVariable& variable) const
{
    return (getID() < variable.getID());
}


bool CVariable::operator==(const CVariable& variable) const
{
    return (getID() == variable.getID());
}


QString CVariable::name() const
{
    return strName_;
}


void CVariable::setName(const QString& strName)
{
    QMutexLocker mutLocker(&_mutex);
    if(!strName.isEmpty())
    {
        strName_ = strName;
    }
    else
    {
        strName_ = QString("Variable_") + QString::number(_lID);
    }
}


QString CVariable::type() const
{
    return strType_;
}


void CVariable::setType(const QString& strType)
{
    QMutexLocker mutLocker(&_mutex);
    strType_ = strType;
}

//QMap<QString, double> CVariable::getLevels() const
//{
//    /* Decode levels of variable */
//    QVariant varData = getProperty("histogram");
//    QJsonDocument jsonDocumentHistogram = QJsonDocument::fromJson(varData.value<QString>().toUtf8());
//    QJsonObject jsonObjectHistorgram = jsonDocumentHistogram.object();

//    /* Breaks */
//    QStringList lstLabels;

//    QJsonObject jsonObjectBreaks = jsonDocumentHistogram.object();
//    QJsonArray jsonArrayDataBreaks = jsonObjectBreaks["breaks"].toArray();

//    /* factor */
//    if(type() == "factor")
//    {
//        for(int i=0; i < jsonArrayDataBreaks.count(); ++i)
//        {
//            QJsonValue jsonValue = jsonArrayDataBreaks.at(i);
//            lstLabels << QString("%1").arg(jsonValue.toDouble());
//        }
//    }
//    else
//    {
//        for(int i=0; i < jsonArrayDataBreaks.count()-1; ++i)
//        {
//            QJsonValue jsonValue = jsonArrayDataBreaks.at(i);
//            QJsonValue jsonValueNext = jsonArrayDataBreaks.at(i+1);

//            lstLabels << QString("%1-%2").arg(jsonValue.toDouble()).arg(jsonValueNext.toDouble());
//        }
//    }

//    /* counts */
//    QVector<double> valueData;

//    QJsonArray jsonArrayDataCounts = jsonObjectHistorgram["counts"].toArray();
////    QJsonArray jsonArrayDataCounts = jsonObjectHistorgram["density"].toArray();
//    QJsonArray::iterator it2 = jsonArrayDataCounts.begin();
//    double dSumCounts = 0;
//    while(it2 != jsonArrayDataCounts.end())
//    {
//        QJsonValue jsonValue = *it2;

//        valueData << jsonValue.toDouble();
//        dSumCounts += jsonValue.toDouble();
//        ++it2;
//    }

//    QMap<QString, double> result;
//    for(int i=0; i<lstLabels.count(); ++i)
//    {
//        QString strLabel = lstLabels.at(i);
//        result[strLabel] = valueData.at(i);
//    }

//    return result;
//}


void CVariable::update(const CVariable &variable)
{
    /* Is this an updated version ? */
    if(variable.getID() != getID())
    {
        return;
    }

    /* Update the basic info */
    setName(variable.name());
    setType(variable.type());
    setLevels(variable.levels());

    /* Just add / override properties, nothing is removed! */
    QMapIterator<QString, QVariant> it(variable.getProperties());
    while(it.hasNext())
    {
        it.next();
        QString strPropertyName = it.key();
        QVariant varProperty = it.value();

        addProperty(strPropertyName, varProperty);
    }
}

QJsonObject CVariable::toJsonObject(bool bIncludeProperties) const
{
//    DEBUG_OBJ_INFO

    QJsonObject jsonObject;

    jsonObject["id"] = QString::number(getID());
    jsonObject["name"] = name();
    jsonObject["type"] = type();
    QJsonArray arLevels = QJsonArray::fromStringList(levels());
    jsonObject["levels"] = arLevels;

    if(bIncludeProperties)
    {
        jsonObject["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObject;
}


void CVariable::fromJsonObject(const QJsonObject& jsonObject)
{
//    DEBUG_OBJ_INFO

    QString strID = jsonObject["id"].toString();
    QString strName = jsonObject["name"].toString();
    QString strType = jsonObject["type"].toString();
    QJsonArray arLevels = jsonObject["levels"].toArray();

    bool ok;
    quint32 lID = strID.toUInt(&ok, 10);
    if(lID != 0)
    {
        setID(lID);
    }

    setName(strName);
    setType(strType);

    /* Levels */
    QStringList lstLevels;
    foreach(QJsonValue valLevel, arLevels)
    {
        lstLevels.append(valLevel.toString());
    }
    setLevels(lstLevels);

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

QStringList CVariable::levels() const
{
    return lstLevels_;
}

void CVariable::setLevels(const QStringList &lstLevels)
{
    lstLevels_ = lstLevels;
}

QDataStream& operator<<(QDataStream& out, const CVariable& variable)
{
    out << variable.getID() << variable.name()
        << variable.type()
        << variable.levels()
        << variable.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CVariable& variable)
{
    quint32 lID;
    QString strName;
    QString strType;
    QStringList lstLevels;
    QMap<QString, QVariant> mapProperties;

    in >> lID >> strName >> strType >> lstLevels >> mapProperties;
    variable.setID(lID);
    variable.setName(strName);
    variable.setType(strType);
    variable.setLevels(lstLevels);
    variable.setProperties(mapProperties);

    return in;
}


QDebug operator<<(QDebug dbg, const CVariable& variable)
{
    dbg.nospace()   << g_spacer
                    << "CVariable(" << variable.getID()
                    << ", " << variable.name()
                    << ", " << variable.type()
                    << ", " << variable.levels()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(variable.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}


