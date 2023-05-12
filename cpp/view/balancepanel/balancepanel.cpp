#include "balancepanel.h"
#include "balancepaneltree.h"
#include "balancepaneltreeweights.h"
#include "panellabel.h"
#include "utils.h"
#include "rcontroller.h"
#include "balanceview.h"
#include "treemodel.h"
#include "refreshbutton.h"
#include "balancepaneltreemodel.h"

#include <QGridLayout>
#include <QTableWidget>
#include <QSplitter>
#include <QCheckBox>
#include <QScrollBar>
#include "extendedgraphicsview.h"
#include <QHeaderView>
#include "balanceviewheader.h"
#include <QComboBox>
#include <QLineEdit>

using namespace utils;

CBalancePanel::CBalancePanel(QWidget *parent)
    : CPanelWidget(parent),
      mainController_(CMainController::instance()),
      rController_(CRController::instance())
{
    /* Labels */
    panelLabel()->setText("Balance");
    setMouseTracking(true);

    /* Refresh button */
    pRefreshButton_ = new CRefreshButton();
    toolBar()->addWidget(pRefreshButton_);
    connect(pRefreshButton_, &QPushButton::clicked, this, &CBalancePanel::onRefreshButtonClicked);

    QSpacerItem *pSpacer = new QSpacerItem(20, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);
    toolBar()->addSpacer(pSpacer);

    /* Comboboxes groups */
    pComboBoxSortingMode_ = new QComboBox();
    pComboBoxSortingMode_->setFixedWidth(160);
    pComboBoxSortingMode_->setFixedHeight(30);
    pComboBoxSortingMode_->addItem("Sort on adj. values");
    pComboBoxSortingMode_->addItem("Sort on un. values");
    pComboBoxSortingMode_->addItem("Sort on max. values");
    connect(pComboBoxSortingMode_, &QComboBox::currentTextChanged, this, &CBalancePanel::onComboSortingModeChanged);

//    toolBar()->addWidget(new CPanelLabel("Sort on", 2));
    toolBar()->addWidget(pComboBoxSortingMode_);
    toolBar()->addSpacer();

    /* Show unadjusted */
    pCheckBoxShowUnadjusted_ = new QCheckBox("Unadjusted");
    toolBar()->addWidget(pCheckBoxShowUnadjusted_);

    /* Balance view */
    pBalanceView_ = new CBalanceView();
    pBalanceView_->setRowHeight(ROW_HEIGHT);
    pBalanceView_->setMinimumWidth(150);
    gridLayout()->addWidget(static_cast<QWidget*>(pBalanceView_->header()), 0, 0, 1, 1);
    gridLayout()->addWidget(pBalanceView_, 0, 2, 1, 1);

    /* Variable balance table */
    pBalanceTreePanel_ = new CBalancePanelTree();
    pBalanceTreePanel_->setFixedWidth(200);
    pBalanceTreePanel_->setSortingMode(CBalancePanelTree::SortingMode::Adjusted);
    gridLayout()->addWidget(pBalanceTreePanel_, 0, 0, 1, 1);

    /* Groups weights panel */
    pBalanceTreePanelWeights_ = new CBalancePanelTreeWeights(pBalanceTreePanel_->getModel(), pBalanceTreePanel_->getProxyModel());
    pBalanceTreePanelWeights_->setMinimumWidth(0);
    gridLayout()->addWidget(pBalanceTreePanelWeights_, 0, 1, 1, 1);

    /* Search bar */
    pSearchBar_ = new QLineEdit();
    pSearchBar_->setPlaceholderText("Search...");
    gridLayout()->addWidget(pSearchBar_, 1, 0, 1, 3);
    connect(pSearchBar_, &QLineEdit::textChanged, this, &CBalancePanel::onSearchTextChanged);

    /* Connect main and r controller */
    connect(&mainController_, &CMainController::variablesUpdated, this, &CBalancePanel::onMainControllerVariablesUpdated);
    connect(&mainController_, &CMainController::balanceModelsUpdated, this, &CBalancePanel::onMainControllerBalanceModelsUpdated);
    connect(&mainController_, &CMainController::adjustmentSetSelected, this, &CBalancePanel::onAdjustmentSetSelected);

    /* UI */
    connect(pBalanceTreePanel_, &CBalancePanelTree::balanceSettingsUpdated, this, &CBalancePanel::onBalancePanelModelUpdated);
    connect(pBalanceTreePanel_, &CBalancePanelTree::expanded, this, &CBalancePanel::onItemExpanded);
    connect(pBalanceTreePanel_, &CBalancePanelTree::collapsed, this, &CBalancePanel::onItemCollapsed);
    connect(pCheckBoxShowUnadjusted_, &QCheckBox::stateChanged, this, &CBalancePanel::onCheckBoxShowUnadjustedStateChanged);

    connect(pBalanceTreePanel_->verticalScrollBar(), &QAbstractSlider::valueChanged, pBalanceView_->pView()->verticalScrollBar(), &QAbstractSlider::setValue);
    connect(pBalanceView_->pView()->verticalScrollBar(), &QAbstractSlider::valueChanged, pBalanceTreePanel_->verticalScrollBar(), &QAbstractSlider::setValue);

    connect(pBalanceTreePanel_->verticalScrollBar(), &QAbstractSlider::valueChanged, pBalanceTreePanelWeights_->verticalScrollBar(), &QAbstractSlider::setValue);
    connect(pBalanceTreePanelWeights_->verticalScrollBar(), &QAbstractSlider::valueChanged, pBalanceTreePanel_->verticalScrollBar(), &QAbstractSlider::setValue);

    applyStyleSheet();
}

