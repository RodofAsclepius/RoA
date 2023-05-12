#include "maincontroller.h"
#include "project.h"
#include "colorandfontmap.h"
#include "utils.h"
#include <QMapIterator>
#include <QRandomGenerator>
#include <QFile>

class CMainController::impl
{
public:

    impl()
    {
        initialize();
    }

    void initialize()
    {
        _bRControllerConnectionStatusAlive = false;
    }

    bool getRControllerConnectionStatusAlive() const
    {
        return _bRControllerConnectionStatusAlive;
    }

    void setRControllerConnectionStatus(bool bAlive)
    {
        _bRControllerConnectionStatusAlive = bAlive;
    }

protected:
    /* State */
    QMutex _mutex;
    bool _bIsDirty;

    /* Processing  */
    bool _bRControllerConnectionStatusAlive;
};

CMainController::CMainController()
    : project_(CProject()),
      rController_(CRController::instance()),
      mapColorsAndFonts_(CColorAndFontMap::instance())
{
    _pImpl.reset(new CMainController::impl());

    connect(&rController_, &CRController::connectionStatusUpdated, this, &CMainController::onRControllerConnectionStatusUpdated);
    connect(&rController_, &CRController::databaseConnectionStatusUpdated, this, &CMainController::onRControllerDatabaseConnectionStatusUpdated);
    connect(&rController_, &CRController::variablesUpdated, this, &CMainController::onRcontrollerVariablesUpdated);
    connect(&rController_, &CRController::groupSizesComputed, this, &CMainController::onRControllerGroupsSizesComputed);
    connect(&rController_, &CRController::balanceModelsComputed, this, &CMainController::onRControllerBalanceModelsUpdated);
    connect(&rController_, &CRController::effectsModelsComputed, this, &CMainController::onRControllerEffectsModelsUpdated);
    connect(&rController_, &CRController::adjustmentModelsComputed, this, &CMainController::onRControllerAdjustmentModelsUpdated);

    connect(&rController_, &CRController::processing, this, &CMainController::onRControllerProcessing);
    connect(&rController_, &CRController::doneProcessing, this, &CMainController::onRControllerDoneProcessing);

    newProject();
    startTimer(500);
}

void CMainController::timerEvent(QTimerEvent *event)
{
    if(!bDataCheckCompleted_)
    {
        checkManageApplicationData();
    }
}

void CMainController::onProjectStartLoading()
{
//    DEBUG_OBJ_INFO
//    emit startProcessing();
}

void CMainController::onProjectFinishedLoading()
{
//    DEBUG_OBJ_INFO

//    emit stopProcessing();
    emit projectLoaded();

    applyProject();
}

void CMainController::onRControllerConnectionStatusUpdated(bool bAlive)
{
//    DEBUG_OBJ_INFO

    _pImpl->setRControllerConnectionStatus(bAlive);
}

void CMainController::onRControllerDatabaseConnectionStatusUpdated(bool bAlive)
{
    Q_UNUSED(bAlive)
//    DEBUG_OBJ_INFO
}

void CMainController::onRcontrollerVariablesUpdated(const CVariablesList &lstVariables)
{
//    DEBUG_OBJ_INFO

    CApplicationData applicationData = project_.applicationData();
    applicationData.registerUpdate(CApplicationData::Updates::Variables);
    applicationData.setVariables(lstVariables);
    setApplicationData(applicationData, true);
}

void CMainController::onRControllerGroupsSizesComputed(const CGroupsList &lstGroups)
{
//    DEBUG_OBJ_INFO
    CApplicationData appData = project().applicationData();
    appData.setGroups(lstGroups);
    appData.registerUpdate(CApplicationData::Updates::GroupsSizes);
    setApplicationData(appData, true);
}

void CMainController::onRControllerBalanceModelsUpdated(const CBalanceModelsList &lstBalanceModels)
{
    CApplicationData appData = project().applicationData();
    appData.setBalanceModels(lstBalanceModels);
    appData.registerUpdate(CApplicationData::Updates::BalanceModels);
    setApplicationData(appData, true);
}

