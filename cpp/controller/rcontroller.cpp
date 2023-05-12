#include "rcontroller.h"
#include "utils.h"
#include "distribution.h"

#include <QUrlQuery>
#include <QSettings>

#include "variableslist.h"

CRController::CRController()
{
//    DEBUG_OBJ_INFO
    reloadSettings();

    connect(&netWorkAccessManager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNetworkReplyFinished(QNetworkReply*)));

    // Start connection timer
    startTimer(5000);

    ping();
}

void CRController::setConnectionAlive(bool bAlive)
{
//    DEBUG_OBJ_INFO

    QMutexLocker mutLocker(&_mutex);

    bool bOldStatus = _bConnectionAlive;
    _bConnectionAlive = bAlive;

    if(bOldStatus != bAlive)
    {
        emit connectionStatusUpdated(bAlive);
    }
}

void CRController::setDatabaseConnectionAlive(bool bAlive)
{
//    DEBUG_OBJ_INFO
    QMutexLocker mutLocker(&_mutex);

    bool bOldStatus = _bDatabaseConnectionAlive;
    _bDatabaseConnectionAlive = bAlive;

    if(bOldStatus != bAlive)
    {
        emit databaseConnectionStatusUpdated(bAlive);
    }
}

void CRController::processNextNetworkRequest()
{
//    DEBUG_OBJ_INFO

}

CRController::~CRController()
{
    //    DEBUG_OBJ_INFO
}

void CRController::reloadSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TU/e", "Asclepius");

    settings.beginGroup("Environment");
        settings.beginGroup("RServer");
        _serviceBaseUrl = settings.value("ServiceBaseUrl", "http://localhost:8001/").toString();
        settings.endGroup();
    settings.endGroup();
}

bool CRController::connectionIsAlive() const
{
    return _bConnectionAlive;
}

bool CRController::databaseConnectionIsAlive() const
{
    return _bDatabaseConnectionAlive;
}

void CRController::resetWorkspace()
{
//    DEBUG_OBJ_INFO

    QString strPath = "resetworkspace";

    QUrl serviceUrl = QUrl(_serviceBaseUrl + strPath);
    QByteArray postData;

    QUrl params;
    QUrlQuery query;
    params.setQuery(query);
    postData = params.toEncoded(QUrl::RemoveFragment);

    QNetworkRequest request = createNetworkRequest(serviceUrl, strPath);
    netWorkAccessManager_.post(request, postData);
}

void CRController::connectDatabase(const QString &strDatabaseName, const QString &strUserName,
                                   const QString &strPassword, const QString &strMainTableName,
                                   const QString &strVariablesTypesMainTableName)
{
    //    DEBUG_OBJ_INFO

    if(strDatabaseName.isEmpty() || strUserName.isEmpty() || strMainTableName.isEmpty() || strVariablesTypesMainTableName.isEmpty())
        return;

    if(checkBusy())
        return;

    registerWorkItem("dbcanconnect");

    qDebug() << "Connecting to " << strDatabaseName << "@"<< _serviceBaseUrl << "...";
    QString strPath = "/connectToDatabase";

    QUrl serviceUrl(_serviceBaseUrl);
    serviceUrl.setPath(strPath, QUrl::TolerantMode);
    QByteArray postData;

    QUrl params;
    QUrlQuery query;

    /* Add populationTreeNode */
    query.addQueryItem("thedatabasename", strDatabaseName);
    query.addQueryItem("username", strUserName);
    query.addQueryItem("password", strPassword);
    query.addQueryItem("maintablename", strMainTableName);
    query.addQueryItem("variablestypestablename", strVariablesTypesMainTableName);

    params.setQuery(query);
    postData = params.toEncoded(QUrl::RemoveFragment);

    QNetworkRequest request = createNetworkRequest(serviceUrl, strPath);
    netWorkAccessManager_.post(request, postData);
}

