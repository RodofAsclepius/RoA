#ifndef CADJUSTMENTSPANEL_H
#define CADJUSTMENTSPANEL_H

#include "panelwidget.h"
#include "applicationdata.h"
#include "adjustmentmodellist.h"

class CAdjustmentsTable;
class QTabWidget;
class QCheckBox;

class CAdjustmentsPanel : public CPanelWidget
{
    Q_OBJECT

public:
    CAdjustmentsPanel(QWidget *parent = nullptr);
    ~CAdjustmentsPanel();

    CAdjustmentModelList adjustmentsModels() const;
    void setAdjustmentsModels(const CAdjustmentModelList &adjustmentsModels);

protected:
    CMainController &mainController_;
    QTabWidget* pTabWidget_;

    CAdjustmentsTable *pAdjustmentsTableSets_;
    CAdjustmentsTable *pAdjustmentsTableOpenPaths_;
    CAdjustmentsTable *pAdjustmentsTableOverview_;

    CAdjustmentModelList lstAdjustmentsModels_;

    void updatePanel();
    void applyStyleSheet();

protected slots:
    void onMainControllerAdjustmentsModelsUpdated(const CAdjustmentModelList &lstAdjustmentsModels);
    void onAdjustmentSetSelected(CAdjustmentSet adjustmentSet);
    void onPathSelected(CGroup group, QString strPath, bool bOpenPath);

    void onForwardToOpenPaths(CGroup group);
    void onForwardToAdjustmenSets(CGroup group);
};

#endif // CADJUSTMENTSPANEL_H
