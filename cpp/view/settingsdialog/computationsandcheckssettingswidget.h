#ifndef COMPUTATIONSANDCHECKSSETTINGSWIDGET_H
#define COMPUTATIONSANDCHECKSSETTINGSWIDGET_H


#include <QWidget>


namespace Ui {
    class CComputationsAndChecksSettingsWidget;
}



class CComputationsAndChecksSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CComputationsAndChecksSettingsWidget(QWidget *parent = 0);
    ~CComputationsAndChecksSettingsWidget();

    void loadSettings();
    void saveSettings() const;

protected:
    bool _bEnableLowPopulationSizeWarning;
    int _iLowPopulationSizeTreshold;
    void updateUI();

private slots:

    void on_checkBoxLowPopulationsSizeWarning_toggled(bool checked);

    void on_spinBoxLowPopulationSizeTreshold_valueChanged(int arg1);

private:
    Ui::CComputationsAndChecksSettingsWidget *ui;
};


#endif // COMPUTATIONSANDCHECKSSETTINGSWIDGET_H