void CRController::getVariables()
{
//    DEBUG_OBJ_INFO

    if(checkBusy())
        return;

    registerWorkItem("variables");

    QString strPath = "getVariables";

    QUrl serviceUrl = QUrl(_serviceBaseUrl + strPath);
    QByteArray postData;

    QUrl params;
    QUrlQuery query;
    params.setQuery(query);
    postData = params.toEncoded(QUrl::RemoveFragment);

    QNetworkRequest request = createNetworkRequest(serviceUrl, strPath);
    netWorkAccessManager_.post(request, postData);
}

void CRController::computeTreatmentDistributions(const CDistributionsList &lstDistributions)
{
    if(checkBusy())
        return;

    registerWorkItem("treatmentdistributions");

    QString strPath = "computeTreatmentDistributions";

    QUrl serviceUrl = QUrl(_serviceBaseUrl + strPath);
    QByteArray postData;

    QUrl params;
    QUrlQuery query;

    /* Add parameters */
    QJsonDocument docDistributions(lstDistributions.toJsonObject());
    query.addQueryItem("distributions", docDistributions.toJson());

    params.setQuery(query);
    postData = params.toEncoded(QUrl::RemoveFragment);

    QNetworkRequest request = createNetworkRequest(serviceUrl, strPath);
    netWorkAccessManager_.post(request, postData);
}

void CRController::computeBalanceModels(const CBalanceModelsList &lstBalanceModel)
{
//    DEBUG_OBJ_INFO

    if(checkBusy())
        return;

    registerWorkItem("balancemodels");

    QString strPath = "computeBalanceModels";

    QUrl serviceUrl = QUrl(_serviceBaseUrl + strPath);
    QByteArray postData;

    QUrl params;
    QUrlQuery query;

    /* Add parameters */
    QJsonDocument docBalanceModels(lstBalanceModel.toJsonObject());
    query.addQueryItem("balanceModels", docBalanceModels.toJson());

    params.setQuery(query);
    postData = params.toEncoded(QUrl::RemoveFragment);

    QNetworkRequest request = createNetworkRequest(serviceUrl, strPath);
    netWorkAccessManager_.post(request, postData);
}

void CRController::computeDistributions(const CDistributionsList &lstDistributions)
{
//    DEBUG_OBJ_INFO

    if(checkBusy())
        return;

    registerWorkItem("distributions");

    QString strPath = "computeDistributions";

    QUrl serviceUrl = QUrl(_serviceBaseUrl + strPath);
    QByteArray postData;

    QUrl params;
    QUrlQuery query;

    /* Add parameters */
    QJsonDocument docDistributions(lstDistributions.toJsonObject());
    query.addQueryItem("distributions", docDistributions.toJson());

    params.setQuery(query);
    postData = params.toEncoded(QUrl::RemoveFragment);

    QNetworkRequest request = createNetworkRequest(serviceUrl, strPath);
    netWorkAccessManager_.post(request, postData);
}

void CRController::computeGroupSizes(const CGroupsList &lstGroups)
{
//    DEBUG_OBJ_INFO

    if(checkBusy())
        return;

    registerWorkItem("groupsizes");

    QString strPath = "computeGroupSizes";

    QUrl serviceUrl = QUrl(_serviceBaseUrl + strPath);
    QByteArray postData;

    QUrl params;
    QUrlQuery query;

    /* Add parameters */
    QJsonDocument docGroups(lstGroups.toJsonObject());
    query.addQueryItem("groups", docGroups.toJson());

    params.setQuery(query);
    postData = params.toEncoded(QUrl::RemoveFragment);

    QNetworkRequest request = createNetworkRequest(serviceUrl, strPath);
    netWorkAccessManager_.post(request, postData);
}

