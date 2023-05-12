#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "rcontroller.h"
#include "project.h"
#include "applicationdata.h"
#include "distribution.h"
#include "adjustmentmodellist.h"

#include <memory>
#include <QScopedPointer>

class CColorAndFontMap;
class MainWindow;

class CMainController : public QObject
{
    Q_OBJECT
public:
    /* Singleton definition */
    static CMainController& instance()
    {
        static CMainController instance;
        return instance;
    }
    ~CMainController();

    CMainController(CMainController const&) = delete;
    void operator=(CMainController const&)  = delete;

    /* Init & settings */
    void initialize();
    void reloadSettings();

    /* State */
    CProject& project();
    CRController& RController() const;
    CColorAndFontMap& colorsAndFontysMap();

    /* Data */
    bool databaseConnectionIsAlive();
    CApplicationData applicationData() const;
    void setApplicationData(const CApplicationData &applicationData, bool bCascadeUpdatedes = false);

    void setGroups(const CGroupsList &lstGroups);
    void setTreatment(const CTreatment &treatment);
    void setEffect(const CEffect &aEffect);

    void saveProject(const QString &strFilename);
    void loadProject(const QString &strFilename);
    bool deleteProject(const QString &strFilename);
    void closeProject();
    void newProject();
    void applyProject();

    void checkManageApplicationData(bool bInitialize = false);

    /* UI */
    typedef enum {Group, GroupCouple } SelectionType;
    void setSelection(SelectionType selectionType, QVariantList selectionData);
    void selectAdjustmentSet(const CAdjustmentSet &adjustmentSet);
    void selectPath(const CGroup &group, const QString strPath, bool bOpenPath);

    bool forceRecomputeBalanceModels() const;
    void setForceRecomputeBalanceModels(bool forceRecomputeBalanceModels);

    bool forceRecomputeAdjustmentModels() const;
    void setForceRecomputeAdjustmentModels(bool forceRecomputeAdjustmentModels);

    bool forceRecomputeEffectsModels() const;
    void setForceRecomputeEffectsModels(bool bForceRecomputeEffectsModels);

private:
    QMutex mutex_;
    CProject project_;
    CRController& rController_;
    CColorAndFontMap& mapColorsAndFonts_;

    CGroupsList lstGroupsPrevious_;
    CTreatment treatmentPrevious_;
    CEffect effectPrevious_;

    bool bForceRecomputeBalanceModels_ = false;
    bool bForceRecomputeEffectsModels_ = false;
    bool bForceRecomputeAdjustmentModels_ = false;

    CMainController();

    class impl;
    QScopedPointer<impl> _pImpl;
    bool bDataCheckCompleted_ = false;

private slots:
    virtual void timerEvent(QTimerEvent *event);
    void onProjectStartLoading();
    void onProjectFinishedLoading();

    void onRControllerConnectionStatusUpdated(bool bAlive);
    void onRControllerDatabaseConnectionStatusUpdated(bool bAlive);
    void onRcontrollerVariablesUpdated(const CVariablesList& lstVariables);
    void onRControllerGroupsSizesComputed(const CGroupsList &lstGroups);
    void onRControllerBalanceModelsUpdated(const CBalanceModelsList& lstBalanceModels);
    void onRControllerEffectsModelsUpdated(const CEffectsModelsList& lstEffectsModels);
    void onRControllerAdjustmentModelsUpdated(const CAdjustmentModelList &lstAdjustmentModels);

    void onRControllerProcessing();
    void onRControllerDoneProcessing();

    void onProjectDatabaseConnectionUpdated();
    void onProjectApplicationDataUpdated(CApplicationData applicationData);

signals:
    /* State */
    void newProjectLoaded(); // implement this
    void projectLoaded();
    void settingsUpdated();
    void startProcessing();
    void stopProcessing();

    /* Data */
    void databaseConnectionStatusUpdated(bool bAlive);
    void applicationDataUpdated(CApplicationData applicationData);

    /* Events */
    void variablesUpdated(const CVariablesList& lstVariables);
    void groupsUpdated(const CGroupsList& lstGroups);
    void treatmentUpdated(const CTreatment& treatment);
    void effectUpdated(const CEffect& effect);
    void balanceModelsUpdated(const CBalanceModelsList& lstBalanceModels);
    void effectsModelsUpdated(const CEffectsModelsList& lstEffectsModels);
    void causalModelsUpdated(const CCausalModelList& lstCausalModels);
    void adjustmentModelsUpdated(const CAdjustmentModelList& lstAdujstmentModels);

    /* UI */
    void selectionChanged(SelectionType selectionType, QVariantList selectionData);

    /* Other */
    void adjustmentSetSelected(CAdjustmentSet adjustmentSet);
    void pathSelected(CGroup group, QString strPath, bool bOpenPath);

    void effectModelsNeedUpdate();
    void effectModelsAutoUpdate();

    void dataCheckCompleted();
};

#endif // MAINCONTROLLER_H
