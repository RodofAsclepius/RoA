#ifndef PROJECT_H
#define PROJECT_H

#include "propertiesinterface.h"
#include "databaseconnection.h"
#include "variableslist.h"
#include "groupslist.h"
#include "applicationdata.h"

#include <QObject>
#include <memory>

class CProject : public CPropertiesInterface
{
    Q_OBJECT
public:
    explicit CProject(QObject* parent = nullptr);
    CProject(const CProject& project);
    ~CProject();

    CProject& operator=(const CProject& project);

    /* state */
    bool isDirty() const;
    void setIsDirty(bool value);
    void setCurrentFileName(const QString& strFileName);
    QString currentFileName() const;
    bool loadFromFile(const QString& strFilename);
    bool saveToFile(const QString& strFilename);
    void close();

    /* Data */
    CDatabaseConnection databaseConnection() const;
    void setDatabaseConnection(const CDatabaseConnection& databaseConnection);
    CApplicationData applicationData() const;
    void setApplicationData(const CApplicationData &applicationData, bool bNotifiyUpdate = false);

protected:
    bool bIsDirty_ = false;
    QString strCurrentFileName_;

    CDatabaseConnection databaseConnection_;
    CApplicationData applicationData_;

signals:
    void startLoading();
    void finishedLoading();
    void closed();

    void currentFileNameUpdated(QString strCurrentFileName);
    void databaseConnectionUpdated(CDatabaseConnection databaseConnection);
    void applicationDataUpdated(CApplicationData applicationData);

    friend QDataStream& operator<<(QDataStream& out, const CProject& project);
    friend QDataStream& operator>>(QDataStream& in, CProject& project);
};

Q_DECLARE_METATYPE(CProject)

QDataStream& operator<<(QDataStream& out, const CProject& project);
QDataStream& operator>>(QDataStream& in, CProject& project);
QDebug operator<<(QDebug dbg, const CProject& project);

#endif // PROJECT_H
