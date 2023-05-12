#ifndef PROPERTIESINTERFACE_H
#define PROPERTIESINTERFACE_H




#include <QObject>
#include <QMutex>
#include <QMap>
#include <QVariant>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDataStream>
#include <QMutexLocker>



class CPropertiesInterface : public QObject
{
    Q_OBJECT
public:
    explicit CPropertiesInterface(QObject *parent = nullptr);
    CPropertiesInterface(const CPropertiesInterface& propertiesInterface);
    virtual ~CPropertiesInterface();

    quint32 getID() const;

    CPropertiesInterface& operator=(const CPropertiesInterface& propertiesInterface);
    virtual bool operator==(const CPropertiesInterface& propertiesInterface) const;
    virtual bool operator<(const CPropertiesInterface& propertiesInterface) const;

    void setProperties(const QVariantMap& mapProperties);
    template<typename T=QVariant>
    T getProperty(const QString& strName) const;
    QJsonObject getJsonObjectProperty(const QString& strName) const;
    QJsonArray getJsonArrayProperty(const QString& strName) const;
    QVariantMap getProperties() const;
    void addProperty(const QString& strName, const QVariant& property);
    void addProperties(const QStringList& lstStrings, const QList<QVariant>& lstProperties);
    void removeProperty(const QString& strName);
    void removeAllProperties();

    void propertiesFromJsonObject(QJsonObject objProperties);
    void propertiesToJsonObject() const;

protected:
    static quint32 _counter;
    QMutex _mutex;

    quint32 _lID;

    QVariantMap _mapProperties;

    virtual void setID(quint32 lID);

    friend QDataStream& operator>>(QDataStream& in, CPropertiesInterface& propertiesInterface);
signals:

public slots:
};


template<typename T>
T CPropertiesInterface::getProperty(const QString& strName) const
{
    T property = _mapProperties[strName].value<T>();
    return property;
}


QJsonObject VariantMaptoJsonObject(const QVariantMap& mapProperties);
QVariantMap VariantMapfromJsonObject(QJsonObject jsonObject);


QDataStream& operator<<(QDataStream& out, const CPropertiesInterface& propertiesInterface);
QDataStream& operator>>(QDataStream& in, CPropertiesInterface& propertiesInterface);
//QDebug operator<<(QDebug dbg, const CPropertiesInterface& propertiesInterface);


#endif // PROPERTIESINTERFACE_H
