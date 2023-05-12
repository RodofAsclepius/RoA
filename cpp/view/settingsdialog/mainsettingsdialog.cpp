
#include "mainsettingsdialog.h"
#include "ui_mainsettingsdialog.h"
#include "environmentsettingswidget.h"
#include "appearancesettingswidget.h"
#include "computationsandcheckssettingswidget.h"

#include "utils.h"



CMainSettingsDialog::CMainSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMainSettingsDialog),
    _mainController(CMainController::instance()),
    _pEnvironmentSettingsWidget(new CEnvironmentSettingsWidget()),
    _pAppearanceSettingsWidget(new CAppearanceSettingsWidget()),
    _pComputationsAndChecksWidget(new CComputationsAndChecksSettingsWidget())
{
    ui->setupUi(this);

    ui->listWidgetCategory->setCurrentRow(0);
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->tabBar()->hide();

    QListWidgetItem* pListWidgetItemEnvironment = new QListWidgetItem(QIcon(":/view/images/iconfinder/screen.png"), "Environment");
    ui->listWidgetCategory->addItem(pListWidgetItemEnvironment);
    ui->tabWidget->addTab(_pEnvironmentSettingsWidget, QIcon(), "Environment");

    QListWidgetItem* pListWidgetItemColorsAndFonts = new QListWidgetItem(QIcon(":/view/images/iconfinder/colorwheel.png"), "Appearance");
    ui->listWidgetCategory->addItem(pListWidgetItemColorsAndFonts);
    ui->tabWidget->addTab(_pAppearanceSettingsWidget, QIcon(), "Appearance");

    QListWidgetItem* pListWidgetItemComputationsAndChecks = new QListWidgetItem(QIcon(":/view/images/iconfinder/compute.png"), "Computations");
    ui->listWidgetCategory->addItem(pListWidgetItemComputationsAndChecks);
    ui->tabWidget->addTab(_pComputationsAndChecksWidget, QIcon(), "Computations");
}


CMainSettingsDialog::~CMainSettingsDialog()
{
    delete ui;
}


void CMainSettingsDialog::showTab(QString strTabName) const
{
    for(int iTab=0; iTab<ui->tabWidget->count(); ++iTab)
    {
        if(ui->tabWidget->tabText(iTab) == strTabName)
        {
            ui->tabWidget->setCurrentIndex(iTab);
            ui->listWidgetCategory->setCurrentRow(iTab);
        }
    }
}

void CMainSettingsDialog::on_listWidgetCategory_currentRowChanged(int currentRow)
{
    ui->tabWidget->setCurrentIndex(currentRow);
}


void CMainSettingsDialog::on_buttonBox_accepted()
{
    _pEnvironmentSettingsWidget->saveSettings();
    _pAppearanceSettingsWidget->saveSettings();
    _pComputationsAndChecksWidget->saveSettings();
}


void CMainSettingsDialog::on_buttonBox_rejected()
{
    close();
}


