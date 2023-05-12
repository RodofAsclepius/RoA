#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maincontroller.h"

#include "loadprojectdialog.h"
#include "mainsettingsdialog.h"
#include "aboutdialog.h"
#include "importgraphdialog.h"
#include "exportgraphdialog.h"

#include "processingoverlaywidget.h"
#include "utils.h"
#include "newprojectdialog.h"
#include "panellabel.h"
#include "colorandfontmap.h"

#include "groupspanel.h"
#include "treatmentpanel.h"
#include "balancepanel.h"
#include "causalitypanel.h"
#include "effectspanel.h"
#include "adjustmentspanel.h"

#include <QShortcut>
#include <QGridLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QTimer>
#include <QSplitter>
#include <QLineEdit>
#include <QCloseEvent>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mainController_(CMainController::instance()),
    rController_(CRController::instance()),
    mapColorsAndFonts_(CColorAndFontMap::instance()),
    pProcessingOverlayWidget_(new CProcessingOverlayWidget(this))
{
    ui->setupUi(this);

    loadSettings();
    applyStyleSheet();
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    /* Main Layout */
    QGridLayout* pGridLayoutMain = dynamic_cast<QGridLayout*>(centralWidget()->layout());
//    pGridLayout->setContentsMargins(0, 0, 0, 0);

    /* Groups panel */
    pGroupsPanel_ = new CGroupsPanel();
    /* Treatment panel */
    pTreatmentPanel = new CTreatmentPanel();
    /* Balance panel */
    pBalancePanel_ = new CBalancePanel();
    /* Causality panel */
    pCausalityPanel_ = new CCausalityPanel();
    /* Adjustments panel */
    pAdjustmentsPanel_ = new CAdjustmentsPanel();
    /* Effects panel */
    pEffectsPanel_ = new CEffectsPanel();

    /* Top */
    QGridLayout* pGridLayoutTop = new QGridLayout();
    pGridLayoutMain->addLayout(pGridLayoutTop, 0, 0, 1, 2);

    pGridLayoutTop->addWidget(pGroupsPanel_, 0, 0, 1, 1);
    pGridLayoutTop->addWidget(pTreatmentPanel, 0, 1, 1, 1);

    QSplitter *pSplitterEffectsAdjustments = new QSplitter(Qt::Horizontal);
    pGridLayoutTop->addWidget(pSplitterEffectsAdjustments, 0, 2, 1, 1);
    pSplitterEffectsAdjustments->addWidget(pEffectsPanel_);
    pSplitterEffectsAdjustments->addWidget(pAdjustmentsPanel_);
    pSplitterEffectsAdjustments->setStretchFactor(0, 2);
    pSplitterEffectsAdjustments->setStretchFactor(1, 1);

    pGridLayoutMain->addWidget(pBalancePanel_, 1, 0, 1, 1);
    pGridLayoutMain->addWidget(pCausalityPanel_, 1, 1, 1, 1);

    int iTopPanelheight = 175 + 36;
    int iExtra = 0;//600 - 556;
    pBalancePanel_->setFixedWidth(556 + iExtra);
    pAdjustmentsPanel_->setFixedHeight(iTopPanelheight);
    pGroupsPanel_->setFixedWidth(240);
    pGroupsPanel_->setFixedHeight(iTopPanelheight);
    pTreatmentPanel->setFixedWidth(300+iExtra+10);
    pTreatmentPanel->setFixedHeight(iTopPanelheight);
    pEffectsPanel_->setFixedHeight(iTopPanelheight);
//    pEffectsPanel_->setFixedHeight(iTopPanelheight);

    /* Connect to mainController */
    connect(&mainController_, &CMainController::startProcessing, this, &MainWindow::onMainControllerStartProcessing);
    connect(&mainController_, &CMainController::stopProcessing, this, &MainWindow::onMainControllerStopProcessing);

    /* Short cuts */
    QShortcut* pShortcut = new QShortcut(this);
    pShortcut->setKey(Qt::Key_Escape);
    connect(pShortcut, &QShortcut::activated, this, &MainWindow::onKeyEsc);
}