void CRController::computeEffectsModels(const CEffectsModelsList &lstEffectsModels)
{
//    DEBUG_OBJ_INFO

    if(checkBusy())
        return;

    registerWorkItem("effectsmodels");

    QString strPath = "computeEffectsModels";

    QUrl serviceUrl = QUrl(_serviceBaseUrl + strPath);
    QByteArray postData;

    QUrl params;
    QUrlQuery query;

    /* Add parameters */
    QJsonDocument docEffectsModels(lstEffectsModels.toJsonObject());
    query.addQueryItem("effectsModels", docEffectsModels.toJson());

    params.setQuery(query);
    postData = params.toEncoded(QUrl::RemoveFragment);

    QNetworkRequest request = createNetworkRequest(serviceUrl, strPath);
    netWorkAccessManager_.post(request, postData);
}

void CRController::computeCausalModels(const CCausalModelList &lstCausalModels, qreal rAlpha)
{
//    DEBUG_OBJ_INFO

    if(checkBusy())
        return;

    registerWorkItem("causalmodels");

    QString strPath = "computeCausalModels";

    QUrl serviceUrl = QUrl(_serviceBaseUrl + strPath);
    QByteArray postData;

    QUrl params;
    QUrlQuery query;

    /* Add parameters */
    QJsonDocument docCausalModels(lstCausalModels.toJsonObject());
    query.addQueryItem("causalModels", docCausalModels.toJson());
    query.addQueryItem("alphaValue", QString::number(rAlpha));

    params.setQuery(query);
    postData = params.toEncoded(QUrl::RemoveFragment);

    QNetworkRequest request = createNetworkRequest(serviceUrl, strPath);
    netWorkAccessManager_.post(request, postData);
}

void CRController::computeAdjustMentModels(const CCausalModelList& lstCausalModels)
{
//    DEBUG_OBJ_INFO
    if(checkBusy())
        return;

    registerWorkItem("adjustmentmodels");

    QString strPath = "computeAdjustmentModels";

    QUrl serviceUrl = QUrl(_serviceBaseUrl + strPath);
    QByteArray postData;

    QUrl params;
    QUrlQuery query;

    /* Add parameters */
    QJsonDocument docCausalModel(lstCausalModels.toJsonObject());
    query.addQueryItem("causalModels", docCausalModel.toJson());

    params.setQuery(query);
    postData = params.toEncoded(QUrl::RemoveFragment);

    QNetworkRequest request = createNetworkRequest(serviceUrl, strPath);
    netWorkAccessManager_.post(request, postData);
}

void CRController::computeMarkovModels(const CCausalModelList &lstCausalModels, const CVariablesList &lstAllVariables)
{
    //    DEBUG_OBJ_INFO
    if(checkBusy())
        return;

    registerWorkItem("markovModels");

    QString strPath = "computeMarkovModels";

    QUrl serviceUrl = QUrl(_serviceBaseUrl + strPath);
    QByteArray postData;

    QUrl params;
    QUrlQuery query;

    /* Add parameters */
    QJsonDocument docCausalModel(lstCausalModels.toJsonObject());
    query.addQueryItem("causalModels", docCausalModel.toJson());

    QJsonDocument docAllVariables(lstAllVariables.toJsonObject());
    query.addQueryItem("allVariables", docAllVariables.toJson());

    params.setQuery(query);
    postData = params.toEncoded(QUrl::RemoveFragment);

    QNetworkRequest request = createNetworkRequest(serviceUrl, strPath);
    netWorkAccessManager_.post(request, postData);;
}

void CRController::timerEvent(QTimerEvent *event)
{
//    DEBUG_OBJ_INFO

    Q_UNUSED(event)

    if(!_bConnectionAlive)
    {
        ping();
    }
    else
    {
        processNextNetworkRequest();
    }
}

QNetworkRequest CRController::createNetworkRequest(const QUrl &url, const QString &strPath) const
{
    QNetworkRequest request(url);

    request.setAttribute(QNetworkRequest::Http2AllowedAttribute, false);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::SameOriginRedirectPolicy);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setOriginatingObject(new CNetworkRequestType(strPath));

    return request;
}

