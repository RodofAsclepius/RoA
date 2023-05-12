#ifndef RCONTROLLER_H
#define RCONTROLLER_H

#include "propertiesinterface.h"
#include "variableslist.h"
#include "groupslist.h"
#include "distributionlist.h"
#include "balancemodelslist.h"
#include "effectsmodelslist.h"
#include "causalmodellist.h"
#include "adjustmentmodellist.h"
#include "markovmodellist.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QMap>

class CRProxy;
class CRProxyWeb;

class CNetworkRequestType : public QObject
{
public:
    CNetworkRequestType(const QString& strType)
        : _strType(strType) {}
    ~CNetworkRequestType() {}
    QString getType() const { return _strType; }

private:
    QString _strType;
};

class CRController : public CPropertiesInterface
{
    Q_OBJECT
public:
    /* Singleton definition */
    static CRController& instance()
    {
        static CRController instance;
        return instance;
    }
    ~CRController();

    CRController(CRController const&) = delete;
    void operator=(CRController const&)  = delete;

    void reloadSettings();

    /* State */
    bool connectionIsAlive() const;
    bool databaseConnectionIsAlive() const;

    /* data */
    void resetWorkspace();
    void connectDatabase(const QString& strDatabaseName, const QString& strUserName,
                         const QString& strPassword, const QString& strMainTableName,
                         const QString& strVariablesTypesMainTableName);
    void getVariables();

    /* Processing */
    void computeTreatmentDistributions(const CDistributionsList &lstDistributions);
    void computeBalanceModels(const CBalanceModelsList& lstBalanceModel);
    void computeDistributions(const CDistributionsList &lstDistributions);
    void computeGroupSizes(const CGroupsList &lstGroups);
    void computeEffectsModels(const CEffectsModelsList& lstEffectsModels);
    void computeCausalModels(const CCausalModelList& lstCausalModels, qreal rAlpha);
    void computeAdjustMentModels(const CCausalModelList& lstCausalModels);
    void computeMarkovModels(const CCausalModelList& lstCausalModels, const CVariablesList &lstAllVariables);

protected:
    bool _bConnectionAlive = false;
    bool _bDatabaseConnectionAlive = false;

    virtual void timerEvent(QTimerEvent *event) override;

    QNetworkRequest createNetworkRequest(const QUrl &url, const QString &strPath) const;

    void ping();

    void processPingReply(const QJsonDocument& document);
    void processConnectDatabaseReply(const QJsonDocument& document);
    void processGetVariablesReply(const QJsonDocument& document);
    void processComputeBalanceModelsReply(const QJsonDocument& document);
    void processComputeDistributions(const QJsonDocument& document);
    void processComputeGroupSizes(const QJsonDocument& document);
    void processComputeEffectModelsReply(const QJsonDocument& document);
    void processComputeCausalModelsReply(const QJsonDocument& document);
    void processComputeAdjustmentModelsReply(const QJsonDocument& document);
    void processComputeMarkovModels(const QJsonDocument& document);

    bool checkBusy();
    void registerWorkItem(const QString& strWorkItem);
    void deregisterWorkItem(const QString& strWorkItem);

private:
    CRController();

    QNetworkAccessManager netWorkAccessManager_;
    QString _strHostname;
    QString _strPortNumber;
    QString _serviceBaseUrl;
    QStringList lstWorksItems_;

    QList<QNetworkRequest> lstNetworkRequests_;

    void setConnectionAlive(bool bAlive);
    void setDatabaseConnectionAlive(bool bAlive);
    void processNextNetworkRequest();

protected slots:
    void onNetworkReplyFinished(QNetworkReply* pReply);

signals:
    void connectionStatusUpdated(bool bAlive);
    void databaseConnectionStatusUpdated(bool bAlive);
    void variablesUpdated(const CVariablesList& lstVariables);
    void distributionsComputed(const CDistributionsList &distribution);
    void balanceModelsComputed(const CBalanceModelsList &balanceModel);
    void groupSizesComputed(const CGroupsList &lstGroups);
    void effectsModelsComputed(const CEffectsModelsList &lstEffectsModels);
    void causalModelsComputed(const CCausalModelList &lstCausalModels);
    void adjustmentModelsComputed(const CAdjustmentModelList &lstAdjustmentModels);
    void markovModelsComputed(const CMarkovModelList &lstMarkovModels);

    void processing();
    void doneProcessing();
};

#endif // RCONTROLLER_H
