#ifndef BALANCESPANELTABLEWEIGHT_H
#define BALANCESPANELTABLEWEIGHT_H

#include <QTreeView>
#include "variableslist.h"
#include "balancemodel.h"
#include "applicationdata.h"
#include "groupslist.h"
#include "distributionlist.h"
#include <QPixmap>
#include "balancepaneltreemodel.h"

class CMainController;
class CColorAndFontMap;
class QGraphicsScene;
class CChartLove;
class CDistributionView;
class CAdjustmentModel;
class CBalancePanelTreeModel;
class CTreeModelProxy;
class CBalancePanelTreeSortFilterProxyModel;

class QTreeWidget;
class QTreeWidgetItem;

class CBalancePanelTreeWeights : public QTreeView
{
    Q_OBJECT
public:
    CBalancePanelTreeWeights(CBalancePanelTreeModel *pModel, CBalancePanelTreeSortFilterProxyModel *pProxyModel);
    virtual ~CBalancePanelTreeWeights() override;

    QPair<CVariablesList, QVector<QStringList>> variablesAndLevels();
    void updateTree();

    int rowheight() const;
    void setRowHeight(int value);
    void updateColumnSizes();

    CBalanceModelsList balanceModels() const;
    void setBalanceModels(const CBalanceModelsList &lstBalanceModels);

    void applyAdjustmentSet(const CAdjustmentSet &adjustmentSet);

protected:
    CMainController &mainController_;
    CColorAndFontMap &mapColorsFonts_;
    CRController &rController_;
    CBalancePanelTreeSortFilterProxyModel *pProxyModel_;
    CBalancePanelTreeModel *pModel_;

    CGroupsList lstGroups_;
    CBalanceModelsList lstBalanceModels_;

    void updateGroups();

protected slots:

signals:
    void updated(CBalanceModel balanceModel);
};

#endif // BALANCESPANELTABLEWEIGHT_H