void CRController::ping()
{
//    DEBUG_OBJ_INFO

    QString strPath = "ping";

    QUrl serviceUrl = QUrl(_serviceBaseUrl + strPath);
    QByteArray postData;

    QUrl params;
    QUrlQuery query;
    params.setQuery(query);
    postData = params.toEncoded(QUrl::RemoveFragment);

    QNetworkRequest request = createNetworkRequest(serviceUrl, strPath);
    netWorkAccessManager_.post(request, postData);
}

void CRController::processPingReply(const QJsonDocument &document)
{
//    DEBUG_OBJ_INFO
    Q_UNUSED(document)
//    deregisterWorkItem("ping");

    setConnectionAlive(true);
}

void CRController::processConnectDatabaseReply(const QJsonDocument &document)
{
//    DEBUG_OBJ_INFO

    deregisterWorkItem("dbcanconnect");

    QJsonObject jsonResult = document.object();
    QJsonValue jsonCanConnect = jsonResult["dbcanconnect"];

    bool bAlive = jsonCanConnect.toBool();

    setDatabaseConnectionAlive(bAlive);

    qDebug() << "Database connection alive";
}

void CRController::processGetVariablesReply(const QJsonDocument &document)
{
//    DEBUG_OBJ_INFO

    deregisterWorkItem("variables");

    CVariablesList lstVariables;

    if(document.isObject())
    {
        QJsonObject objDoc = document.object();
        QJsonValue valVariables = objDoc["variablestypes"];
        if(valVariables.isArray())
        {
            lstVariables.fromJsonArray(valVariables.toArray());
        }
    }

    emit variablesUpdated(lstVariables);
}

void CRController::processComputeBalanceModelsReply(const QJsonDocument &document)
{
//    DEBUG_OBJ_INFO
    deregisterWorkItem("balancemodels");

    CBalanceModelsList lstBalanceModels;

    if(document.isObject())
    {
        QJsonObject objDoc = document.object();

        QJsonValue valBalanceModels = objDoc["balanceModels"];
        if(valBalanceModels.isArray())
        {
            lstBalanceModels.fromJsonArray(valBalanceModels.toArray());
        }
    }

    emit balanceModelsComputed(lstBalanceModels);
}

void CRController::processComputeDistributions(const QJsonDocument &document)
{
//    DEBUG_OBJ_INFO
    deregisterWorkItem("distributions");

    CDistributionsList lstDistributions;

    if(document.isObject())
    {
        QJsonObject objDoc = document.object();

        QJsonValue valDistributions = objDoc["distributions"];
        if(valDistributions.isArray())
        {
            lstDistributions.fromJsonArray(valDistributions.toArray());
        }
    }

    emit distributionsComputed(lstDistributions);
}

void CRController::processComputeGroupSizes(const QJsonDocument &document)
{
//    DEBUG_OBJ_INFO
    deregisterWorkItem("groupsizes");

    CGroupsList lstGroups;

    if(document.isObject())
    {
        QJsonObject objDoc = document.object();

        QJsonValue valGroups = objDoc["groups"];
        if(valGroups.isArray())
        {
            lstGroups.fromJsonArray(valGroups.toArray());
        }
    }

    emit groupSizesComputed(lstGroups);
}

void CRController::processComputeEffectModelsReply(const QJsonDocument &document)
{
//    DEBUG_OBJ_INFO
    deregisterWorkItem("effectsmodels");

    CEffectsModelsList lstEffectsModels;

    if(document.isObject())
    {
        QJsonObject objDoc = document.object();

        QJsonValue valEffectsModels = objDoc["effectsModels"];
        if(valEffectsModels.isArray())
        {
            lstEffectsModels.fromJsonArray(valEffectsModels.toArray());
        }
    }

    emit effectsModelsComputed(lstEffectsModels);
}

