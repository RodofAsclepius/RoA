#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUndoStack>
#include <QVariant>
#include "adjustmentset.h"

namespace Ui {
    class MainWindow;
}

class CMainController;
class CRController;
class CProcessingOverlayWidget;
class CGroupsPanel;
class CTreatmentPanel;
class CBalancePanel;
class CCausalityPanel;
class CColorAndFontMap;
class CEffectsPanel;
class CAdjustmentsPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void quit();
    void askQuit();

    void newProject();
    void saveProject(const QString& strFileName);
    void saveProjectAs();
    void loadProject();

    void applyStyleSheet();

signals:
private slots:
    void askLoadPreviousProject();
    void onKeyEsc();
    void on_actionE_xit_triggered();

    void on_action_New_project_triggered();

    void on_action_Save_project_triggered();

    void on_action_Load_project_triggered();

    void on_action_Settings_triggered();

    void on_actionS_ave_project_as_triggered();

    void on_action_About_triggered();

    void on_actionShow_Fullscreen_toggled(bool arg1);

    void on_actionRefresh_balance_models_triggered();

    void on_actionRefresh_causal_models_triggered();

    void on_actionRefresh_effects_models_triggered();

    void on_action_Import_triggered();

    void on_action_Export_triggered();

    void on_actionReset_all_triggered();

    void on_action_Refresh_all_triggered();

    void on_actionAdjustment_models_triggered();

    void on_actionEffect_models_triggered();

    void on_actionConnect_to_database_triggered();

    void on_action_Balance_models_triggered();

    void on_actionMarkov_models_triggered();

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent (QCloseEvent *event) override;
    void loadSettings();
    void saveSettings() const;
    void showSettingsDialog();

    void onMainControllerResetEnvironment();
    void onMainControllerStartProcessing();
    void onMainControllerStopProcessing();

    void onRefreshButtonClicked();

private:
    Ui::MainWindow *ui;
    CMainController &mainController_;
    CRController &rController_;
    CColorAndFontMap &mapColorsAndFonts_;
    CProcessingOverlayWidget *pProcessingOverlayWidget_;
    CGroupsPanel *pGroupsPanel_;
    CTreatmentPanel *pTreatmentPanel;
    CBalancePanel *pBalancePanel_;
    CCausalityPanel *pCausalityPanel_;
    CEffectsPanel *pEffectsPanel_;
    CAdjustmentsPanel *pAdjustmentsPanel_;

    bool bStartFullScreen_;
    bool bLoadPreviousProjectOnStartup_;
};



#endif // MAINWINDOW_H
