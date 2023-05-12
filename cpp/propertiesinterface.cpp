
#include "propertiesinterface.h"
#include "utils.h"


using namespace utils;

quint32 CPropertiesInterface::_counter = 0;

CPropertiesInterface::CPropertiesInterface(QObject *parent)
    : QObject(parent),
      _lID(_counter++)
{

}

CPropertiesInterface::CPropertiesInterface(const CPropertiesInterface &propertiesInterface)
    : CPropertiesInterface()
{
    this->operator =(propertiesInterface);
}


CPropertiesInterface::~CPropertiesInterface()
{

}

quint32 CPropertiesInterface::getID() const
{
    return _lID;
}


CPropertiesInterface &CPropertiesInterface::operator=(const CPropertiesInterface &propertiesInterface)
{
//    DEBUG_OBJ_INFO

    if (this != &propertiesInterface)
    {
        setID(propertiesInterface.getID());
        setProperties(propertiesInterface.getProperties());
    }
    return *this;
}


bool CPropertiesInterface::operator==(const CPropertiesInterface &propertiesInterface) const
{
    return (getID() == propertiesInterface.getID());
}


bool CPropertiesInterface::operator<(const CPropertiesInterface &propertiesInterface) const
{
    return (getID() < propertiesInterface.getID());
}


void CPropertiesInterface::addProperty(const QString& strName, const QVariant& property)
{
    QMutexLocker mutLocker(&_mutex);

    _mapProperties.insert(strName, property);
}


void CPropertiesInterface::addProperties(const QStringList& lstStrings, const QList<QVariant>& lstProperties)
{
    Q_ASSERT(lstStrings.count() == lstProperties.count());

    for(int iIndex=0; iIndex<lstStrings.count(); ++iIndex)
    {
        addProperty(lstStrings.at(iIndex), lstProperties.at(iIndex));
    }
}


void CPropertiesInterface::setProperties(const QVariantMap& mapProperties)
{
    QMutexLocker mutLocker(&_mutex);

    _mapProperties = mapProperties;
}


QJsonObject CPropertiesInterface::getJsonObjectProperty(const QString& strName) const
{
    QString strJsonData = getProperty<QString>(strName);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(strJsonData.toUtf8());

    QJsonObject jsonObject = jsonDocument.object();

    return jsonObject;
}


QJsonArray CPropertiesInterface::getJsonArrayProperty(const QString& strName) const
{
    QString strJsonData = getProperty<QString>(strName);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(strJsonData.toUtf8());

    QJsonArray jsonArray = jsonDocument.array();

    return jsonArray;
}


QVariantMap CPropertiesInterface::getProperties() const
{
    return _mapProperties;
}


void CPropertiesInterface::removeProperty(const QString& strName)
{
    QMutexLocker mutLocker(&_mutex);
    _mapProperties.remove(strName);
}


void CPropertiesInterface::removeAllProperties()
{
    QMutexLocker mutLocker(&_mutex);
    _mapProperties.clear();
}


void CPropertiesInterface::propertiesFromJsonObject(QJsonObject objProperties)
{
//    DEBUG_OBJ_INFO

    /* Properties */
    foreach(QString strPropertyName, objProperties.keys())
    {
        QJsonValue jsonValue = objProperties.value(strPropertyName);

        QString strPropertyValue = jsonValue.toString();
        if(!jsonValue.isString())
        {
            strPropertyValue = QString::number(jsonValue.toDouble());
        }
        addProperty(strPropertyName, strPropertyValue);
    }
}

void CPropertiesInterface::propertiesToJsonObject() const
{

}


void CPropertiesInterface::setID(quint32 lID)
{
    QMutexLocker mutLocker(&_mutex);
    _lID = lID;

    if(_counter <= _lID)
    {
        _counter = _lID + 1;
    }
}


QJsonObject VariantMaptoJsonObject(const QVariantMap &mapProperties)
{
    QJsonObject jsonObject;

    QMapIterator<QString, QVariant> mapIterator(mapProperties);
    while(mapIterator.hasNext())
    {
        mapIterator.next();
        QString strKey = mapIterator.key();
        QVariant varValue = mapIterator.value();
        jsonObject[strKey] = varValue.toJsonValue();
    }

    return jsonObject;
}


QVariantMap VariantMapfromJsonObject(QJsonObject jsonObject)
{
    QVariantMap mapProperties;

    mapProperties = jsonObject.toVariantMap();

    return mapProperties;
}


QDataStream& operator<<(QDataStream& out, const CPropertiesInterface& propertiesInterface)
{
    out << propertiesInterface.getID()
        << propertiesInterface.getProperties();

    return out;
}

QDataStream& operator>>(QDataStream& in, CPropertiesInterface& propertiesInterface)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;

    in >> lID >> mapProperties;
    propertiesInterface.setID(lID);
    propertiesInterface.setProperties(mapProperties);

    return in;
}


//QDebug operator<<(QDebug dbg, const CPropertiesInterface& propertiesInterface)
//{
//    dbg.nospace()   << g_spacer
//                    << "CPropertiesInterface(" << propertiesInterface.getID()
//                    << ")" << "\n";

//    g_spacer.indent();
//    dbg.nospace() << g_spacer << "Properties:" << "\n";
//    QMapIterator<QString, QVariant> mapIterator(propertiesInterface.getProperties());
//    while (mapIterator.hasNext())
//    {
//          mapIterator.next();
//          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
//    }
//    g_spacer.unindent();

//    return dbg.maybeSpace();
//}