/******************************************************************************/
void MainWindow::askLoadPreviousProject()
{
    CLoadProjectDialog* pLoadProjectDialog = new CLoadProjectDialog(this);
    int iResult = pLoadProjectDialog->exec();
    if(iResult == QDialog::Accepted)
    {
        QString strProjectFile = pLoadProjectDialog->getProjectFile();
        mainController_.loadProject(strProjectFile);
    }
}

MainWindow::~MainWindow()
{
    saveSettings();

    delete ui;
}

void MainWindow::quit()
{
    bool bDoQuit = true;

    if(mainController_.project().isDirty())
    {
        /* Save project ? */
        QMessageBox msgBox;

        msgBox.setWindowTitle("Quit?");
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Save | QMessageBox::Discard);
        msgBox.setDefaultButton(QMessageBox::Discard);
        int ret = msgBox.exec();

        switch(ret)
        {
            case QMessageBox::Save:
            {
                QString strProjectFileName = mainController_.project().currentFileName();

                if(strProjectFileName.isEmpty())
                {
                    saveProjectAs();
                }
                else
                {
                     saveProject(strProjectFileName);
                }
            }
            break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                {
                bDoQuit = false;
                }
            break;
            default:
            break;

            }
    }

    if(bDoQuit)
    {
        QApplication::quit();
    }
}

void MainWindow::askQuit()
{
    QMessageBox msgBox;
    msgBox.setText("Do you want to quit?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes) {
        quit();
    }
}

void MainWindow::newProject()
{
    if(mainController_.project().isDirty())
    {
        /* Save project ? */
        QMessageBox msgBox;

        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
        msgBox.setDefaultButton(QMessageBox::Discard);
        int ret = msgBox.exec();

        switch(ret)
        {
            case QMessageBox::Save:
            {
                QString strProjectFileName = mainController_.project().currentFileName();

                if(strProjectFileName.isEmpty())
                {
                    saveProjectAs();
                }
                else
                {
                     saveProject(strProjectFileName);
                }
            }
            break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                return;
//            break;
        }
    }

    CNewProjectDialog dlg(this);
    if(dlg.exec() == QDialog::Accepted)
    {
        mainController_.newProject();
        CProject& project = mainController_.project();

        CDatabaseConnection dbConnection = dlg.databaseConnection();
        project.setDatabaseConnection(dbConnection);
    }
}

void MainWindow::saveProject(const QString &strFileName)
{
   mainController_.saveProject(strFileName);
}

void MainWindow::saveProjectAs()
{
//    DEBUG_OBJ_INFO
    QString strFileName = QFileDialog::getSaveFileName(this, tr("Save project"), "~/",tr("Project files (*.cvis)"));
    if(!strFileName.isEmpty())
    {
        mainController_.saveProject(strFileName);
    }
}

void MainWindow::loadProject()
{
    if(mainController_.project().isDirty())
        {
            /* Save project ? */
            QMessageBox msgBox;

            msgBox.setText("The document has been modified.");
            msgBox.setInformativeText("Do you want to save your changes?");
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
            msgBox.setDefaultButton(QMessageBox::Discard);
            int ret = msgBox.exec();

            switch(ret)
            {
            case QMessageBox::Save:
            {
                QString strProjectFileName = mainController_.project().currentFileName();

                if(strProjectFileName.isEmpty())
                {
                    saveProjectAs();
                }
                else
                {
                     saveProject(strProjectFileName);
                }
            }
            break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                return;
//                break;
            }
        }

        CLoadProjectDialog* pLoadProjectDialog = new CLoadProjectDialog(this);
        int iResult = pLoadProjectDialog->exec();
        if(iResult == QDialog::Accepted)
        {
            QString strProjectFile = pLoadProjectDialog->getProjectFile();
            mainController_.loadProject(strProjectFile);
        }
}

