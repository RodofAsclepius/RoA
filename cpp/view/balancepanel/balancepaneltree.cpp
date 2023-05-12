#include "balancepaneltree.h"
#include "maincontroller.h"
#include "balancepaneltreedelegate.h"
#include "utils.h"
#include "balancemodel.h"
#include "distributionview.h"
#include "variablebalance.h"
#include "balancepaneldistributionsview.h"
#include "adjustmentset.h"
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QGraphicsLayout>
#include <QLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScrollBar>
#include <QRandomGenerator>
#include <QModelIndex>
#include "colorandfontmap.h"
#include "balancepaneltreeproxymodel.h"
#include "treeitem.h"
#include "balancepaneltreeheader.h"

#include <QBuffer>

CBalancePanelTree::CBalancePanelTree()
    : mainController_(CMainController::instance()),
      mapColorsFonts_(CColorAndFontMap::instance()),
      rController_(CRController::instance())
{
    header()->hide();

    QColor altRowColor = palette().alternateBase().color();
    QString strStyleSheet = styleSheet();

    setStyleSheet(strStyleSheet);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionMode(QAbstractItemView::NoSelection);
    setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    setFocusPolicy(Qt::NoFocus);
    header()->setSectionResizeMode(QHeaderView::Fixed);
    setSortingEnabled(false);
    setUniformRowHeights(true);
    setVerticalScrollMode(ScrollPerPixel);
    header()->setStretchLastSection(true);
    setAllColumnsShowFocus(false);

    /* Models */
    pModel_ = new CBalancePanelTreeModel();
    pProxyModel_ = new CBalancePanelTreeSortFilterProxyModel();

    pProxyModel_->setSourceModel(pModel_);
    setModel(pProxyModel_);

    updateColumnSizes();
}

CBalancePanelTree::~CBalancePanelTree()
{
}

int CBalancePanelTree::treeCount(QTreeWidget *tree, QTreeWidgetItem *parent)
{
    int count = 0;
    if (parent == 0) {
        int topCount = tree->topLevelItemCount();
        for (int i = 0; i < topCount; i++) {
            QTreeWidgetItem *item = tree->topLevelItem(i);
            if (item->isExpanded()) {
                count += treeCount(tree, item);
            }
        }
        count += topCount;
    } else {
        int childCount = parent->childCount();
        for (int i = 0; i < childCount; i++) {
            QTreeWidgetItem *item = parent->child(i);
            if (item->isExpanded()) {
                count += treeCount(tree, item);
            }
        }
        count += childCount;
    }
    return count;
}

int CBalancePanelTree::treeHeight(QTreeWidget *tree, QTreeWidgetItem *parent)
{
    int iGroupsCount = lstGroups_.count();

    int iHeight = 0;
    if (parent == 0)
    {
        int topCount = tree->topLevelItemCount();
        for (int i = 0; i < topCount; i++)
        {
            QTreeWidgetItem *item = tree->topLevelItem(i);
//            iHeight += rowHeight(indexFromItem(item));
            iHeight += rowHeight( model()->index(i, iGroupsCount+COLUMN_VARIABLE));
            if (item->isExpanded()) {
                iHeight += treeHeight(tree, item);
            }
        }
    } else {
        int childCount = parent->childCount();
        for (int i = 0; i < childCount; i++)
        {
            QTreeWidgetItem *item = parent->child(i);
//             iHeight +=  rowHeight(indexFromItem(item));
            iHeight += rowHeight( model()->index(i, iGroupsCount+COLUMN_VARIABLE));
            if (item->isExpanded()) {
                iHeight += treeHeight(tree, item);
            }
        }
    }
    return iHeight;
}

void CBalancePanelTree::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
//    DEBUG_OBJ_INFO

    QTreeView::dataChanged(topLeft, bottomRight, roles);

    if(bNotifyDataChanges)
    {
//        qDebug() << "notify";
        /* Group weights updated, store in balance models */
        if(topLeft.column() >= COLUMN_GROUPWEIGHTS_OFFSET || bottomRight.column() >= COLUMN_GROUPWEIGHTS_OFFSET)
        {
            refreshBalanceModels();
        }
        emit balanceSettingsUpdated();
    }
}

//void CBalancePanelTree::getAllChildren(QModelIndex &index, QModelIndexList &indicies)
//{
//    indicies.push_back(index);
//    for (int i = 0; i != model()->rowCount(index); ++i)
//    {
//        index.
//        QModelIndex idxChild = index.child(i, 0);
//        getAllChildren(idxChild, indicies);
//    }
//}