CVariablesList CBalancePanel::variables() const
{
    return lstVariables_;
}

void CBalancePanel::setVariables(const CVariablesList &lstVariables)
{
    lstVariables_ = lstVariables;
    updatePanel();
}

CBalanceModelsList CBalancePanel::balanceModels() const
{
    return lstBalanceModels_;
}

void CBalancePanel::setBalanceModels(const CBalanceModelsList &lstBalanceModels)
{
//    DEBUG_OBJ_INFO

    lstBalanceModels_ = lstBalanceModels;
    updatePanel();
}

void CBalancePanel::applyStyleSheet()
{
    QString strStyleSheet;

    strStyleSheet = styleSheet();
//    strStyleSheet += "QComboBox"
//                     "{"
//                       "height: 17px;"
//                       "border: 1px solid gray;"
//                       "width: 175px;"
//                       "font-size: 18px;"
//                      "}"
//            ;
    setStyleSheet(strStyleSheet);

    QString strStyleSheetCheckbox = strStyleSheet;
    strStyleSheetCheckbox +=
                      "QCheckBox::indicator:checked"
                      "{"
                        "image: url(:/view/images/ui/visible.png);"
                      "}"
                      "QCheckBox::indicator:unchecked"
                      "{"
                        "image: url(:/view/images/ui/invisible.png);"
                      "}"
            ;
    pCheckBoxShowUnadjusted_->setStyleSheet(strStyleSheetCheckbox);

}

void CBalancePanel::updatePanel(bool bForce)
{
//    DEBUG_OBJ_INFO

    int iPosY = pBalanceTreePanel_->verticalScrollBar()->value();
    pBalanceTreePanel_->setVariables(lstVariables_);

    if(!lstBalanceModels_.isEmpty())
    {
        pBalanceTreePanel_->setBalanceModels(lstBalanceModels_);
        pBalanceTreePanelWeights_->setBalanceModels(lstBalanceModels_);

        bool bShowUnadjustedValues = pCheckBoxShowUnadjusted_->checkState() == Qt::Checked;
        pBalanceView_->setShowUnadjustedValues(bShowUnadjustedValues);
    }
    else
    {
        pBalanceView_->clear();
        pBalanceTreePanel_->setBalanceModels(CBalanceModelsList());
        pBalanceTreePanelWeights_->setBalanceModels(CBalanceModelsList());
    }

    refreshBalanceView();

    pBalanceTreePanel_->verticalScrollBar()->setValue(iPosY);
    pBalanceTreePanel_->updateColumnSizes();
    pBalanceTreePanelWeights_->verticalScrollBar()->setValue(iPosY);
    pBalanceTreePanelWeights_->updateColumnSizes();
    pBalanceView_->header()->setFixedHeight(pBalanceTreePanelWeights_->header()->height());
}