void CMainController::onRControllerEffectsModelsUpdated(const CEffectsModelsList &lstEffectsModels)
{
//    DEBUG_OBJ_INFO

    CApplicationData appData = project().applicationData();
    appData.setEffectModels(lstEffectsModels);
    appData.registerUpdate(CApplicationData::Updates::EffectsModels);
    setApplicationData(appData, true);
}

void CMainController::onRControllerAdjustmentModelsUpdated(const CAdjustmentModelList &lstAdjustmentModels)
{
//    DEBUG_OBJ_INFO

    CApplicationData appData = project().applicationData();
    appData.setAdjustmentModels(lstAdjustmentModels);
    appData.registerUpdate(CApplicationData::Updates::AdjustmentModels);
    setApplicationData(appData, true);
}

void CMainController::onRControllerProcessing()
{
    emit startProcessing();
}

void CMainController::onRControllerDoneProcessing()
{
    emit stopProcessing();
}

void CMainController::onProjectDatabaseConnectionUpdated()
{
//    emit startProcessing();
    applyProject();
}

void CMainController::onProjectApplicationDataUpdated(CApplicationData applicationData)
{
//    DEBUG_OBJ_INFO

    checkManageApplicationData();
}

CMainController::~CMainController()
{

}

void CMainController::initialize()
{
}

void CMainController::reloadSettings()
{
    rController_.reloadSettings();
    mapColorsAndFonts_.reloadSettings();

    emit settingsUpdated();
}

CProject &CMainController::project()
{
    return project_;
}

void CMainController::newProject()
{
    project_ = CProject(this);

    connect(&project_, &CProject::startLoading, this, &CMainController::onProjectStartLoading);
    connect(&project_, &CProject::finishedLoading, this, &CMainController::onProjectFinishedLoading);
    connect(&project_, &CProject::databaseConnectionUpdated, this, &CMainController::onProjectDatabaseConnectionUpdated);
    connect(&project_, &CProject::applicationDataUpdated, this, &CMainController::onProjectApplicationDataUpdated);
}

void CMainController::applyProject()
{
//    DEBUG_OBJ_INFO

    emit startProcessing();

    if(_pImpl->getRControllerConnectionStatusAlive())
    {
        CDatabaseConnection dbConnection = project().databaseConnection();

        rController_.connectDatabase(dbConnection.databaseName(),
                                     dbConnection.databaseUserName(),
                                     dbConnection.databasePassword(),
                                     dbConnection.databaseMainTableName(),
                                     dbConnection.databaseVariablesTypesTableName());
    }
    else {
        qDebug() << "No database connection. Please fix this and reload the project.";
    }

    checkManageApplicationData(true);

    emit stopProcessing();
}