bool CBalancePanelTree::notifyDataChanges() const
{
    return bNotifyDataChanges;
}

void CBalancePanelTree::setNotifyDataChanges(bool value)
{
    bNotifyDataChanges = value;
}

QMap<CGroup, QVariantList> CBalancePanelTree::groupValues() const
{
    return mapGroupValues_;
}

void CBalancePanelTree::setGroupValues(const QMap<CGroup, QVariantList> &mapGroupValues)
{
    mapGroupValues_ = mapGroupValues;
}

void CBalancePanelTree::setSortingMode(CBalancePanelTree::SortingMode sortingMode)
{
    if(sortingMode == SortingMode::Adjusted)
    {
        pProxyModel_->setSortingMode(CBalancePanelTreeSortFilterProxyModel::SortingMode::Adjusted);
    }

    if(sortingMode == SortingMode::Unadjusted)
    {
        pProxyModel_->setSortingMode(CBalancePanelTreeSortFilterProxyModel::SortingMode::Unadjusted);
    }
    if(sortingMode == SortingMode::Max)
    {
        pProxyModel_->setSortingMode(CBalancePanelTreeSortFilterProxyModel::SortingMode::Max);
    }

    updateTree();
}

void CBalancePanelTree::showVariableByName(const QString &strVariableName)
{
//    DEBUG_OBJ_INFO

    int iRowCount = model()->rowCount(rootIndex());
    for(int iRow=0; iRow<iRowCount; ++iRow)
    {
        QModelIndex idxVariable = model()->index(iRow, COLUMN_VARIABLE);
        QVariant varData = model()->data(idxVariable, Qt::DisplayRole);

        if(varData.canConvert<CVariable>())
        {
            CVariable variable = varData.value<CVariable>();
            if(variable.name() == strVariableName)
            {
                QModelIndex idxDistribution = model()->index(iRow, COLUMN_DISTRIBUTION);
                qDebug() << "found:" << idxDistribution;
//                setCurrentIndex(idxDistribution);
                scrollTo(idxDistribution, QAbstractItemView::ScrollHint::PositionAtTop);
                return;
            }
        }
    }
}

CBalancePanelTreeSortFilterProxyModel *CBalancePanelTree::getProxyModel() const
{
    return pProxyModel_;
}

void CBalancePanelTree::updateTree()
{
    //    DEBUG_OBJ_INFO
    setSortingEnabled(false);
    setNotifyDataChanges(false);

    /* Groups */
    updateGroups();

    /* Variables */
    updateVariables();

    /* Remove all column delegates */
    for(int j=0; j<model()->columnCount(); ++j)
    {
        setItemDelegateForColumn(COLUMN_VARIABLE, nullptr);
    }
    setItemDelegateForColumn(COLUMN_VARIABLE, new CVariablesTableDelegate());
    sortByColumn(COLUMN_VARIABLE, Qt::SortOrder::AscendingOrder);

    /* Curves */
    updateBalanceModels();
    if(!lstBalanceModels_.isEmpty())
    {
        sortByColumn(COLUMN_CURVES, Qt::SortOrder::DescendingOrder);
    }

    /* Column sizes */
    updateColumnSizes();

    /* Show message */
    showMessageInFirstDistribution();

    setNotifyDataChanges(true);
    emit balanceViewNeedsUpdate();
}

void CBalancePanelTree::clearTree()
{

}

