#ifndef ENVIRONMENTSETTINGSWIDGET_H
#define ENVIRONMENTSETTINGSWIDGET_H


#include <QWidget>


namespace Ui {
class CEnvironmentSettingsWidget;
}



class CEnvironmentSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CEnvironmentSettingsWidget(QWidget *parent = 0);
    ~CEnvironmentSettingsWidget();

    void loadSettings();
    void saveSettings() const;

protected:
    bool _bStartFullScreen;
    bool _bShowPreviousProjectsDialogOnStartup;
    QString _strRServiceBaseUrl;
    void updateUI();

private slots:
    void on_checkBoxLoadPreviousProjectOnStartup_toggled(bool checked);

    void on_lineEditHostnameAndPort_textChanged(const QString &arg1);


    void on_checkBoxStartFullScreen_toggled(bool checked);

private:
    Ui::CEnvironmentSettingsWidget *ui;
};


#endif // ENVIRONMENTSETTINGSWIDGET_H