void CMainController::checkManageApplicationData(bool bInitialize)
{
//    DEBUG_OBJ_INFO

    bDataCheckCompleted_ = false;

    CApplicationData appData = applicationData();
//    qDebug() << appData.latestUpdates();

    /* Check database connection */
    if(databaseConnectionIsAlive())
    {
//        qDebug() << "Database connection alive";
    }
    else
    {
        qDebug() << "Waiting for database connection...";
        return;
    }

    /* Check variables first */
    CVariablesList lstVariables = appData.variables();
    if(bInitialize || lstVariables.isEmpty())
    {
        rController_.getVariables();
        return;
    }

    if(appData.latestUpdates().contains(CApplicationData::Updates::Variables))
    {
        appData.removeUpdate(CApplicationData::Updates::Variables);
        project().setApplicationData(appData, false);
        emit variablesUpdated(lstVariables);
    }

    /* Groups and sizes **************************************************** */
    CGroupsList lstGroups = appData.groups();

    if(bInitialize || lstGroups.isEmpty()) // 0 groups
    {
        CGroup groupNew;
        groupNew.setEnabled(true);
        lstGroups.append(groupNew);

        setGroups(lstGroups);
        return;
    }

    if(lstGroups.count() == 1)     // 1 group
    {
        CGroup group = lstGroups.first();
        if(!group.enabled())
        {
            group.setEnabled(true);
            lstGroups.clear();
            lstGroups.append(group);
            setGroups(lstGroups);
            return;
        }
    }

    if(appData.latestUpdates().contains(CApplicationData::Updates::Groups))
    {
        appData.removeUpdate(CApplicationData::Updates::Groups);
        project().setApplicationData(appData, false);

        rController_.computeGroupSizes(lstGroups);
        return;
    }

    if(appData.latestUpdates().contains(CApplicationData::Updates::GroupsSizes))
    {
        appData.removeUpdate(CApplicationData::Updates::GroupsSizes);
        project().setApplicationData(appData, false);

        emit groupsUpdated(lstGroups);
    }

    /* Enabled groups?  */
    CGroupsList lstGroupsEnabled = lstGroups.groupsEnabled();
    if(lstGroupsEnabled.isEmpty())
    {
        return;
    }

    /* Treatment **************************************************** */
    CTreatment treatment = appData.treatment();
    if(!treatment.isValid())
    {
        return;
    }

    if(appData.latestUpdates().contains(CApplicationData::Updates::Treatment))
    {
        appData.removeUpdate(CApplicationData::Updates::Treatment);
        project().setApplicationData(appData, false);

//        qDebug() << "emit treatment";
        emit treatmentUpdated(treatment);
    }

    /* Balance models **************************************************** */
    CBalanceModelsList lstBalanceModels = appData.balanceModels();

    /* Make sure we have all balance models */
    CBalanceModelsList lstBalanceModelsUpdated;
    bool bBalanceModelsListUpdated = false;

    foreach(CGroup group, lstGroupsEnabled)
    {
        bool bFound = false;
        foreach(CBalanceModel balanceModel, lstBalanceModels)
        {
            if( balanceModel.group().name() == group.name() &&
                balanceModel.treatment().estimationMethod() == treatment.estimationMethod() &&
                balanceModel.treatment().variable().name() == treatment.variable().name())
            {
                lstBalanceModelsUpdated.append(balanceModel); // reuse cached one
                bFound = true;
                break;
            }
        }
        if(!bFound)
        {
            /* add a new balance model */
            CBalanceModel balanceModelNew(group, treatment);
            balanceModelNew.setVariables(lstVariables);
            balanceModelNew.initDistributions();
            balanceModelNew.setNeedsRefresh(true);
            lstBalanceModelsUpdated.append(balanceModelNew);
            bBalanceModelsListUpdated = true;
        }
    }

    /* If bBalanceModelsListUpdated= false and we had old ones now ignored, set the flag */
    if(lstBalanceModels.count() != lstBalanceModelsUpdated.count())
    {
        bBalanceModelsListUpdated = true;
    }

    /* Do we need to refresh a balance model ? */
    CBalanceModelsList lstBalanceModelsCheckedForRefresh;
    bool bRefreshBalanceModels = false;
    foreach(CBalanceModel balanceModel, lstBalanceModelsUpdated)
    {
        if(balanceModel.needsRefresh())
        {
            balanceModel.setNeedsRefresh(false);
            bRefreshBalanceModels = true;
        }

        lstBalanceModelsCheckedForRefresh.append(balanceModel);
    }
    if(bRefreshBalanceModels)
    {
//        if(lstBalanceModelsCheckedForRefresh.isEmpty())
//        {

//        }
        rController_.computeBalanceModels(lstBalanceModelsCheckedForRefresh);
        emit effectModelsNeedUpdate();
        return;
    }

    /* Update list for further processing */
    lstBalanceModels = lstBalanceModelsUpdated;
    if(bBalanceModelsListUpdated)
    {
        /* Store and retry */
        CApplicationData appData = project().applicationData();
        appData.setBalanceModels(lstBalanceModels);
        appData.registerUpdate(CApplicationData::Updates::BalanceModels);
        setApplicationData(appData, true);
        return;
    }

    /* All good, emit if changed */
    bool bAutoMode_ = true;
    bool bDoRecomputeEffectModels = false;
    if(appData.latestUpdates().contains(CApplicationData::Updates::BalanceModels))
    {
        appData.removeUpdate(CApplicationData::Updates::BalanceModels);
        project().setApplicationData(appData, false);
        emit balanceModelsUpdated(lstBalanceModels);

        if(bAutoMode_)
        {
            bDoRecomputeEffectModels = true;
        }
    }

//    bDataCheckCompleted_ = true;
//    return;

    /* Effect **************************************************** */
    CEffect effect = appData.effect();
    if(!effect.isValid())
    {
         return;
    }

    if(appData.latestUpdates().contains(CApplicationData::Updates::Effect))
    {
        emit effectUpdated(effect);
    }

    /* Causal models **************************************************** */
    /* Make sure we have initial causal models */
    bool bCausalModelsUpdated = false;

    CCausalModelList lstCausalModels = appData.causalModels();

    foreach(CGroup group, lstGroupsEnabled)
    {
        bool bFound = false;

        foreach(CCausalModel causalModel, lstCausalModels)
        {
            /* Create initial models */
            if(causalModel.group().name() == group.name() &&
                causalModel.treatment().variable().name() == treatment.variable().name() &&
                causalModel.effect().variable().name() == effect.variable().name())
            {
                bFound = true;
            }
        }

        /* Add */
        if(!bFound)
        {
            CCausalModel causalModel;
            causalModel.setGroup(group);
            causalModel.setVariables(lstVariables);
            causalModel.setTreatment(treatment);
            causalModel.setEffect(effect);
            causalModel.initGraphs();

            lstCausalModels.append(causalModel);
            bCausalModelsUpdated = true;
        }
    }

    /* Remove unused causal models */
    CCausalModelList lstCausalModelsFiltered = lstCausalModels;
    foreach(CCausalModel causalModel, lstCausalModels)
    {
        bool bFound = false;
        foreach(CGroup group, lstGroups)
        {
           if(causalModel.group().name() == group.name() &&
                causalModel.group().name() == group.name() &&
                causalModel.treatment().estimationMethod() == treatment.estimationMethod() &&
                causalModel.effect().variable().name() == effect.variable().name())
            {
                bFound = true;
            }
        }

        if(!bFound)
        {
            lstCausalModelsFiltered.removeOne(causalModel);
            bCausalModelsUpdated = true;
        }
    }

    if(bCausalModelsUpdated)
    {
        CApplicationData appData = project().applicationData();
        appData.setCausalModels(lstCausalModelsFiltered);
        appData.registerUpdate(CApplicationData::Updates::CausalModels);
        setApplicationData(appData, true);
        return;
    }
    else
    {
        if(appData.latestUpdates().contains(CApplicationData::Updates::CausalModels))
        {
            appData.removeUpdate(CApplicationData::Updates::CausalModels);
            project().setApplicationData(appData, false);
//            setForceRecomputeAdjustmentModels(true);
            emit causalModelsUpdated(lstCausalModelsFiltered);
        }
    }

    /* Effects models ****************************************/
    CEffectsModelsList lstEffectsModels = appData.effectModels();
    CEffectsModelsList lstEffectsModelsFiltered;

    if(bForceRecomputeEffectsModels_)
    {
        bDoRecomputeEffectModels = true;
        bForceRecomputeEffectsModels_ = false;
    }

    if(bDoRecomputeEffectModels)
    {
        lstEffectsModels.clear();
        emit effectModelsAutoUpdate();
    }

    /* Remove old ones */
    foreach(CBalanceModel balanceModel, lstBalanceModels)
    {
        bool bFound = false;
        foreach(CEffectsModel effectModel, lstEffectsModels)
        {
            if(effectModel.group() == balanceModel.group() &&
                effectModel.treatment().estimationMethod() == treatment.estimationMethod() &&
                effectModel.treatment().variable().name() == balanceModel.treatment().variable().name() &&
                effectModel.effect().variable().name() == effect.variable().name())
            {
                bFound = true;
                lstEffectsModelsFiltered.append(effectModel);
            }
        }
    }

    /* We have removed no old ones .. */
    if(lstEffectsModelsFiltered.count() != lstEffectsModels.count())
    {
        CApplicationData appData = project().applicationData();
        appData.setEffectModels(lstEffectsModelsFiltered);
        appData.registerUpdate(CApplicationData::Updates::EffectsModels);
        setApplicationData(appData, true);
        return;
    }


    /* Add new effect models? */
    foreach(CBalanceModel balanceModel, lstBalanceModels)
    {
        bool bFound = false;
        foreach(CEffectsModel effectsModel, lstEffectsModelsFiltered)
        {
            if(effectsModel.group().name() == balanceModel.group().name() &&
                    effectsModel.treatment().estimationMethod() == treatment.estimationMethod() &&
                    effectsModel.treatment().variable().name() == balanceModel.treatment().variable().name() &&
                    effectsModel.effect().variable().name() == effect.variable().name())
            {
                bFound = true;
            }
        }
        if(!bFound)
        {
            if(treatment.isValid() && !lstGroupsEnabled.isEmpty())
            {
                CEffectsModelsList lstEffectsModelsNew;
                foreach(CBalanceModel balanceModel2, lstBalanceModels)
                {
                   CEffectsModel effectsModel;
                   effectsModel.setEffect(effect);
                   effectsModel.setBalanceModel(balanceModel2);
                   lstEffectsModelsNew.append(effectsModel);
                }
                rController_.computeEffectsModels(lstEffectsModelsNew);
                return;
            }
        }
    }

    /* All good, emit if changed */
    if(appData.latestUpdates().contains(CApplicationData::Updates::EffectsModels))
    {
        appData.removeUpdate(CApplicationData::Updates::EffectsModels);
        project().setApplicationData(appData, false);
        emit effectsModelsUpdated(lstEffectsModels);
    }

//    bDataCheckCompleted_ = true;
//    return;

    /* Adjustment models ************************************************************ */
    CAdjustmentModelList lstAdjustmentModels = appData.adjustmentModels();
    CAdjustmentModelList lstAdjustmentModelsFiltered;

    if(forceRecomputeAdjustmentModels())
    {
        lstAdjustmentModels.clear();
        setForceRecomputeAdjustmentModels(false);
    }

    /* Remove old adjustment models */
    foreach(CCausalModel causalModel, lstCausalModelsFiltered)
    {
        bool bFound = false;
        foreach(CAdjustmentModel adjustmentModel, lstAdjustmentModels)
        {
            if(adjustmentModel.causalModel().group().name() == causalModel.group().name() &&
                adjustmentModel.causalModel().treatment().variable().name() == causalModel.treatment().variable().name() &&
                adjustmentModel.causalModel().effect().variable().name() == causalModel.effect().variable().name())
            {
                bFound = true;
                lstAdjustmentModelsFiltered.append(adjustmentModel);
            }
        }
    }

    if(lstAdjustmentModelsFiltered.count() != lstAdjustmentModels.count())
    {
        CApplicationData appData = project().applicationData();
        appData.setAdjustmentModels(lstAdjustmentModelsFiltered);
        appData.registerUpdate(CApplicationData::Updates::AdjustmentModels);
        setApplicationData(appData, true);
        return;
    }

    /* Add new adjustment models */
    foreach(CCausalModel causalModel, lstCausalModelsFiltered)
    {
        bool bFound = false;
        foreach(CAdjustmentModel adjustmentModel, lstAdjustmentModelsFiltered)
        {
            if(adjustmentModel.causalModel().group().name() == causalModel.group().name() &&
                adjustmentModel.causalModel().treatment().variable().name() == causalModel.treatment().variable().name() &&
                adjustmentModel.causalModel().effect().variable().name() == causalModel.effect().variable().name())
            {
                bFound = true;
            }
        }
        if(!bFound)
        {
            if(!lstCausalModelsFiltered.isEmpty())
            {
                rController_.computeAdjustMentModels(lstCausalModelsFiltered);
                return;
            }
        }
    }

    /* All good, update state and emit */
    if(appData.latestUpdates().contains(CApplicationData::Updates::AdjustmentModels))
    {
        appData.removeUpdate(CApplicationData::Updates::AdjustmentModels);
        project().setApplicationData(appData, false);
        emit adjustmentModelsUpdated(lstAdjustmentModelsFiltered);
    }

    /* Finish up ************************************************************* */
    bDataCheckCompleted_ = true;
    emit dataCheckCompleted();
}