void CBalancePanelTree::updateBalanceModels()
{
//    DEBUG_OBJ_INFO

    /* Setup variable name -> row map */
    int iRowCount = model()->rowCount(rootIndex());
    QMap<QString, int> mapVariableNameRow;
    for(int iRow=0; iRow<iRowCount; ++iRow)
    {
        QModelIndex idxVariable = model()->index(iRow, COLUMN_VARIABLE);
        QVariant varData = model()->data(idxVariable, Qt::DisplayRole);

        if(varData.canConvert<CVariable>())
        {
            CVariable variable = varData.value<CVariable>();
            mapVariableNameRow.insert(variable.name(), idxVariable.row());
        }
    }

    /* Sort distributions */
    mapVariableDistributionsTreated_.clear();
    mapVariableDistributionsControl_.clear();
    foreach(CBalanceModel balanceModel, lstBalanceModels_)
    {
        /* Treatment distributions */
        foreach(CDistribution distribution, balanceModel.distributionsTreatment())
        {
            CDistributionsList lstVariableDistributions;

            lstVariableDistributions = mapVariableDistributionsTreated_.value(distribution.variable().name());
            lstVariableDistributions.append(distribution);

            mapVariableDistributionsTreated_[distribution.variable().name()] = lstVariableDistributions;
        }

        /* Control distributions */
        foreach(CDistribution distribution, balanceModel.distributionsControl())
        {
            CDistributionsList lstVariableDistributions;

            lstVariableDistributions = mapVariableDistributionsControl_.value(distribution.variable().name());
            lstVariableDistributions.append(distribution);

            mapVariableDistributionsControl_[distribution.variable().name()] = lstVariableDistributions;
        }
    }

    /* Show distributions */
    foreach(QString strVariableName, mapVariableNameRow.keys())
    {
        int iRow = mapVariableNameRow.value(strVariableName);
        QModelIndex idxDistribution = model()->index(iRow, COLUMN_DISTRIBUTION);

        CDistributionsList lstDistributionsTreated = mapVariableDistributionsTreated_.value(strVariableName);
        CDistributionsList lstDistributionsUntreated = mapVariableDistributionsControl_.value(strVariableName);

        CBalancePanelDistributionsView *pDistributionsView = new CBalancePanelDistributionsView();
        pDistributionsView->setFixedHeight(ROW_HEIGHT);
        pDistributionsView->setDistributionsTreated(lstDistributionsTreated);
        pDistributionsView->setDistributionsUntreated(lstDistributionsUntreated);
        setIndexWidget(idxDistribution, pDistributionsView);
    }

    /* Weights */
    for(int iGroupIndex=0; iGroupIndex<lstBalanceModels_.count(); ++iGroupIndex)
    {
        CBalanceModel balanceModel = lstBalanceModels_.at(iGroupIndex);
        CVariablesList lstVariablesBaseline = balanceModel.variablesBaseline();

        for(int iRow=0; iRow<iRowCount; ++iRow)
        {
            QModelIndex idxVariable = model()->index(iRow, COLUMN_VARIABLE);
            QVariant varData = model()->data(idxVariable);

            if(varData.canConvert<CVariable>())
            {
               CVariable variable = varData.value<CVariable>();
               QModelIndex idxWeights = model()->index(iRow, COLUMN_GROUPWEIGHTS_OFFSET + iGroupIndex);

               bool bIsBaseline = (lstVariablesBaseline.indexByName(variable.name()) != -1);

               int iCheckState = Qt::CheckState::Unchecked;
               if(bIsBaseline)
               {
                   iCheckState = Qt::CheckState::Checked;
               }

               model()->setData(idxWeights, QVariant::fromValue(static_cast<int>(iCheckState)), Qt::CheckStateRole);
            }
        }
    }

    /* Love curves */
    QMap<QString, qreal> mapVariableBalancesUn;
    QMap<QString, qreal> mapVariableBalancesAdj;
    foreach(CBalanceModel balanceModel, lstBalanceModels_)
    {
        CVariableBalanceList lstVariablesBalanced = balanceModel.variablesBalanced();
        foreach(CVariableBalance varBalance, lstVariablesBalanced)
        {
            qreal rMaxValueUn = mapVariableBalancesUn.value(varBalance.name());
            qreal rMaxValueAdj = mapVariableBalancesAdj.value(varBalance.name());

            CVariableBalanceInfo balanceInfo = varBalance.balanceInfo();
            QMap<QString, QMap<QString, qreal>> mapLevelValues = balanceInfo.levelValues();
            foreach(QString strLevel, mapLevelValues.keys())
            {
                QMap<QString, qreal> mapValues = balanceInfo.levelValues().value(strLevel);

                qreal rDiffUn = mapValues.value("Diff.Un");
                rMaxValueUn = qMax(rMaxValueUn, qAbs(rDiffUn));

                qreal rDiffAdj = mapValues.value("Diff.Adj");
                rMaxValueAdj = qMax(rMaxValueAdj, qAbs(rDiffAdj));
            }

            mapVariableBalancesUn[varBalance.name()] = rMaxValueUn;
            mapVariableBalancesAdj[varBalance.name()] = rMaxValueAdj;
        }
    }

    /* Store values */
    for(int iRow=0; iRow<iRowCount; ++iRow)
    {
        QModelIndex idxVariable = model()->index(iRow, COLUMN_VARIABLE);
        QVariant varData = model()->data(idxVariable);

        if(varData.canConvert<CVariable>())
        {
            CVariable variable = varData.value<CVariable>();
            QModelIndex idxCurve = model()->index(iRow, COLUMN_CURVES);

            qreal rMaxValueUn = mapVariableBalancesUn.value(variable.name());
            qreal rMaxValueAdj = mapVariableBalancesAdj.value(variable.name());

            /* Place treatment on bottom? */
            if(!lstBalanceModels_.isEmpty())
            {
                CBalanceModel balanceModel = lstBalanceModels_.first();
                if(variable.name() == balanceModel.treatment().variable().name())
                {
                    rMaxValueUn = 0;
                    rMaxValueAdj = 0;
                }
            }

            QPair<qreal, qreal> pairValues = QPair<qreal, qreal>(rMaxValueUn, rMaxValueAdj);
            model()->setData(idxCurve, QVariant::fromValue(pairValues), Qt::EditRole);
        }
    }
}

