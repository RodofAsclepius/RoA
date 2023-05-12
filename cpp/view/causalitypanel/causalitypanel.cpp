#include "causalitypanel.h"
#include "utils.h"

#include "panellabel.h"
#include "maincontroller.h"
#include "rcontroller.h"
#include <QRadioButton>
#include "causalitypanelgraphview.h"
#include "refreshbutton.h"
#include "splitmergebutton.h"

#include "causalitypanelgraphview.h"
#include "causalitypanelgraphviewnode.h"
#include "causalitypanelgrapviewhedge.h"
#include "causalmodellist.h"
#include <math.h>

#include <QHeaderView>
#include <QGridLayout>
#include <QListWidget>
#include <QSplitter>
#include <QRandomGenerator>
#include <QPushButton>
#include <QComboBox>

#include <QCheckBox>
#include <QFileDialog>
#include "panellabel.h"
#include <QSpinBox>
#include <QButtonGroup>
#include "causalitypanelcombodelegate.h"
#include <QLineEdit>
#include <sortlayoutdialog.h>
#include <minegraphdialog.h>
#include <transfergraphdialog.h>
#include <QRegularExpression>
#include <graphviewoptionsdialog.h>

CCausalityPanel::CCausalityPanel(QWidget *parent)
    : CPanelWidget(parent),
      mainController_(CMainController::instance()),
      rController_(CRController::instance()),
      pGraphView_(new CCausalityPanelGraphView())
{
    panelLabel()->setText("Causal relations");
    setMouseTracking(true);

    /* Button refresh */
    pRefreshButton_ = new CRefreshButton();
    toolBar()->addWidget(pRefreshButton_);
    connect(pRefreshButton_, &QPushButton::clicked, this, &CCausalityPanel::onRefreshButtonClicked);
    QSpacerItem* pSpacerItem = new QSpacerItem(20, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);
    toolBar()->addSpacer(pSpacerItem);

    /* Comboboxes groups */
    pComboBoxGroup_ = new QComboBox();
    pComboBoxGroupComparison_ = new QComboBox();

    toolBar()->addWidget(new CPanelLabel("Graph #1", 2));
    toolBar()->addWidget(pComboBoxGroup_);
    toolBar()->addWidget(new CPanelLabel("Graph #2", 2));
    toolBar()->addWidget(pComboBoxGroupComparison_);
    QSpacerItem* pSpacerItem2 = new QSpacerItem(80, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);
    toolBar()->addSpacer(pSpacerItem2);

    pButtonRemoveEdge_ = new QPushButton("Remove edge");
    connect(pButtonRemoveEdge_, &QPushButton::clicked, this, &CCausalityPanel::onPushButtonRemoveEdgeClicked);
//    toolBar()->addWidget(pButtonRemoveEdge_);

    QSpacerItem* pSpacerItem3 = new QSpacerItem(20, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    toolBar()->addSpacer(pSpacerItem3);

    /* Buttons for sorting, mining and transfering */
    pButtonAutoLayout_ = new QPushButton("Layout");
    connect(pButtonAutoLayout_, &QPushButton::clicked, this, &CCausalityPanel::onPushButtonSortLayoutClicked);
    toolBar()->addWidget(pButtonAutoLayout_);

    pButtonMine_ = new QPushButton("Mine");
    connect(pButtonMine_, &QPushButton::clicked, this, &CCausalityPanel::onPushButtonMineClicked);
    toolBar()->addWidget(pButtonMine_);

    pButtonViewOptions_ = new QPushButton("View options");
    connect(pButtonViewOptions_, &QPushButton::clicked, this, &CCausalityPanel::onPushButtonViewOptionsClicked);
    toolBar()->addWidget(pButtonViewOptions_);

    gridLayout()->addWidget(pGraphView_, 0, 0, 1, 1);

    /* Connect maincontroller */
    connect(&mainController_, &CMainController::causalModelsUpdated, this, &CCausalityPanel::onMainControllerCausalModelsUpdated);
    connect(&mainController_, &CMainController::pathSelected, this, &CCausalityPanel::onPathSelected);
    connect(&rController_, &CRController::causalModelsComputed, this, &CCausalityPanel::onRControllerCausalModelsUpdated);
    connect(&rController_, &CRController::markovModelsComputed, this, &CCausalityPanel::onRControllerMarkovModelsComputed);
    connect(pGraphView_, &CCausalityPanelGraphView::causalModelUpdated, this, &CCausalityPanel::onViewCausalModelUpdated);

    connect(&mainController_, &CMainController::dataCheckCompleted, this, &CCausalityPanel::onDataCheckCompleted);

    applyStyleSheet();

    applyViewSettings();
}

CCausalModelList CCausalityPanel::causalModels() const
{
    return lstCausalModels_;
}

void CCausalityPanel::setCausalModels(const CCausalModelList &lstCausalModels)
{
//    DEBUG_OBJ_INFO

    lstCausalModels_ = lstCausalModels;
    updatePanel();
}

void CCausalityPanel::setMarkovModels(const CMarkovModelList &lstMarkovModels)
{
    lstMarkovModels_ = lstMarkovModels;
}

void CCausalityPanel::applyStyleSheet()
{
    QString strStyleSheet;

    strStyleSheet = styleSheet();
    setStyleSheet(strStyleSheet);

    QString strStyleSheetCheckbox = strStyleSheet;
    strStyleSheetCheckbox +=
                      "QCheckBox::indicator:checked"
                      "{"
                        "image: url(:/view/images/visible.png);"
                      "}"
                      "QCheckBox::indicator:unchecked"
                      "{"
                        "image: url(:/view/images/invisible.png);"
                      "}"
                      "QCheckBox::indicator:checked:hover"
                      "{"
                        "image: url(:/view/images/visible.png);"
                      "}"
                      "QCheckBox::indicator:unchecked:hover"
                      "{"
                        "image: url(:/view/images/invisible.png);"
                      "}"
            ;
//    pCheckShowOutliers_->setStyleSheet(strStyleSheetCheckbox);
}

void CCausalityPanel::mineCausalModel(const CCausalModel &causalModel, qreal rAlpha)
{
    CCausalModelList lstCausalModels;
    lstCausalModels.append(causalModel);

    rController_.computeCausalModels(lstCausalModels, rAlpha);
}

void CCausalityPanel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void CCausalityPanel::onMainControllerCausalModelsUpdated(const CCausalModelList &lstCausalModels)
{
//    DEBUG_OBJ_INFO

    setCausalModels(lstCausalModels);
}

void CCausalityPanel::onRefreshButtonClicked()
{
//    DEBUG_OBJ_INFO

    CApplicationData appData = mainController_.applicationData();
    appData.clearCausalModels();
    appData.setCausalModels(causalModels());
    appData.registerUpdate(CApplicationData::Updates::CausalModels, true);

    mainController_.setForceRecomputeAdjustmentModels(true);
    mainController_.setApplicationData(appData, true);
}

void CCausalityPanel::onPathSelected(CGroup group, QString strPath, bool bOpenPath)
{
//    DEBUG_OBJ_INFO
    selectPath(group, strPath, bOpenPath);
}

void CCausalityPanel::onViewCausalModelUpdated(const CCausalModel &causalModel)
{
//    DEBUG_OBJ_INFO

    lstCausalModels_.replace(causalModel);
    pRefreshButton_->setNeedRefreshState(true);
}

void CCausalityPanel::onRControllerCausalModelsUpdated(const CCausalModelList &lstCausalModels)
{
//    DEBUG_OBJ_INFO

    /* Update causal models with mined ones */
    CCausalModelList lstCausalModelsUpdated;
    foreach(CCausalModel causalModelCurrent, lstCausalModels_)
    {
        foreach(CCausalModel causalModelMined, lstCausalModels)
        {
            if(causalModelCurrent.group().name() == causalModelMined.group().name())
            {
                CGraph graphMined = causalModelMined.graphAlg();
                causalModelCurrent.setGraphAlg(graphMined);
            }
        }
        lstCausalModelsUpdated.append(causalModelCurrent);
    }

    setCausalModels(lstCausalModelsUpdated);
    pRefreshButton_->setNeedRefreshState(true);
}

void CCausalityPanel::onRControllerMarkovModelsComputed(const CMarkovModelList &lstMarkovModels)
{
//    DEBUG_OBJ_INFO

    setMarkovModels(lstMarkovModels);
    applyViewSettings();
}

//void CCausalityPanel::onRadioButtonShowMarkovBlanketToggled(bool bChecked)
//{
////    DEBUG_OBJ_INFO
//    if(bChecked)
//    {
//        recomputeMarkovModels();
//    }
//    else
//    {
//        applyMarkovBlanketOptions();
//    }
//}

void CCausalityPanel::onComboBoxGroupIndexChanged(int)
{
//    DEBUG_OBJ_INFO
    updatePanel();
}

void CCausalityPanel::onComboBoxGroupComparisonIndexChanged(int)
{
//    DEBUG_OBJ_INFO
    updatePanel();
}

void CCausalityPanel::onPushButtonSortLayoutClicked()
{
//    DEBUG_OBJ_INFO

    CSortLayoutDialog dlg;
    if(dlg.exec() == CSortLayoutDialog::Accepted)
    {
        /* Do sort */
        QString strLayoutAlgo = dlg.layoutMethod();
        QVariantMap mapParameters = dlg.parameters();
        pGraphView_->updateLayout(strLayoutAlgo, mapParameters);
    }
}

void CCausalityPanel::onPushButtonMineClicked()
{
//    DEBUG_OBJ_INFO
    CMineGraphDialog dlg;
    dlg.setCausalModels(lstCausalModels_);
    if(dlg.exec() == CMineGraphDialog::Accepted)
    {
        qreal rAlpha = dlg.alpha();
        CCausalModel causalModel = dlg.causalModelSelected();
        mineCausalModel(causalModel, rAlpha);
    }
}

void CCausalityPanel::onPushButtonRemoveEdgeClicked()
{
//    DEBUG_OBJ_INFO
    pGraphView_->removeSelectedEdge();
}

void CCausalityPanel::onPushButtonViewOptionsClicked()
{
    GraphViewOptionsDialog dlg;

    dlg.setShowUnconnected(bShowUnconnectedNodes_);
    dlg.setShowMarkovBlanketPlus(bShowMarkovBlanketPlus_);
    dlg.setMarkovBlanketDistance(iMarkovBlanketPlusDistance_);

    if(dlg.exec() == GraphViewOptionsDialog::Accepted)
    {
        bShowUnconnectedNodes_ = dlg.showUnconnected();
        bShowMarkovBlanketPlus_ = dlg.showMarkovBlanketPlus();
        iMarkovBlanketPlusDistance_ = dlg.markovBlanketDistance();
        applyViewSettings();

        if(bShowMarkovBlanketPlus_)
            recomputeMarkovModels();
    }
}

void CCausalityPanel::onDataCheckCompleted()
{
//    DEBUG_OBJ_INFO
//    pGraphView_->setFocus();
}

void CCausalityPanel::updatePanel()
{
//    DEBUG_OBJ_INFO

    updateCombos();

    updateCausalModels();

    applyViewSettings();

    if(pComboBoxGroupComparison_->currentText() != "None")
        pGraphView_->setMultiGroupEnabled(true);
    else
        pGraphView_->setMultiGroupEnabled(false);
}

void CCausalityPanel::updateCombos()
{
    disconnect(pComboBoxGroup_, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxGroupIndexChanged(int)));
    disconnect(pComboBoxGroupComparison_, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxGroupComparisonIndexChanged(int)));

    QString strGroupNamePrevious1 = pComboBoxGroup_->currentText();
    QString strGroupNamePrevious2 = pComboBoxGroupComparison_->currentText();

    pComboBoxGroup_->clear();
    pComboBoxGroupComparison_->clear();

    pComboBoxGroupComparison_->addItem("None");

    pComboBoxGroup_->setItemDelegate(new CCausalityPanelComboDelegate());
    pComboBoxGroupComparison_->setItemDelegate(new CCausalityPanelComboDelegate());

    foreach(CCausalModel causalModel, lstCausalModels_)
    {
        CGroup group = causalModel.group();

        pComboBoxGroup_->addItem(group.name(), QVariant::fromValue(causalModel));
        pComboBoxGroup_->setItemData(pComboBoxGroup_->count()-1, group.colorPrimary(), Qt::ForegroundRole);

        pComboBoxGroupComparison_->addItem(group.name(), QVariant::fromValue(causalModel));
        pComboBoxGroupComparison_->setItemData(pComboBoxGroupComparison_->count()-1, QBrush(group.colorPrimary()), Qt::ForegroundRole);

//        pComboBoxGroup_->addItem(group.name() + " (mined)", QVariant::fromValue(causalModel));
//        pComboBoxGroup_->setItemData(pComboBoxGroup_->count()-1, QBrush(Qt::gray), Qt::ForegroundRole);

        pComboBoxGroupComparison_->addItem(group.name() + " (mined)", QVariant::fromValue(causalModel));
        pComboBoxGroupComparison_->setItemData(pComboBoxGroupComparison_->count()-1, QBrush(Qt::gray), Qt::ForegroundRole);
    }

    /* Restore settings */
    int iIndex1 = pComboBoxGroup_->findText(strGroupNamePrevious1);
    if(iIndex1 != -1)
    {
        pComboBoxGroup_->setCurrentIndex(iIndex1);
    }

    /* Restore settings */
    int iIndex2 = pComboBoxGroupComparison_->findText(strGroupNamePrevious2);
    if(iIndex2 != -1)
    {
        pComboBoxGroupComparison_->setCurrentIndex(iIndex2);
    }

    manageCombos();

    connect(pComboBoxGroup_, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxGroupIndexChanged(int)));
    connect(pComboBoxGroupComparison_, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxGroupComparisonIndexChanged(int)));
}

