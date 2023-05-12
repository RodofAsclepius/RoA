#include "balancepaneltreeweights.h"
#include "maincontroller.h"
#include "balancepaneltreeweightsdelegate.h"
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

CBalancePanelTreeWeights::CBalancePanelTreeWeights(CBalancePanelTreeModel *pModel, CBalancePanelTreeSortFilterProxyModel *pProxyModel)
    : mainController_(CMainController::instance()),
      mapColorsFonts_(CColorAndFontMap::instance()),
      rController_(CRController::instance()),
      pProxyModel_(pProxyModel),
      pModel_(pModel)
{
//    CBalancePanelTreeHeader *pHeader = new CBalancePanelTreeHeader(header()->orientation());
//    setHeader(pHeader);

    QColor altRowColor = palette().alternateBase().color();
    QString strStyleSheet = styleSheet();
    strStyleSheet += ""
                     "QTreeView {"
//                        "background-color: #313131;"// + mapColorsFonts_.getColor("widget/chart").name() + ";"
//                        "background-color: white;"// + mapColorsFonts_.getColor("widget/chart").name() + ";"

                        "border: none;"
                     "}"
            ;
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
    header()->setStretchLastSection(false);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* Models */
    pProxyModel_->setSourceModel(pModel_);
    setModel(pProxyModel_);

    updateColumnSizes();
}

CBalancePanelTreeWeights::~CBalancePanelTreeWeights()
{
}

void CBalancePanelTreeWeights::updateTree()
{
//    DEBUG_OBJ_INFO
    setSortingEnabled(false);

    /* Groups */
    updateGroups();

    /* Column sizes */
    updateColumnSizes();

    /* Remove all column delegates */
    for(int j=COLUMN_GROUPWEIGHTS_OFFSET; j<model()->columnCount(); ++j)
    {
        setItemDelegateForColumn(j, nullptr);

        CGroup group = lstGroups_.at(j-COLUMN_GROUPWEIGHTS_OFFSET);

        CBalancePanelTreeWeightsDelegate* pDelegate = new CBalancePanelTreeWeightsDelegate();
        pDelegate->setGroup(group);
        pDelegate->setTotalWidth(width());
        pDelegate->setGroupCount(lstGroups_.count());
        pDelegate->setGroupIndex(j-COLUMN_GROUPWEIGHTS_OFFSET);
        setItemDelegateForColumn(j, pDelegate);
    }
}

void CBalancePanelTreeWeights::updateColumnSizes()
{
    header()->resizeSection(COLUMN_VARIABLE, 0);
    header()->resizeSection(COLUMN_DISTRIBUTION, 0);
    header()->resizeSection(COLUMN_CURVES, 0);

    header()->hideSection(COLUMN_VARIABLE);
    header()->hideSection(COLUMN_DISTRIBUTION);
    header()->hideSection(COLUMN_CURVES);
    header()->hide();

    /* Column sizes */
    int iGroupsCount = lstGroups_.count();
    for(int i=0; i<iGroupsCount; ++i)
    {
        header()->resizeSection(COLUMN_GROUPWEIGHTS_OFFSET+i, COLUMN_GROUP_WIDTH+2);
    }

//    int iSectionWidth = qMax(iGroupsCount * (COLUMN_GROUP_WIDTH + 2), 175);
    int iSectionWidth = qMax(iGroupsCount * (COLUMN_GROUP_WIDTH + 2), 120);
    setFixedWidth(iSectionWidth);
}

CBalanceModelsList CBalancePanelTreeWeights::balanceModels() const
{
    return lstBalanceModels_;
}

void CBalancePanelTreeWeights::setBalanceModels(const CBalanceModelsList &lstBalanceModels)
{
    lstBalanceModels_ = lstBalanceModels;
    updateTree();
}

void CBalancePanelTreeWeights::applyAdjustmentSet(const CAdjustmentSet &adjustmentSet)
{
//    DEBUG_OBJ_INFO

    QString strGroupName = adjustmentSet.group().name();
    QStringList lstVariablesToBeBalanced = adjustmentSet.nodesNames();

    /* Find correct column */
    int iGroupsCount = lstGroups_.count();
    for(int i=0; i<iGroupsCount; ++i)
    {
        QVariant varData = model()->headerData(COLUMN_GROUPWEIGHTS_OFFSET+i, Qt::Horizontal, Qt::DisplayRole);
        CGroup groupData = varData.value<CGroup>();
        if(groupData.name() == strGroupName)
        {
            /* Flip the switches */
            for(int iRow=0; iRow<model()->rowCount(); ++iRow)
            {
                QModelIndex idxVariable = model()->index(iRow, COLUMN_VARIABLE);
                QVariant varVariable = model()->data(idxVariable, Qt::DisplayRole);

                if(varVariable.canConvert<CVariable>())
                {
                    QModelIndex idxCheckBox = model()->index(iRow, COLUMN_GROUPWEIGHTS_OFFSET+i);
                    QVariant varCheckState;

                    CVariable variable = varVariable.value<CVariable>();
                    if(lstVariablesToBeBalanced.contains(variable.name()))
                    {
                        varCheckState = QVariant::fromValue(Qt::CheckState::Checked);
                    }
                    else
                    {
                        varCheckState = QVariant::fromValue(Qt::CheckState::Unchecked);
                    }

                    model()->setData(idxCheckBox, varCheckState, Qt::CheckStateRole);
                }
            }
        }
    }
}

void CBalancePanelTreeWeights::updateGroups()
{
    lstGroups_.clear();

    foreach(CBalanceModel balanceModel, lstBalanceModels_)
    {
        lstGroups_.append(balanceModel.group());
    }
}