void CBalancePanelTree::refreshBalanceModels()
{
//    DEBUG_OBJ_INFO

    /* Loop all groups and weight settings and store them in to the balance models */
    for(int iGroupIndex=0; COLUMN_GROUPWEIGHTS_OFFSET+iGroupIndex < model()->columnCount(); ++iGroupIndex)
    {
        /* Group */
        QVariant varGroup = model()->headerData(COLUMN_GROUPWEIGHTS_OFFSET + iGroupIndex, Qt::Horizontal, Qt::DisplayRole);
        CGroup group = varGroup.value<CGroup>();

        /* Find correct balance model */
        bool bModelFound = false;
        CBalanceModel balanceModel;
        foreach(CBalanceModel balanceModelCurrent, lstBalanceModels_)
        {
            if(balanceModelCurrent.group() == group)
            {
                balanceModel = balanceModelCurrent;
                balanceModel.setGroup(group);
                bModelFound = true;
            }
        }

        if(!bModelFound)
        {
            return;
        }

        /* Loop variables and weight settings in table */
        for(int iRow=0; iRow<model()->rowCount(); ++iRow)
        {
            /* Variable */
            QModelIndex idxVariable = model()->index(iRow, COLUMN_VARIABLE);
            QVariant varVariable = model()->data(idxVariable, Qt::DisplayRole);
            CVariable variable = varVariable.value<CVariable>();

            /* Weight check state */
            QModelIndex idxWeight = model()->index(iRow, COLUMN_GROUPWEIGHTS_OFFSET + iGroupIndex);
            QVariant varWeight = model()->data(idxWeight, Qt::CheckStateRole);
            int iCheckState = varWeight.value<int>();

            CVariablesList lstVariablesBaseline = balanceModel.variablesBaseline();
            if(iCheckState == Qt::CheckState::Checked)
            {
                 /* Previously unchecked */
                if(lstVariablesBaseline.indexOf(variable) == -1)
                {
                     balanceModel.setNeedsRefresh(true);
                     lstVariablesBaseline.append(variable);
                }
            }

            if(iCheckState == Qt::CheckState::Unchecked)
            {
                /* Previously checkend */
                if(lstVariablesBaseline.indexOf(variable) != -1)
                {
                    balanceModel.setNeedsRefresh(true);
                    lstVariablesBaseline.removeOne(variable);
                }
            }

            balanceModel.setVariablesBaseline(lstVariablesBaseline);
        }

        int iIndex = lstBalanceModels_.indexOf(balanceModel);
        if(iIndex >-1)
        {
            lstBalanceModels_.replace(iIndex, balanceModel);
        }
    }
}

void CBalancePanelTree::showMessageInFirstDistribution()
{
    /* Add message to the first one */
    bool bShowMessage = true;
    if(bShowMessage)
    {
        QModelIndex idxDistribution = indexAt(QPoint(5, 5));
        idxDistribution = model()->index(idxDistribution.row(), COLUMN_DISTRIBUTION);
        CApplicationData appData = mainController_.applicationData();
        CTreatment treatment = appData.treatment();

        CBalancePanelDistributionsView *pDistributionsView = dynamic_cast<CBalancePanelDistributionsView *>(indexWidget(idxDistribution));
        if(pDistributionsView)
        {
            /* treatment */
            pDistributionsView->distributionViewTreated()->setUseGroupColorsForMessage(true);
            pDistributionsView->distributionViewTreated()->setMessage("Tr: " + treatment.levelTreatment());
            pDistributionsView->distributionViewTreated()->setShowMessage(true);

            /* untreated */
            pDistributionsView->distributionViewUntreated()->setUseGroupColorsForMessage(true);
            pDistributionsView->distributionViewUntreated()->setMessage("Tr: " + treatment.levelControl());
            pDistributionsView->distributionViewUntreated()->setShowMessage(true);

        }
    }
}