void CCausalityPanel::manageCombos()
{
    /* Combo group */
    QString strCurrentText = pComboBoxGroup_->currentText();
    bool bIsMinedNetWork = strCurrentText.endsWith("(mined)");
    CCausalModel causalModel = pComboBoxGroup_->currentData(Qt::UserRole).value<CCausalModel>();

    QColor clrText = bIsMinedNetWork ? Qt::gray :  causalModel.group().colorPrimary();
    QString strStyleSheet = pComboBoxGroup_->styleSheet();
    strStyleSheet +=
                    "QComboBox"
                    "{"
                       "color: " + clrText.name() + ";"
                      "}"
                ;
    pComboBoxGroup_->setStyleSheet(strStyleSheet);

    /* Same models? */
    if(pComboBoxGroup_->currentText() == pComboBoxGroupComparison_->currentText())
    {
        pComboBoxGroupComparison_->setCurrentIndex(0);
    }

    /* Combo group comparison */
    if(pComboBoxGroupComparison_->currentIndex() == 0)
    {
            QString strStyleSheet = pComboBoxGroupComparison_->styleSheet();
            strStyleSheet +=
                "QComboBox"
                "{"
                   "color: lightgray;"
                  "}"
            ;
        pComboBoxGroupComparison_->setStyleSheet(strStyleSheet);
    }
    else
    {
        QString strCurrentText = pComboBoxGroupComparison_->currentText();
        bool bIsMinedNetWork = strCurrentText.endsWith("(mined)");
        CCausalModel causalModel = pComboBoxGroupComparison_->currentData(Qt::UserRole).value<CCausalModel>();

        QColor clrText = bIsMinedNetWork ? Qt::gray :  causalModel.group().colorPrimary();
        QString strStyleSheet = pComboBoxGroupComparison_->styleSheet();
        strStyleSheet +=
                        "QComboBox"
                        "{"
                           "color: " + clrText.name() + ";"
                          "}"
                    ;
        pComboBoxGroupComparison_->setStyleSheet(strStyleSheet);
    }
}

