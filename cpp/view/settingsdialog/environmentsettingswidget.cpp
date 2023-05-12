#include "environmentsettingswidget.h"
#include "ui_environmentsettingswidget.h"

#include <QSettings>



CEnvironmentSettingsWidget::CEnvironmentSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CEnvironmentSettingsWidget)
{
    ui->setupUi(this);

    loadSettings();
    updateUI();
}


CEnvironmentSettingsWidget::~CEnvironmentSettingsWidget()
{
    delete ui;
}


void CEnvironmentSettingsWidget::loadSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TU/e", "Asclepius");

    /* System */
    settings.beginGroup("Environment");
        settings.beginGroup("System");
            _bStartFullScreen = settings.value("startFullScreen", true).toBool();
            _bShowPreviousProjectsDialogOnStartup = settings.value("ShowPreviousProjectsDialogOnStartup", true).toBool();
        settings.endGroup();
    settings.endGroup();

    /* RServer */
    settings.beginGroup("Environment");
        settings.beginGroup("RServer");
        _strRServiceBaseUrl = settings.value("ServiceBaseUrl", "http://localhost:4971/").toString();
        settings.endGroup();
    settings.endGroup();
}


void CEnvironmentSettingsWidget::saveSettings() const
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TU/e", "Asclepius");

    /* System */
    settings.beginGroup("Environment");
        settings.beginGroup("System");
            settings.setValue("startFullScreen", _bStartFullScreen);
            settings.setValue("ShowPreviousProjectsDialogOnStartup", _bShowPreviousProjectsDialogOnStartup);
        settings.endGroup();
    settings.endGroup();

    /* RServer */
    settings.beginGroup("Environment");
        settings.beginGroup("RServer");
            settings.setValue("ServiceBaseUrl", QVariant::fromValue(_strRServiceBaseUrl));
        settings.endGroup();
    settings.endGroup();
}


void CEnvironmentSettingsWidget::updateUI()
{
    ui->checkBoxStartFullScreen->setChecked(_bStartFullScreen);
    ui->checkBoxLoadPreviousProjectOnStartup->setChecked(_bShowPreviousProjectsDialogOnStartup);
    ui->lineEditHostnameAndPort->setText(_strRServiceBaseUrl);
}


void CEnvironmentSettingsWidget::on_checkBoxLoadPreviousProjectOnStartup_toggled(bool checked)
{
    _bShowPreviousProjectsDialogOnStartup = checked;
}


void CEnvironmentSettingsWidget::on_lineEditHostnameAndPort_textChanged(const QString &arg1)
{
    _strRServiceBaseUrl = arg1;
}


void CEnvironmentSettingsWidget::on_checkBoxStartFullScreen_toggled(bool checked)
{
    _bStartFullScreen = checked;
}