void CMainController::setSelection(CMainController::SelectionType selectionType, QVariantList selectionData)
{
//    DEBUG_OBJ_INFO

    emit selectionChanged(selectionType, selectionData);
}

void CMainController::selectAdjustmentSet(const CAdjustmentSet &adjustmentSet)
{
    emit adjustmentSetSelected(adjustmentSet);
}

void CMainController::selectPath(const CGroup &group, const QString strPath, bool bOpenPath)
{
    emit pathSelected(group, strPath, bOpenPath);
}

bool CMainController::forceRecomputeBalanceModels() const
{
    return bForceRecomputeAdjustmentModels_;
}

void CMainController::setForceRecomputeBalanceModels(bool bForceRecomputeBalanceModels)
{
    bForceRecomputeAdjustmentModels_ = bForceRecomputeBalanceModels;
}

bool CMainController::forceRecomputeAdjustmentModels() const
{
    return bForceRecomputeAdjustmentModels_;
}

void CMainController::setForceRecomputeAdjustmentModels(bool bForceRecomputeAdjustmentModels)
{
    bForceRecomputeAdjustmentModels_ = bForceRecomputeAdjustmentModels;
}

bool CMainController::forceRecomputeEffectsModels() const
{
    return bForceRecomputeEffectsModels_;
}

