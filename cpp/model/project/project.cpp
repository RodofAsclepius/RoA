#include "project.h"
#include "utils.h"
#include "databaseconnection.h"
#include "variableslist.h"
#include "groupslist.h"

#include <QMutexLocker>
#include <QFile>
#include <QMutexLocker>

using namespace utils;

CProject::CProject(QObject* parent)
    : CPropertiesInterface(parent)
{
//    DEBUG_OBJ_INFO
    qRegisterMetaType<CProject>("CProject");
//    qRegisterMetaTypeStreamOperators<CProject>("CProject");
}

CProject::CProject(const CProject& project)
    : CProject(project.parent())
{
    this->operator =(project);
}

CProject::~CProject()
{
//    DEBUG_OBJ_INFO
}

CProject& CProject::operator=(const CProject& project)
{
//    DEBUG_OBJ_INFO

    if (this != &project)
    {
        setParent(project.parent());
        setCurrentFileName(project.currentFileName());
        setDatabaseConnection(project.databaseConnection());
        setApplicationData(project.applicationData(), false);
        setProperties(project.getProperties());
        setIsDirty(project.isDirty());
    }
    return *this;
}

bool CProject::isDirty() const
{
    return bIsDirty_;
}

void CProject::setIsDirty(bool value)
{
    bIsDirty_ = value;
}

void CProject::setCurrentFileName(const QString &strFileName)
{
    QMutexLocker mutLocker(&_mutex);

    strCurrentFileName_ = strFileName;
    mutLocker.unlock();

    setIsDirty(true);
    emit currentFileNameUpdated(currentFileName());
}

QString CProject::currentFileName() const
{
    return strCurrentFileName_;
}

bool CProject::loadFromFile(const QString& strFilename)
{
//    DEBUG_OBJ_INFO

//    QMutexLocker mutLocker(&_mutex);

    emit startLoading();

    QFile file(strFilename);

    if (!file.open(QIODevice::ReadOnly)) {

        qDebug() << "Unable to open file:" << file.errorString();

        emit finishedLoading();
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    in >> *this;

    file.close();

    setCurrentFileName(strFilename);
    setIsDirty(false);

    applicationData_.registerUpdate(CApplicationData::Updates::Variables);

    if(!applicationData().groups().isEmpty())
    {
        applicationData_.registerUpdate(CApplicationData::Updates::Groups);
    }
    if(applicationData().treatment().isValid())
    {
        applicationData_.registerUpdate(CApplicationData::Updates::Treatment);
    }
    if(applicationData().effect().isValid())
    {
        applicationData_.registerUpdate(CApplicationData::Updates::Effect);
    }
    if(!applicationData().causalModels().isEmpty())
    {
        applicationData_.registerUpdate(CApplicationData::Updates::CausalModels);
    }
    if(!applicationData().balanceModels().isEmpty())
    {
        applicationData_.registerUpdate(CApplicationData::Updates::BalanceModels);
    }
    if(!applicationData().effectModels().isEmpty())
    {
        applicationData_.registerUpdate(CApplicationData::Updates::EffectsModels);
    }
    if(!applicationData().effectModels().isEmpty())
    {
        applicationData_.registerUpdate(CApplicationData::Updates::AdjustmentModels);
    }

    emit finishedLoading();
    emit applicationDataUpdated(applicationData());

    return true;
}

bool CProject::saveToFile(const QString& strFilename)
{
//    DEBUG_OBJ_INFO

//    QMutexLocker mutLocker(&_mutex);

    QString strFileNameFull = strFilename;
    if(strFilename.right(5) != QString(".cvis"))
    {
        strFileNameFull += QString(".cvis");
    }

    QFile file(strFileNameFull);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Unable to open file:" << file.errorString();
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    out << *this;
    file.close();

//    mutLocker.unlock();

    setCurrentFileName(strFileNameFull);
//    notifySaved();
    setIsDirty(false);


    return true;
}

void CProject::close()
{

}

CDatabaseConnection CProject::databaseConnection() const
{
    return databaseConnection_;
}

void CProject::setDatabaseConnection(const CDatabaseConnection &databaseConnection)
{
//    DEBUG_OBJ_INFO
    QMutexLocker mutLocker(&_mutex);

    databaseConnection_ = databaseConnection;
    mutLocker.unlock();

    setIsDirty(true);
    emit databaseConnectionUpdated(CProject::databaseConnection());
}

CApplicationData CProject::applicationData() const
{
    return applicationData_;
}

void CProject::setApplicationData(const CApplicationData &applicationData, bool bNotifiyUpdate)
{
    QMutexLocker mutLocker(&_mutex);

    applicationData_ = applicationData;
    mutLocker.unlock();

//    if(applicationData.latestUpdates().contains(CApplicationData::SELECTOR_TREE_UPDATE))
//    {
//        setIsDirty(true);
//    }

    if(bNotifiyUpdate)
    {
        emit applicationDataUpdated(applicationData_);
    }
}

QDataStream& operator<<(QDataStream& out, const CProject& project)
{
    out << project.isDirty()
        << project.currentFileName()
        << project.databaseConnection()
        << project.applicationData()
        << project.getProperties();

    return out;
}

QDataStream& operator>>(QDataStream& in, CProject& project)
{
    bool bIsDirty;
    QString strCurrentFileName;
    CDatabaseConnection databaseConnection;
    CApplicationData applicationData;
    QMap<QString, QVariant> mapProperties;

    in >> bIsDirty
        >> strCurrentFileName
        >> databaseConnection
        >> applicationData
        >> mapProperties;

    project.setCurrentFileName(strCurrentFileName);
    project.setDatabaseConnection(databaseConnection);
    project.setApplicationData(applicationData);
    project.setProperties(mapProperties);
    project.setIsDirty(bIsDirty);

    return in;
}

QDebug operator<<(QDebug dbg, const CProject& project)
{
    dbg.nospace() << g_spacer
                  << "CProject()" << project.currentFileName()
                  << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(project.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}