void CRController::processComputeCausalModelsReply(const QJsonDocument &document)
{
//    DEBUG_OBJ_INFO
    deregisterWorkItem("causalmodels");

    CCausalModelList lstCausalModels;

    if(document.isObject())
    {
        QJsonObject objDoc = document.object();

        QJsonValue valCausalModels = objDoc["causalModels"];
        if(valCausalModels.isArray())
        {
            lstCausalModels.fromJsonArray(valCausalModels.toArray());
        }
    }

    emit causalModelsComputed(lstCausalModels);
}

void CRController::processComputeAdjustmentModelsReply(const QJsonDocument& document)
{
//    DEBUG_OBJ_INFO
//    qDebug() << document;
    deregisterWorkItem("adjustmentmodels");

    CAdjustmentModelList lstAdjustmentModels;

    if(document.isObject())
    {
        QJsonObject objDoc = document.object();

        QJsonValue valAdjustmentModels = objDoc["adjustmentModels"];
        if(valAdjustmentModels.isArray())
        {
            lstAdjustmentModels.fromJsonArray(valAdjustmentModels.toArray());
        }
    }

    emit adjustmentModelsComputed(lstAdjustmentModels);
}

void CRController::processComputeMarkovModels(const QJsonDocument &document)
{
//    DEBUG_OBJ_INFO

    deregisterWorkItem("markovModels");

    CMarkovModelList lstMarkovModels;

    if(document.isObject())
    {
        QJsonObject objDoc = document.object();

        QJsonValue valMarkovModels = objDoc["markovModels"];
        if(valMarkovModels.isArray())
        {
            lstMarkovModels.fromJsonArray(valMarkovModels.toArray());
        }
    }

    emit markovModelsComputed(lstMarkovModels);
}

bool CRController::checkBusy()
{
//    DEBUG_OBJ_INFO
//    qDebug() << !lstWorksItems_.isEmpty();

    return !lstWorksItems_.isEmpty();
}

void CRController::registerWorkItem(const QString &strWorkItem)
{
    QMutexLocker mutLocker(&_mutex);

    lstWorksItems_.append(strWorkItem);

//    DEBUG_OBJ_INFO

    qDebug() << "register work item:" << strWorkItem;

    emit processing();
}

void CRController::deregisterWorkItem(const QString &strWorkItem)
{
    QMutexLocker mutLocker(&_mutex);

    lstWorksItems_.removeAll(strWorkItem);

//    DEBUG_OBJ_INFO

    qDebug() << "DEregister work item:" << strWorkItem;

    if(lstWorksItems_.isEmpty())
    {
        emit doneProcessing();
    }
}

void CRController::onNetworkReplyFinished(QNetworkReply *pReply)
{
//    DEBUG_OBJ_INFO

    if (pReply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Network error: " << pReply->errorString() << pReply->error();

        setConnectionAlive(false);

        return;
    }
    else
    {
        QString strType = static_cast<CNetworkRequestType*>(pReply->request().originatingObject())->getType();

        /* Process Json */
        QJsonDocument document = QJsonDocument::fromJson(pReply->readAll());

        if(strType == "ping")
        {
            processPingReply(document);
        }

        if(strType == "/connectToDatabase")
        {
            processConnectDatabaseReply(document);
        }

        if(strType == "getVariables")
        {
            processGetVariablesReply(document);
        }

        if(strType == "computeBalanceModels")
        {
            processComputeBalanceModelsReply(document);
        }

        if(strType == "computeDistributions")
        {
            processComputeDistributions(document);
        }

        if(strType == "computeGroupSizes")
        {
            processComputeGroupSizes(document);
        }

        if(strType == "computeEffectsModels")
        {
            processComputeEffectModelsReply(document);
        }

        if(strType == "computeCausalModels")
        {
            processComputeCausalModelsReply(document);
        }

        if(strType == "computeAdjustmentModels")
        {
            processComputeAdjustmentModelsReply(document);
        }

        if(strType == "computeMarkovModels")
        {
            processComputeMarkovModels(document);
        }
    }

    pReply->deleteLater();
}