void CMainController::setForceRecomputeEffectsModels(bool bForceRecomputeEffectsModels)
{
    bForceRecomputeEffectsModels_ = bForceRecomputeEffectsModels;
}

CApplicationData CMainController::applicationData() const
{
    return project_.applicationData();
}

void CMainController::setApplicationData(const CApplicationData &applicationData, bool bCascadeUpdatedes)
{
    project().setApplicationData(applicationData, bCascadeUpdatedes);
}

void CMainController::setGroups(const CGroupsList &lstGroups)
{
//    DEBUG_OBJ_INFO

    CApplicationData appData = project().applicationData();

//    if(!(appData.groups() == lstGroups))
//    {
        lstGroupsPrevious_ = appData.groups();
        appData.setGroups(lstGroups);
        appData.registerUpdate(CApplicationData::Updates::Groups);
        project().setApplicationData(appData, true);
//    }
}

void CMainController::setTreatment(const CTreatment &aTreatment)
{
//    DEBUG_OBJ_INFO

    CApplicationData appData = project().applicationData();

    if(aTreatment.isValid())
    {
        treatmentPrevious_ = appData.treatment();
        appData.setTreatment(aTreatment);
        appData.registerUpdate(CApplicationData::Updates::Treatment);
        project().setApplicationData(appData, true);
    }
}

void CMainController::setEffect(const CEffect &aEffect)
{
//    DEBUG_OBJ_INFO

    CApplicationData appData = project().applicationData();

    if(!(appData.effect() == aEffect) && aEffect.isValid())
    {
        effectPrevious_ = appData.effect();
        appData.setEffect(aEffect);
        appData.registerUpdate(CApplicationData::Updates::Effect, true);
        project().setApplicationData(appData, true);
    }

    setForceRecomputeEffectsModels(true);
}

void CMainController::closeProject()
{
    project_.close();
}

void CMainController::loadProject(const QString& strFilename)
{
    project_.loadFromFile(strFilename);
}

bool CMainController::deleteProject(const QString &strFilename)
{
    return QFile::remove(strFilename);
}
void CMainController::saveProject(const QString& strFilename)
{
    project_.saveToFile(strFilename);
}

bool CMainController::databaseConnectionIsAlive()
{
    return rController_.databaseConnectionIsAlive();
}



