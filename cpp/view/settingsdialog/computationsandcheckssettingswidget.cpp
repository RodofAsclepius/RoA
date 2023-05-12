#include "computationsandcheckssettingswidget.h"
#include "ui_computationsandcheckssettingswidget.h"

#include <QSettings>



CComputationsAndChecksSettingsWidget::CComputationsAndChecksSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CComputationsAndChecksSettingsWidget)
{
    ui->setupUi(this);

    loadSettings();
    updateUI();
}


CComputationsAndChecksSettingsWidget::~CComputationsAndChecksSettingsWidget()
{
    delete ui;
}


void CComputationsAndChecksSettingsWidget::loadSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TU/e", "Asclepius");

    /* Computations */
    settings.beginGroup("Computations");
        settings.beginGroup("Statistics");
            _bEnableLowPopulationSizeWarning = settings.value("lowPopulationSizeWarning", true).toBool();
            _iLowPopulationSizeTreshold = settings.value("lowPopulationSizeTreshold", 20).toInt();
        settings.endGroup();
    settings.endGroup();
}


void CComputationsAndChecksSettingsWidget::saveSettings() const
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TU/e", "Asclepius");

    /* Computations */
    settings.beginGroup("Computations");
        settings.beginGroup("Statistics");
            settings.setValue("lowPopulationSizeWarning", _bEnableLowPopulationSizeWarning);
            settings.setValue("lowPopulationSizeTreshold", _iLowPopulationSizeTreshold);
        settings.endGroup();
    settings.endGroup();
}


void CComputationsAndChecksSettingsWidget::updateUI()
{
    ui->checkBoxLowPopulationsSizeWarning->setChecked(_bEnableLowPopulationSizeWarning);
    ui->spinBoxLowPopulationSizeTreshold->setValue(_iLowPopulationSizeTreshold);
}


void CComputationsAndChecksSettingsWidget::on_checkBoxLowPopulationsSizeWarning_toggled(bool checked)
{
    _bEnableLowPopulationSizeWarning = checked;
}


void CComputationsAndChecksSettingsWidget::on_spinBoxLowPopulationSizeTreshold_valueChanged(int arg1)
{
    _iLowPopulationSizeTreshold = arg1;
}