void CCausalityPanel::updateCausalModels()
{
    /* Select the right causal model for selected group */
    QString strCurrentTextGroup = pComboBoxGroup_->currentText();
    bool bUseMindedGraphForGroup = strCurrentTextGroup.endsWith("(mined)");

    QString strCurrentTextComparisonGroup = pComboBoxGroupComparison_->currentText();
    bool bUseMindedGraphForComparisonGroup = strCurrentTextComparisonGroup.endsWith("(mined)");

    CCausalModel causalModelGroup = pComboBoxGroup_->currentData().value<CCausalModel>();
    CCausalModel causalModelComparisonGroup = pComboBoxGroupComparison_->currentData().value<CCausalModel>();

    pGraphView_->initialize(causalModelGroup, causalModelComparisonGroup, bUseMindedGraphForGroup, bUseMindedGraphForComparisonGroup);
}

void CCausalityPanel::recomputeMarkovModels()
{
//    DEBUG_OBJ_INFO

    CApplicationData appData = mainController_.applicationData();
    CVariablesList lstVariables = appData.variables();

    if(!lstCausalModels_.isEmpty())
    {
        rController_.computeMarkovModels(lstCausalModels_, lstVariables);
    }
}

void CCausalityPanel::selectPath(const CGroup &group, const QString &strPath, bool bOpenPath)
{
//    DEBUG_OBJ_INFO
    if(pGraphView_->causalModelGroup().group().name() == group.name())
    {
        QRegularExpression separator("<-|->");
        QStringList lstNodeNames = strPath.split(separator);
        QStringList lstNodeNamesTrimmed;

        foreach(QString strNodeNameUntrimmed, lstNodeNames)
        {
            lstNodeNamesTrimmed.append(strNodeNameUntrimmed.trimmed());
        }

        pGraphView_->highlightNodes(lstNodeNamesTrimmed);
    }
}

