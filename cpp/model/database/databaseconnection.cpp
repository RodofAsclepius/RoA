#include "databaseconnection.h"
#include "utils.h"

#include <QMutexLocker>

using namespace utils;

CDatabaseConnection::CDatabaseConnection()
    : CPropertiesInterface()
{

}

CDatabaseConnection::CDatabaseConnection(const CDatabaseConnection &databaseConnection)
    : CDatabaseConnection()
{
    this->operator =(databaseConnection);
}

CDatabaseConnection::~CDatabaseConnection()
{

}

CDatabaseConnection &CDatabaseConnection::operator=(const CDatabaseConnection &databaseConnection)
{
    if (this != &databaseConnection)
    {
        setHost(databaseConnection.host());
        setDatabaseName(databaseConnection.databaseName());
        setDatabaseUserName(databaseConnection.databaseUserName());
        setDatabasePassword(databaseConnection.databasePassword());
        setDatabaseMainTableName(databaseConnection.databaseMainTableName());
        setDatabaseVariablesTypesTableName(databaseConnection.databaseVariablesTypesTableName());

        setID(databaseConnection.getID());
        setProperties(databaseConnection.getProperties());
    }
    return *this;
}

bool CDatabaseConnection::operator==(const CDatabaseConnection &databaseConnection) const
{
    return (getID() == databaseConnection.getID());
}

bool CDatabaseConnection::operator<(const CDatabaseConnection &databaseConnection) const
{
    return (getID() < databaseConnection.getID());
}

QString CDatabaseConnection::host() const
{
    return _strHost;
}

void CDatabaseConnection::setHost(const QString &strHost)
{
    QMutexLocker mutLocker(&_mutex);

    _strHost = strHost;
}

QString CDatabaseConnection::databaseName() const
{
    return _strDatabaseName;
}

void CDatabaseConnection::setDatabaseName(const QString &strDatabaseName)
{
    QMutexLocker mutLocker(&_mutex);

    _strDatabaseName = strDatabaseName;
}

QString CDatabaseConnection::databaseUserName() const
{
    return _strUserName;
}

void CDatabaseConnection::setDatabaseUserName(const QString &strUserName)
{
    QMutexLocker mutLocker(&_mutex);

    _strUserName = strUserName;
}

QString CDatabaseConnection::databasePassword() const
{
    return _strUserPassword;
}

void CDatabaseConnection::setDatabasePassword(const QString &strPassword)
{
    QMutexLocker mutLocker(&_mutex);

    _strUserPassword = strPassword;
}

QString CDatabaseConnection::databaseMainTableName() const
{
    return _strMainTableName;
}

void CDatabaseConnection::setDatabaseMainTableName(const QString &strMainTableName)
{
    QMutexLocker mutLocker(&_mutex);

    _strMainTableName = strMainTableName;
}

QString CDatabaseConnection::databaseVariablesTypesTableName() const
{
    return _strVariablesTypesTableName;
}

void CDatabaseConnection::setDatabaseVariablesTypesTableName(const QString &strVariablesTypesTableName)
{
    QMutexLocker mutLocker(&_mutex);

    _strVariablesTypesTableName = strVariablesTypesTableName;
}


QDataStream& operator<<(QDataStream& out, const CDatabaseConnection& databaseConnection)
{
    out << databaseConnection.getID()
        << databaseConnection.getProperties()
        << databaseConnection.host()
        << databaseConnection.databaseName()
        << databaseConnection.databaseUserName()
        << databaseConnection.databasePassword()
        << databaseConnection.databaseMainTableName()
        << databaseConnection.databaseVariablesTypesTableName();

    return out;
}


QDataStream& operator>>(QDataStream& in, CDatabaseConnection& databaseConnection)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;
    QString strHost;
    QString strDatabaseName;
    QString strUserName;
    QString strUserPassword;
    QString strMainTableName;
    QString strVariablesTypesTableName;

    in >> lID >> mapProperties
       >> strHost >> strDatabaseName
       >> strUserName >> strUserPassword
       >> strMainTableName >> strVariablesTypesTableName;

    databaseConnection.setID(lID);
    databaseConnection.setProperties(mapProperties);
    databaseConnection.setHost(strHost);
    databaseConnection.setDatabaseName(strDatabaseName);
    databaseConnection.setDatabaseUserName(strUserName);
    databaseConnection.setDatabasePassword(strUserPassword);
    databaseConnection.setDatabaseMainTableName(strMainTableName);
    databaseConnection.setDatabaseVariablesTypesTableName(strVariablesTypesTableName);

    return in;
}


QDebug operator<<(QDebug dbg, const CDatabaseConnection& databaseConnection)
{
    dbg.nospace()   << g_spacer
                    << "CDatabaseConnection(" << databaseConnection.getID()
                    << ", " << databaseConnection.host()
                    << ", " << databaseConnection.databaseName()
                    << ", " << databaseConnection.databaseUserName()
//                    << ", " << databaseConnection.databasePassword()
                    << ", " << databaseConnection.databaseMainTableName()
                    << ", " << databaseConnection.databaseVariablesTypesTableName()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(databaseConnection.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}

