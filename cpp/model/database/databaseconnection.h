#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include "propertiesinterface.h"

class CDatabaseConnection : public CPropertiesInterface
{
    Q_OBJECT
public:
    CDatabaseConnection();
    CDatabaseConnection(const CDatabaseConnection& databaseConnection);
    virtual ~CDatabaseConnection();

    CDatabaseConnection& operator=(const CDatabaseConnection& databaseConnection);
    virtual bool operator==(const CDatabaseConnection& databaseConnection) const;
    virtual bool operator<(const CDatabaseConnection& databaseConnection) const;
    friend QDataStream &operator>>(QDataStream& in, CDatabaseConnection& databaseConnection);

    QString host() const;
    void setHost(const QString& strHost);
    QString databaseName() const;
    void setDatabaseName(const QString& strDatabaseName);
    QString databaseUserName() const;
    void setDatabaseUserName(const QString& strUserName);
    QString databasePassword() const;
    void setDatabasePassword(const QString& strPassword);
    QString databaseMainTableName() const;
    void setDatabaseMainTableName(const QString& strMainTableName);
    QString databaseVariablesTypesTableName() const;
    void setDatabaseVariablesTypesTableName(const QString& strVariablesTypesTableName);

protected:
    QString _strHost;
    QString _strDatabaseName;
    QString _strUserName;
    QString _strUserPassword;
    QString _strMainTableName;
    QString _strVariablesTypesTableName;
signals:

public slots:
};

Q_DECLARE_METATYPE(CDatabaseConnection)

QDataStream& operator<<(QDataStream& out, const CDatabaseConnection& databaseConnection);
QDataStream& operator>>(QDataStream& in, CDatabaseConnection& databaseConnection);
QDebug operator<<(QDebug dbg, const CDatabaseConnection& databaseConnection);


#endif // DATABASECONNECTION_H
