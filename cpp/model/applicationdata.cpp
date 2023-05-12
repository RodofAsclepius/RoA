#include "applicationdata.h"
#include "utils.h"

#include <QMutexLocker>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

using namespace utils;

const QString CApplicationData::Updates::All = QString("UpdateAll");
const QString CApplicationData::Updates::Variables = QString("UpdateVariables");
const QString CApplicationData::Updates::Groups = QString("UpdateGroups");
const QString CApplicationData::Updates::GroupsSizes = QString("UpdateGroupsSizes");
const QString CApplicationData::Updates::Treatment = QString("UpdateTreatment");
const QString CApplicationData::Updates::Effect = QString("UpdateEffect");
const QString CApplicationData::Updates::Distributions = QString("UpdateDistributions");
const QString CApplicationData::Updates::CausalModels = QString("CausalModelsUpdate");
const QString CApplicationData::Updates::CausalModelCaches = QString("CausalModelCachesUpdate");
const QString CApplicationData::Updates::BalanceModels = QString("UpdateBalanceModels");
const QString CApplicationData::Updates::EffectsModels = QString("UpdateEffectsModels");
const QString CApplicationData::Updates::AdjustmentModels = QString("UpdateAdjustmentSets");

CApplicationData::CApplicationData()
    : CPropertiesInterface()
{
    qRegisterMetaType<CApplicationData>("CApplicationData");
//    qRegisterMetaTypeStreamOperators<CApplicationData>("CApplicationData");
}

CApplicationData::CApplicationData(const CApplicationData& applicationData)
    : CApplicationData()
{
    this->operator =(applicationData);
}

CApplicationData::~CApplicationData()
{
}

CApplicationData& CApplicationData::operator=(const CApplicationData& applicationData)
{
    if (this != &applicationData)
    {
        setID(applicationData.getID());
        setProperties(applicationData.getProperties());

        setVariables(applicationData.variables());
        setGroups(applicationData.groups());
        setTreatment(applicationData.treatment());
        setEffect(applicationData.effect());
        setDistributions(applicationData.distributions());

        setCausalModels(applicationData.causalModels());
        setCausalModelCaches(applicationData.causalModelCaches());
        setBalanceModels(applicationData.balanceModels());
        setEffectModels(applicationData.effectModels());

        setAdjustmentModels(applicationData.adjustmentModels());

        setLatestUpdates(applicationData.latestUpdates());
    }
    return *this;
}

bool CApplicationData::operator<(const CApplicationData& applicationData) const
{
    return (getID() < applicationData.getID());
}

bool CApplicationData::operator==(const CApplicationData& applicationData) const
{
    return (getID() == applicationData.getID());
}

CVariablesList CApplicationData::variables() const
{
    return lstVariables_;
}

void CApplicationData::setVariables(const CVariablesList &lstVariables)
{
    QMutexLocker mutLocker(&_mutex);
    lstVariables_ = lstVariables;
}

CGroupsList CApplicationData::groups() const
{
    return lstGroups_;
}

void CApplicationData::setGroups(const CGroupsList &lstGroups)
{
    lstGroups_ = lstGroups;
}

CBalanceModelsList CApplicationData::balanceModels() const
{
    return lstBalanceModels_;
}

void CApplicationData::setBalanceModels(const CBalanceModelsList &lstBalanceModels)
{
    lstBalanceModels_ = lstBalanceModels;
}

void CApplicationData::clearBalanceModels()
{
    lstBalanceModels_.clear();
}

void CApplicationData::addBalanceModel(const CBalanceModel &balanceModel)
{
    int iIndex = lstBalanceModels_.indexOf(balanceModel);
    if(iIndex == -1)
    {
        lstBalanceModels_.append(balanceModel);
    }
    else
    {
        lstBalanceModels_.replace(balanceModel);
    }
}

CEffectsModelsList CApplicationData::effectModels() const
{
    return lstEffectModels_;
}

void CApplicationData::setEffectModels(const CEffectsModelsList &lstEffectModels)
{
    lstEffectModels_ = lstEffectModels;
}

void CApplicationData::clearEffectModels()
{
    lstEffectModels_.clear();
}

void CApplicationData::addEffectModel(const CEffectsModel &model)
{
    int iIndex = lstEffectModels_.indexOf(model);
    if(iIndex == -1)
    {
        lstEffectModels_.append(model);
    }
    else
    {
        lstEffectModels_.replace(model);
    }
}

QStringList CApplicationData::latestUpdates() const
{
    return lstLatestUpdates_;
}

void CApplicationData::setLatestUpdates(const QStringList &lstLatestUpdates)
{
    lstLatestUpdates_ = lstLatestUpdates;
}

void CApplicationData::clearLatestUpdates()
{
    lstLatestUpdates_.clear();
}

void CApplicationData::registerUpdate(QString strUpdate, bool bClearPrevious)
{
    if(bClearPrevious)
    {
        clearLatestUpdates();
    }

    lstLatestUpdates_.append(strUpdate);
}