void MainWindow::applyStyleSheet()
{
    QString strStyleSheet = styleSheet();

    /* Dark theme */
//    strStyleSheet +=
//                    "QDialog { "
//                        "background: #525252;"
//                    "}"
//                    "QWidget, QGroupBox, QLineEdit"
//                     "{"
////                        "background-color: #141414;"
//////                        "background-color: darkgray;"
//                          "background-color: white;"
//                        "border:none;"
//                     "}"
//                    "QTableWidget"
//                    "{"
////                        "background-color: #313131;"
//                        "background-color: #black;"
//                        "border:none;"
//                    "}"
//                    "QRadioButton { background-color: transparent; }"
//                    "QComboBox"
//                      "{"
//                         "height: 28px;"
//                         "border: 1px solid gray;"
////                         "width: 100px;"
//                         "width: 130px;"
//                         "font-size: 28px;"
//                         "font-weight: bold;"
//                         "selection-color: white;"
//                         "selection-background-color: darkgray;"
//                        "}"
//                    "QComboBox::item:selected"
//                    "{"
//                        "color: black;"
//                        "background-color: white;"
//                        "padding-left: 0px;"
//                    "}"
//                    "QListView {"
//                        "background-color: #313131;"
//                    "}"
//                    "QListView:item:selected {"
//                        "color: black;"
//                        "background-color: darkgray;"
//                    "}"
//                    "QListView::item:hover {"
//                        "color: black;"
//                        "selection-background-color: transparent;"
//                    "}"
//                     "QPushButton"
//                      "{"
//                       "border: 1px solid gray;"
//                        "height: 14px;"
//                      "}"
//                    "QTabWidget::pane {"
//                        "background-color: #313131;"
//                        "border: none;"
//                    "}"
//                    "QTabBar::tab {"
//                        "border: 1px solid gray;"
//                        "border-top: none;"
//                        "background-color: #313131;"
//                    "}"
//                    "QTabWidget::tab-bar {"
//                        "background-color: #313131;"
//                    "}"
//                    "QTabWidget::tab-bar {"
//                        "left: 5px;"
//                    "}"
//                    "QCheckBox"
//                     "{"
//                        "border-radius: 2px;"
//                        "border: 1px solid gray;"
//                        "padding-left : 4px;"
//                        "padding-right : 4px;"
//                     "}"
//                     "QCheckBox::indicator {"
//                        "height: 24px;"
//                     "}"
//                    /* Treeview*/
//                    "QTreeView {"
//                        "border: none;"
//                        "show-decoration-selected: 0;"
//                        "selection-background-color: transparent;"
//                        "background-color: #313131;"
//                     "}"
//                    "QTreeView::item:hover {"
//                        "background-color: transparent;"
//                        "border: none;"
//                    "}"
//                    "QTreeView::item:selected {"
//                        "background-color: transparent;"
//                        "border: none;"
//                    "}"
//                    "QTreeView::item:hover, QTreeView::item:hover:selected, QTreeView::item:hover:!selected {"
//                         "border: transparent;"
//                    "}"
//                    "QTreeView::item:selected, QTreeView::item:selected:active, QTreeView::item:selected:!active {"
//                         "border: none;"
//                    "QTreeView::item:selected:hover"
//                    "{"
//                        "#background: rgba(255, 255, 255, 0.8);"
//                        "background-color: transparent;"
//                        "#color: #eff0f1;"
//                    "}"

//                    "}"
//                    "QSplitter::handle"
//                    "{"
//                        "border: none;"
//                    "}"
//                    "QSplitter::handle:hover"
//                    "{"
//                        "background-color: #787876;"
//                    "}"
//                    "QSplitter::handle:horizontal "
//                    "{"
//                        "width: 8px;"
//                        "border: none;"
//                    "}"
//                    "QSplitter::handle:vertical "
//                    "{"
//                        "height: 8px;"
//                        "border: none;"
//                    "}"
//                     "QLineEdit"
//                     "{"
//                         "border: 1px solid gray;"
//                     "}"
//                    ;

    setStyleSheet(strStyleSheet);
}

void MainWindow::onKeyEsc()
{
    quit();
}

void MainWindow::on_actionE_xit_triggered()
{
    quit();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    quit();
    event->accept();
}

void MainWindow::on_action_New_project_triggered()
{
    newProject();
}

