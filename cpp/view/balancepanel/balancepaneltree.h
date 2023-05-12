#ifndef BALANCESPANELTABLE_H
#define BALANCESPANELTABLE_H

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

class CBalancePanelTree : public QTreeView
{
    Q_OBJECT
public:
    CBalancePanelTree();
    virtual ~CBalancePanelTree() override;

    QPair<CVariablesList, QVector<QStringList>> variablesAndLevels();
    void updateTree();
    void clearTree();

    int rowheight() const;
    void setRowHeight(int value);

//    void getAllChildren(QModelIndex &index, QModelIndexList &indicies);
    void updateColumnSizes();

    CVariablesList variables() const;
    void setVariables(const CVariablesList &lstVariables);

    CBalanceModelsList balanceModels() const;
    void setBalanceModels(const CBalanceModelsList &lstBalanceModels);

    CBalancePanelTreeModel *getModel() const;
    CBalancePanelTreeSortFilterProxyModel *getProxyModel() const;

    bool notifyDataChanges() const;
    void setNotifyDataChanges(bool value);

    QMap<CGroup, QVariantList> groupValues() const;
    void setGroupValues(const QMap<CGroup, QVariantList> &mapGroupValues);

    typedef enum
    {
        Adjusted,
        Unadjusted,
        Max
    } SortingMode;
    void setSortingMode(SortingMode sortingMode);

    void showVariableByName(const QString &strVariableName);

protected:
    CMainController &mainController_;
    CColorAndFontMap &mapColorsFonts_;
    CRController &rController_;
    CBalancePanelTreeSortFilterProxyModel *pProxyModel_;
    CBalancePanelTreeModel *pModel_;

    CGroupsList lstGroups_;
    CVariablesList lstVariables_;
    CBalanceModelsList lstBalanceModels_;

    QMap<QString, CDistributionsList> mapVariableDistributionsTreated_;
    QMap<QString, CDistributionsList> mapVariableDistributionsControl_;

    QMap<CGroup, QVariantList> mapGroupValues_;

    /* UI */
    bool bShowUnadjusted_ = false;
    bool bNotifyDataChanges = false;

    void updateGroups();
    void updateVariables();
    void updateBalanceModels();
    void refreshBalanceModels();
    void showMessageInFirstDistribution();

    /* State */
    int treeCount(QTreeWidget *tree, QTreeWidgetItem *parent = nullptr);
    int treeHeight(QTreeWidget *tree, QTreeWidgetItem *parent = nullptr);
//    int treeHeightNew(QTreeWidget *tree, QTreeWidgetItem *parent = nullptr);

    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()) override;

protected slots:

signals:
    void balanceSettingsUpdated();
    void balanceViewNeedsUpdate();
};

#endif // BALANCESPANELTABLE_H