void CApplicationData::removeUpdate(QString strUpdate)
{
    lstLatestUpdates_.removeAll(strUpdate);
}

bool CApplicationData::checkUpdate(QString strUpdate)
{
    return lstLatestUpdates_.indexOf(strUpdate) != -1;
}

CCausalModelCacheList CApplicationData::causalModelCaches() const
{
    return lstCausalModelCaches_;
}

void CApplicationData::setCausalModelCaches(const CCausalModelCacheList &lstCausalModelCaches)
{
    lstCausalModelCaches_ = lstCausalModelCaches;
}

void CApplicationData::addCausalModelCache(const CCausalModelCache &causalModelCache)
{
    int iIndex = lstCausalModelCaches_.indexOf(causalModelCache);
    if(iIndex == -1)
    {
        lstCausalModelCaches_.append(causalModelCache);
    }
    else
    {
        lstCausalModelCaches_.replace(causalModelCache);
    }
}

void CApplicationData::clearCausalModelCaches()
{
    lstCausalModelCaches_.clear();
}

CAdjustmentModelList CApplicationData::adjustmentModels() const
{
    return lstAdjustmentModels_;
}

void CApplicationData::setAdjustmentModels(const CAdjustmentModelList &lstAdjustmentModels)
{
    lstAdjustmentModels_ = lstAdjustmentModels;
}

CCausalModelList CApplicationData::causalModels() const
{
    return lstCausalModels_;
}

void CApplicationData::setCausalModels(const CCausalModelList &lstCausalModels)
{
    lstCausalModels_ = lstCausalModels;
}

void CApplicationData::addBalanceModel(const CCausalModel &causalModel)
{
    lstCausalModels_.append(causalModel);
}

void CApplicationData::clearCausalModels()
{
    lstCausalModels_.clear();
}

CTreatment CApplicationData::treatment() const
{
    return treatment_;
}

void CApplicationData::setTreatment(const CTreatment &treatment)
{
    treatment_ = treatment;
}

CEffect CApplicationData::effect() const
{
    return effect_;
}

void CApplicationData::setEffect(const CEffect &effect)
{
    effect_ = effect;
}

CDistributionsList CApplicationData::distributions() const
{
    return lstDistributions_;
}

void CApplicationData::setDistributions(const CDistributionsList &lstDistributions)
{
    lstDistributions_ = lstDistributions;
}

QDataStream& operator<<(QDataStream& out, const CApplicationData& applicationData)
{
    out << applicationData.getID()
        << applicationData.getProperties()
        << applicationData.variables()
        << applicationData.groups()
        << applicationData.treatment()
        << applicationData.effect()
        << applicationData.distributions()
        << applicationData.causalModels()
        << applicationData.causalModelCaches()
        << applicationData.balanceModels()
        << applicationData.effectModels()
        << applicationData.adjustmentModels()
        << applicationData.latestUpdates();

    return out;
}

QDataStream& operator>>(QDataStream& in, CApplicationData& applicationData)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;

    CVariablesList lstVariables;
    CGroupsList lstGroups;
    CTreatment treatment;
    CEffect effect;
    CDistributionsList lstDistributions;
    CCausalModelList lstCausalModels;
    CCausalModelCacheList lstCausalModelCaches;
    CBalanceModelsList lstBalanceModels;
    CEffectsModelsList lstEffectModels;
    CAdjustmentModelList lstAdjustmentModels;
    QStringList lstLatestUpdates;

    in >> lID
        >> mapProperties
        >> lstVariables
        >> lstGroups
        >> treatment
        >> effect
        >> lstDistributions
        >> lstCausalModels
        >> lstCausalModelCaches
        >> lstBalanceModels
        >> lstEffectModels
        >> lstAdjustmentModels
        >> lstLatestUpdates;

    CApplicationData applicationDataNew;

    applicationDataNew.setID(lID);
    applicationDataNew.setProperties(mapProperties);

    applicationDataNew.setVariables(lstVariables);
    applicationDataNew.setGroups(lstGroups);
    applicationDataNew.setTreatment(treatment);
    applicationDataNew.setEffect(effect);
    applicationDataNew.setDistributions(lstDistributions);

    applicationDataNew.setCausalModels(lstCausalModels);
    applicationDataNew.setCausalModelCaches(lstCausalModelCaches);

    applicationDataNew.setBalanceModels(lstBalanceModels);
    applicationDataNew.setEffectModels(lstEffectModels);
    applicationDataNew.setAdjustmentModels(lstAdjustmentModels);

    applicationDataNew.setLatestUpdates(lstLatestUpdates);

    applicationData = applicationDataNew;

    return in;
}

QDebug operator<<(QDebug dbg, const CApplicationData& applicationData)
{
    dbg.nospace()   << g_spacer
                    << "CApplicationData(" << applicationData.getID()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Latest updates:" << "\n";
    dbg.nospace() << g_spacer <<  applicationData.latestUpdates();
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(applicationData.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}