void MainWindow::on_action_Save_project_triggered()
{
    QString strProjectFileName = mainController_.project().currentFileName();

    if(strProjectFileName.isEmpty())
    {
        saveProjectAs();
    }
    else if(mainController_.project().isDirty() || true)
    {
//        qDebug() << strProjectFileName;
         saveProject(strProjectFileName);
    }
}

void MainWindow::on_action_Load_project_triggered()
{
    loadProject();
}

void MainWindow::loadSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TU/e", "Asclepius");
    settings.beginGroup("Environment");
        settings.beginGroup("System");
        bStartFullScreen_ = settings.value("startFullScreen", false).toBool();
        bLoadPreviousProjectOnStartup_ = settings.value("ShowPreviousProjectsDialogOnStartup", true).toBool();
        settings.endGroup();
    settings.endGroup();
}

void MainWindow::saveSettings() const
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TU/e", "Asclepius");

    QString strApplicationDir = QCoreApplication::applicationDirPath();

    settings.beginGroup("Environment");
        settings.beginGroup("System");
            settings.setValue("ProjectsDirectory", strApplicationDir + "/../res/projects");
        settings.endGroup();
    settings.endGroup();

    settings.beginGroup("Environment");
        settings.beginGroup("System");
            settings.setValue("ProjectsDirectory", strApplicationDir + "/../res/projects");
        settings.endGroup();
    settings.endGroup();
}

void MainWindow::showSettingsDialog()
{
    CMainSettingsDialog mainSettingsDialog;
    if(mainSettingsDialog.exec() == QDialog::Accepted)
    {
        mainController_.reloadSettings();
    }
}

void MainWindow::onMainControllerResetEnvironment()
{
//    reset();
}

void MainWindow::onMainControllerStartProcessing()
{
    pProcessingOverlayWidget_->show();
}

void MainWindow::onMainControllerStopProcessing()
{
    pProcessingOverlayWidget_->hide();
}

void MainWindow::onRefreshButtonClicked()
{

}

void MainWindow::on_action_Settings_triggered()
{
    showSettingsDialog();
}

void MainWindow::on_actionS_ave_project_as_triggered()
{
    saveProjectAs();
}

void MainWindow::on_action_About_triggered()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::on_actionShow_Fullscreen_toggled(bool arg1)
{
    Q_UNUSED(arg1)
    setWindowState(windowState() ^ Qt::WindowFullScreen);
}

void MainWindow::showEvent(QShowEvent *event)
{
    static bool bFirstTime = true;

    if(bFirstTime)
    {
        bFirstTime = false;

        /* Apply settings */
        if(bStartFullScreen_)
        {
            setWindowState(windowState() | Qt::WindowFullScreen);
        }
        else
        {
            setWindowState(windowState() & ~Qt::WindowFullScreen);
        }

        if(bLoadPreviousProjectOnStartup_)
        {
            QTimer::singleShot(300, this, SLOT(askLoadPreviousProject()));
        }
    }

    QMainWindow::showEvent(event);
}

void MainWindow::on_actionRefresh_balance_models_triggered()
{

}

void MainWindow::on_actionRefresh_causal_models_triggered()
{

}

void MainWindow::on_actionRefresh_effects_models_triggered()
{

}

void MainWindow::on_action_Import_triggered()
{
    CApplicationData appData = mainController_.applicationData();

    CImportGraphDialog dlg;
    dlg.setGroups(appData.groups());
    CCausalModelList lstCausalModels = appData.causalModels();

    if(dlg.exec() == QDialog::Accepted)
    {
        QString strFilename = dlg.filename();
        CGraph graphImported;

        if(graphImported.importCSV(strFilename))
        {
            CCausalModelList lstCausalModelsNew;
            CGroup group = dlg.group();
            bool bImportUserGraph = dlg.ImportUserGraph();
            bool bClearCurrentGraph = dlg.clearCurrentGraph();

            foreach(CCausalModel causalModel, lstCausalModels)
            {
                if(causalModel.group() == group)
                {
                    if(bImportUserGraph)
                    {
                        CGraph graphUser = causalModel.graphUser();
                        if(bClearCurrentGraph)
                        {
                            graphUser.clearEdges();
                        }
                        graphUser.importEdges(graphImported);
                        causalModel.setGraphUser(graphUser);
                    }
                    else
                    {
                        CGraph graphAlg = causalModel.graphAlg();
                        if(bClearCurrentGraph)
                        {
                            graphAlg.clearEdges();
                        }
                        graphAlg.importEdges(graphImported);
                        causalModel.setGraphAlg(graphAlg);
                    }
                }

                lstCausalModelsNew.append(causalModel);
            }

            appData.setCausalModels(lstCausalModelsNew);
            appData.registerUpdate(CApplicationData::Updates::CausalModels);
            mainController_.setApplicationData(appData, true);
        }
    }

}

