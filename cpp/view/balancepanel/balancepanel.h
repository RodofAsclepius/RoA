#ifndef BALANCEPANEL_H
#define BALANCEPANEL_H

#include "panelwidget.h"
#include "applicationdata.h"
#include "maincontroller.h"

class CBalancePanelTree;
class CBalancePanelTreeWeights;
class CBalanceView;
class QCheckBox;
class CVariablesPanelControl;
class QTreeWidgetItem;
class QPushButton;
class CRefreshButton;
class QComboBox;
class QLineEdit;

class CBalancePanel : public CPanelWidget
{
    Q_OBJECT
public:
    explicit CBalancePanel(QWidget *parent = nullptr);

    CVariablesList variables() const;
    void setVariables(const CVariablesList &variables);

    CBalanceModelsList balanceModels() const;
    void setBalanceModels(const CBalanceModelsList &balanceModels);

protected:
    CMainController& mainController_;
    CRController &rController_;
    CBalancePanelTree* pBalanceTreePanel_;
    CBalancePanelTreeWeights* pBalanceTreePanelWeights_;
    CBalanceView *pBalanceView_;
    QCheckBox *pCheckBoxShowUnadjusted_;
    CRefreshButton *pRefreshButton_;
    QComboBox *pComboBoxSortingMode_;
    QLineEdit *pSearchBar_;

    CVariablesList lstVariables_;
    CBalanceModelsList lstBalanceModels_;

    void updatePanel(bool bForce = false);
    void refreshBalanceView();

    void applyStyleSheet();
    void onItemExpanded(const QModelIndex &index);
    void onItemCollapsed(const QModelIndex &index);
    void onComboSortingModeChanged(const QString& strText);

    void onRefreshButtonClicked();
public slots:
    void onBalancePanelModelUpdated();

    /* RController */
    void onMainControllerVariablesUpdated(const CVariablesList &lstVariables);
    void onMainControllerBalanceModelsUpdated(const CBalanceModelsList& lstBalanceModels);

    /* UI */
    void onCheckBoxShowUnadjustedStateChanged(int iState);
    void onAdjustmentSetSelected(CAdjustmentSet adjustmentSet);
    void onSearchTextChanged(const QString &strText);
};

#endif // BALANCEPANEL_H