void CBalancePanel::refreshBalanceView()
{
    int iPosY = pBalanceView_->pView()->verticalScrollBar()->value();

    /* We need to simply pass the values now */
    QMap<CGroup, QVariantList> mapGroupValues = pBalanceTreePanel_->groupValues();
    pBalanceView_->setBalanceModels(lstBalanceModels_);

    QPair<CVariablesList, QVector<QStringList>> variablesAndLevels = pBalanceTreePanel_->variablesAndLevels();
    pBalanceView_->setVariablesAndLevels(variablesAndLevels);
    bool bShowUnadjustedValues = pCheckBoxShowUnadjusted_->checkState() == Qt::Checked;
    pBalanceView_->setShowUnadjustedValues(bShowUnadjustedValues);
    pBalanceView_->updateView(true);

    pBalanceView_->pView()->verticalScrollBar()->setValue(iPosY);
}

void CBalancePanel::onBalancePanelModelUpdated()
{
//    DEBUG_OBJ_INFO

    pRefreshButton_->setNeedRefreshState();
}

void CBalancePanel::onMainControllerVariablesUpdated(const CVariablesList &lstVariables)
{
//    DEBUG_OBJ_INFO
    setVariables(lstVariables);
}

void CBalancePanel::onMainControllerBalanceModelsUpdated(const CBalanceModelsList &lstBalanceModels)
{
//    DEBUG_OBJ_INFO
    setBalanceModels(lstBalanceModels);
}

void CBalancePanel::onCheckBoxShowUnadjustedStateChanged(int iState)
{
//    DEBUG_OBJ_INFO

    switch(iState)
    {
        case Qt::Unchecked:
            pBalanceView_->setShowUnadjustedValues(false);
            break;
        case Qt::PartiallyChecked:
        case Qt::Checked:
            pBalanceView_->setShowUnadjustedValues(true);
            break;
    }
}

void CBalancePanel::onAdjustmentSetSelected(CAdjustmentSet adjustmentSet)
{
//    DEBUG_OBJ_INFO
    pBalanceTreePanelWeights_->applyAdjustmentSet(adjustmentSet);
}

void CBalancePanel::onSearchTextChanged(const QString &strText)
{
//    DEBUG_OBJ_INFO
    pBalanceTreePanel_->showVariableByName(strText);
}

void CBalancePanel::onItemExpanded(const QModelIndex &index)
{
//    DEBUG_OBJ_INFO

    pBalanceTreePanelWeights_->expand(index);
    refreshBalanceView();
}

void CBalancePanel::onItemCollapsed(const QModelIndex &index)
{
//    DEBUG_OBJ_INFO

    pBalanceTreePanelWeights_->collapse(index);
    refreshBalanceView();
}

void CBalancePanel::onComboSortingModeChanged(const QString &strText)
{
//    DEBUG_OBJ_INFO
//    qDebug() << strText;

    if(strText == "Sort on adj. values")
    {
        pBalanceTreePanel_->setSortingMode(CBalancePanelTree::SortingMode::Adjusted);
    }
    if(strText == "Sort on un. values")
    {
        pBalanceTreePanel_->setSortingMode(CBalancePanelTree::SortingMode::Unadjusted);
    }
    if(strText == "Sort on max. values")
    {
        pBalanceTreePanel_->setSortingMode(CBalancePanelTree::SortingMode::Max);
    }

    updatePanel();
//    refreshBalanceView();
}

void CBalancePanel::onRefreshButtonClicked()
{
//    DEBUG_OBJ_INFO

    CBalanceModelsList lstBalanceModels = pBalanceTreePanel_->balanceModels();

    CApplicationData appData = mainController_.applicationData();
    appData.clearBalanceModels();
    appData.setBalanceModels(lstBalanceModels);
    appData.registerUpdate(CApplicationData::Updates::BalanceModels, true);
    mainController_.setApplicationData(appData, true);
}