void MainWindow::on_action_Export_triggered()
{
    CApplicationData appData = mainController_.applicationData();

    CExportGraphDialog dlg;
    CCausalModelList lstCausalModels = appData.causalModels();
    dlg.setCausalModels(lstCausalModels);

    if(dlg.exec() == QDialog::Accepted)
    {
        QString strFilename = dlg.filename();
        CCausalModel causalModelSelected = dlg.causalModelSelected();
        bool bExportUserGraph = dlg.exportUserGraph();
        bool bOpenExportedFile = dlg.openExportedFile();

        CGraph graphToExport;
        if(bExportUserGraph)
        {
            graphToExport = causalModelSelected.graphUser();
        }
        else
        {
            graphToExport = causalModelSelected.graphAlg();
        }

        if(graphToExport.exportCSV(strFilename))
        {
            bOpenExportedFile = true;
            if(bOpenExportedFile)
            {
                qDebug() << "file://" + strFilename;
                QDesktopServices::openUrl(QUrl("file://" + strFilename));
            }
        }
    }
}

void MainWindow::on_actionReset_all_triggered()
{
    CApplicationData appData = mainController_.applicationData();
    appData.setCausalModels(CCausalModelList());
    appData.registerUpdate(CApplicationData::Updates::CausalModels);
    mainController_.setApplicationData(appData, true);
}

void MainWindow::on_action_Refresh_all_triggered()
{
    mainController_.checkManageApplicationData();
}

void MainWindow::on_actionAdjustment_models_triggered()
{
    CApplicationData appData = mainController_.applicationData();
    rController_.computeAdjustMentModels(appData.causalModels());
}

void MainWindow::on_actionEffect_models_triggered()
{
    CApplicationData appData = mainController_.applicationData();
    CEffectsModelsList lstEffectsModelsNew;
    CEffect effect = appData.effect();
    if(effect.isValid())
    {
        foreach(CBalanceModel balanceModel2, appData.balanceModels())
        {
           CEffectsModel effectsModel;
           effectsModel.setEffect(effect);
           effectsModel.setBalanceModel(balanceModel2);
           lstEffectsModelsNew.append(effectsModel);
        }
        rController_.computeEffectsModels(lstEffectsModelsNew);
    }
}

void MainWindow::on_actionConnect_to_database_triggered()
{
    mainController_.applyProject();
}

void MainWindow::on_action_Balance_models_triggered()
{
    CApplicationData appData = mainController_.applicationData();
    CBalanceModelsList lstBalanceModelsNew;
    CVariablesList lstVariables = appData.variables();
    CTreatment treatment = appData.treatment();
    CGroupsList lstGroups = appData.groups().groupsEnabled();

    if(treatment.isValid() && !lstGroups.isEmpty())
    {
        CBalanceModelsList lstBalanceModelsNew;
        foreach(CGroup group, lstGroups)
        {
            CBalanceModel balanceModel(group, treatment);
            balanceModel.setVariables(lstVariables);
            balanceModel.initDistributions();
            lstBalanceModelsNew.append(balanceModel);
        }
        rController_.computeBalanceModels(lstBalanceModelsNew);
    }
}

void MainWindow::on_actionMarkov_models_triggered()
{
//    DEBUG_OBJ_INFO

    CApplicationData appData = mainController_.applicationData();
    CCausalModelList lstCausalModels = appData.causalModels();
    CVariablesList lstVariables = appData.variables();

    if(!lstCausalModels.isEmpty())
    {
        rController_.computeMarkovModels(lstCausalModels, lstVariables);
    }
}