void CBalancePanelTree::updateColumnSizes()
{
    header()->resizeSection(COLUMN_VARIABLE, COLUMN_VARIABLE_WIDTH);
    header()->resizeSection(COLUMN_DISTRIBUTION, COLUMN_DISTRIBUTION_WIDTH);
    header()->resizeSection(COLUMN_CURVES, 0);
    header()->hideSection(COLUMN_CURVES);
    header()->hideSection(COLUMN_VARIABLE);
    header()->hide();

    /* Column sizes */
    int iGroupsCount = lstGroups_.count();
    for(int i=0; i<iGroupsCount; ++i)
    {
        header()->resizeSection(COLUMN_GROUPWEIGHTS_OFFSET + i, COLUMN_GROUP_WIDTH);
        header()->hideSection(COLUMN_GROUPWEIGHTS_OFFSET + i);
    }

    resetIndentation();
//    setIndentation()
}

CVariablesList CBalancePanelTree::variables() const
{
    return lstVariables_;
}

void CBalancePanelTree::setVariables(const CVariablesList &lstVariables)
{
    lstVariables_ = lstVariables;
    updateTree();
}

CBalanceModelsList CBalancePanelTree::balanceModels() const
{
    return lstBalanceModels_;
}

void CBalancePanelTree::setBalanceModels(const CBalanceModelsList &lstBalanceModels)
{
    lstBalanceModels_ = lstBalanceModels;
    updateTree();
}

CBalancePanelTreeModel *CBalancePanelTree::getModel() const
{
    return pModel_;
}

void CBalancePanelTree::updateGroups()
{
    lstGroups_.clear();

    pModel_->removeColumns(0, model()->rowCount());
    foreach(CBalanceModel balanceModel, lstBalanceModels_)
    {
        lstGroups_.append(balanceModel.group());
    }

    /* Store header info */
    pModel_->setGroups(lstGroups_);
}

void CBalancePanelTree::updateVariables()
{
    pModel_->removeRows(0, model()->rowCount());
    pModel_->clearContents();

    for(int i=0;i<lstVariables_.count(); ++i)
    {
        CVariable variable = lstVariables_.at(i);

        pModel_->insertRow(i);
        QModelIndex idxVariable = pModel_->index(i, COLUMN_VARIABLE);

        pModel_->setData(idxVariable, QVariant::fromValue(variable), Qt::EditRole);

        /* Levels */
        QStringList lstLevels = variable.levels();
        pModel_->insertRows(0, lstLevels.count(), idxVariable);

        for(int j=0; j<lstLevels.count(); ++j)
        {
            QString strLevel = lstLevels.at(j);
            QModelIndex idxLevel = pModel_->index(j, COLUMN_VARIABLE, idxVariable);
//            qDebug() << "idxLevel " << idxLevel << strLevel;
            pModel_->setData(idxLevel, QVariant::fromValue(strLevel), Qt::EditRole);
//            pModel_->setData(idxLevel, QVariant::fromValue(QString("Test")), Qt::EditRole);
        }
    }
}

QPair<CVariablesList, QVector<QStringList>> CBalancePanelTree::variablesAndLevels()
{
//    DEBUG_OBJ_INFO
    QPair<CVariablesList, QVector<QStringList>> vecVariablesAndLevels;

    CVariablesList lstVariables;
    QVector<QStringList> lstVariablesLevels;

    /* Iterate through items */
    for(int iRow=0; iRow<model()->rowCount(); ++iRow)
    {
        QModelIndex idxRow = model()->index(iRow, 0);

        QModelIndex idxVariable = model()->index(iRow, COLUMN_VARIABLE);
        CVariable variable = model()->data(idxVariable, Qt::DisplayRole).value<CVariable>();
        lstVariables.append(variable);

        QStringList lstLevels;
        if(isExpanded(idxRow))
        {
            lstLevels = variable.levels();
        }
        else
        {
            lstLevels.append(variable.name());
        }

        lstVariablesLevels.append(lstLevels);
    }

    vecVariablesAndLevels.first = lstVariables;
    vecVariablesAndLevels.second = lstVariablesLevels;

    return vecVariablesAndLevels;
}
