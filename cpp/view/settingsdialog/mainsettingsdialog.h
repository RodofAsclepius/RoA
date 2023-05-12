#ifndef MAINSETTINGSDIALOG_H
#define MAINSETTINGSDIALOG_H


#include "maincontroller.h"
#include "computationsandcheckssettingswidget.h"

#include <QDialog>



namespace Ui {
class CMainSettingsDialog;
}


class CEnvironmentSettingsWidget;
class CAppearanceSettingsWidget;

class CMainSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMainSettingsDialog(QWidget *parent = 0);
    ~CMainSettingsDialog();

    void showTab(QString strTabName) const;

private slots:
    void on_listWidgetCategory_currentRowChanged(int currentRow);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::CMainSettingsDialog *ui;
    CMainController& _mainController;
    CEnvironmentSettingsWidget* _pEnvironmentSettingsWidget;
    CAppearanceSettingsWidget* _pAppearanceSettingsWidget;
    CComputationsAndChecksSettingsWidget* _pComputationsAndChecksWidget;
};


#endif // MAINSETTINGSDIALOG_H