void CCausalityPanel::applyViewSettings()
{
    if(bShowMarkovBlanketPlus_)
    {
        foreach(CMarkovModel markovModel, lstMarkovModels_)
        {
            if(markovModel.causalModel().group().name() == pGraphView_->causalModelGroup().group().name())
            {
                pGraphView_->setMarkovBlanketPlusDistance(iMarkovBlanketPlusDistance_);
                pGraphView_->setMarkovModel(markovModel);
            }
        }
    }

    pGraphView_->setShowMarkovBlanketPlus(bShowMarkovBlanketPlus_);
    pGraphView_->setShowUnconnectedNodes(bShowUnconnectedNodes_);
}

//void CCausalityPanelGraphViewContainer::onNetwork1EditRequest()
//{
////    DEBUG_OBJ_INFO
//    CEditNetworkDialog dlg;

//    dlg.setCausalModels(lstCausalModels_);
//    CCausalModel causalModelSelected = pComboBoxGroup_->currentData(Qt::UserRole).value<CCausalModel>();
//    dlg.setActiveModel(causalModelSelected);
//    if(dlg.exec() == QDialog::Accepted)
//    {
//        CCausalModelList lstCausalModelsUpdated = dlg.causalModels();
//        causalModelsUpdated(lstCausalModelsUpdated);
//    }
//}
