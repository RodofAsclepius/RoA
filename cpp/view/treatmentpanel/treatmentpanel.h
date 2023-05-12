#ifndef CTREATMENTPANEL_H
#define CTREATMENTPANEL_H

#include "panelwidget.h"

#include "applicationdata.h"
#include "distributionlist.h"

class CMainController;
class CDistributionView;
class QComboBox;
class QRadioButton;

class CTreatmentPanel : public CPanelWidget
{
    Q_OBJECT
public:
    explicit CTreatmentPanel(QWidget *parent = nullptr);

    CTreatment treatment() const;
    void setTreatment(const CTreatment &treatment);

    CVariablesList variables() const;
    void setVariables(const CVariablesList &lstVariables);

    CGroupsList groups() const;
    void setGroups(const CGroupsList &groups);

protected:
    CMainController& mainController_;
    CRController &rController_;

    QComboBox* pComboBoxTreatment_;
    CDistributionView *pDistributionView;
    QRadioButton *pRadioEstimandATE_;
    QRadioButton *pRadioEstimandATT_;

    CVariablesList lstVariables_;
    CGroupsList lstGroups_;
    CTreatment treatment_;

    void updatePanel();

    void applyStyleSheet();

protected slots:
    void onMainControllerVariablesUpdated(const CVariablesList& variables);
    void onMainControllerGroupsUpdated(const CGroupsList &groups);
    void onMainControllerTreatmentUpdated(const CTreatment& treatment);
    void onRControllerDistributionsComputed(const CDistributionsList &lstDistributions);
    void onComboBoxTreatmentIndexChanged(int ICurrentIndex);
    void onRadioButtonMethodClicked();
};

#endif // CTREATMENTPANEL_H
