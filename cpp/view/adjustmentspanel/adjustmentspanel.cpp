#include "adjustmentspanel.h"
#include "utils.h"
#include "panellabel.h"
#include "maincontroller.h"
#include "adjustmentstable.h"
#include <QGridLayout>
#include <QTabWidget>
#include <QCheckBox>

CAdjustmentsPanel::CAdjustmentsPanel(QWidget *parent)
    : CPanelWidget(parent),
        mainController_(CMainController::instance()),
        pTabWidget_(new QTabWidget()),
        pAdjustmentsTableSets_(new CAdjustmentsTable()),
        pAdjustmentsTableOpenPaths_(new CAdjustmentsTable()),
        pAdjustmentsTableOverview_(new CAdjustmentsTable())
{
    applyStyleSheet();

    panelLabel()->setText("Diagnostics");

    /* Refresh button */
    QSpacerItem *pSpacer = new QSpacerItem(20, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    toolBar()->addSpacer(pSpacer);

    /* tabs */
    pTabWidget_->setTabPosition(QTabWidget::TabPosition::South);

    pTabWidget_->addTab(pAdjustmentsTableOverview_, "Overview");
    pTabWidget_->addTab(pAdjustmentsTableSets_, "Adjustment sets");
    pTabWidget_->addTab(pAdjustmentsTableOpenPaths_, "Open paths");

    pAdjustmentsTableOverview_->setShowMode(CAdjustmentsTable::ShowMode::Overview);
    pAdjustmentsTableSets_->setShowMode(CAdjustmentsTable::ShowMode::AdjustmenSets);
    pAdjustmentsTableOpenPaths_->setShowMode(CAdjustmentsTable::ShowMode::OpenPaths);

    gridLayout()->addWidget(pTabWidget_, 0, 0, 1, 1);

    /* Connect maincontroller */
    connect(&mainController_, &CMainController::adjustmentModelsUpdated, this, &CAdjustmentsPanel::onMainControllerAdjustmentsModelsUpdated);
    connect(pAdjustmentsTableSets_, &CAdjustmentsTable::adjustmentSetSelected, this, &CAdjustmentsPanel::onAdjustmentSetSelected);
    connect(pAdjustmentsTableOpenPaths_, &CAdjustmentsTable::pathSelected, this, &CAdjustmentsPanel::onPathSelected);
    connect(pAdjustmentsTableOverview_, &CAdjustmentsTable::forwardToOpenPaths, this, &CAdjustmentsPanel::onForwardToOpenPaths);
    connect(pAdjustmentsTableOverview_, &CAdjustmentsTable::forwardToAdjustmentSets, this, &CAdjustmentsPanel::onForwardToAdjustmenSets);
}

CAdjustmentsPanel::~CAdjustmentsPanel()
{

}

CAdjustmentModelList CAdjustmentsPanel::adjustmentsModels() const
{
    return lstAdjustmentsModels_;
}

void CAdjustmentsPanel::setAdjustmentsModels(const CAdjustmentModelList &lstAdjustmentsModels)
{
    lstAdjustmentsModels_ = lstAdjustmentsModels;
    updatePanel();
}

void CAdjustmentsPanel::applyStyleSheet()
{
    QString strStyleSheet;

    strStyleSheet = styleSheet();

    strStyleSheet +=
        "QCheckBox::indicator:checked"
        "{"
        "image: url(:/view/images/ui/visible.png);"
        "}"
        "QCheckBox::indicator:unchecked"
        "{"
        "image: url(:/view/images/ui/invisible.png);"
        "}"
//         "QTabWidget::pane {"
//             "border-bottom: 1px solid gray;"
//         "}"
    ;
//    strStyleSheet += "QCheckBox"
//                    "{"
//                      "border: 1px solid gray;"
//                      "border-radius: 2px;"
//                    "}"
//                     "QCheckBox::indicator:checked"
//                     "{"
//                       "image: url(:/view/images/visible.png);"
//                     "}"
//                     "QCheckBox::indicator:unchecked"
//                     "{"
//                       "image: url(:/view/images/invisible.png);"
//                     "}"
//                     "QCheckBox::indicator:checked:hover"
//                     "{"
//                       "image: url(:/view/images/visible.png);"
//                     "}"
//                     "QCheckBox::indicator:unchecked:hover"
//                     "{"
//                       "image: url(:/view/images/invisible.png);"
//                     "}"
//                     "QTabWidget::pane {"
////                         "background-color: #313131;"
//                           "background-color: black;"
//                         "border: none;"
//                         "margin: -6px -6px  0px -6px;"
//                         "border-bottom: 1px solid gray;"
//                     "}"
//                     "QTabBar::tab:selected {"
//                         "border: none;"
//                         "background-color: #313131;"
//                     "}"
//                     "QTabBar::tab {"
//                         "background-color: transparent;"
//                     "}"
//                     "QTabWidget::tab-bar {"
//                         "background-color: #313131;"
//                     "}";

    setStyleSheet(strStyleSheet);
}

void CAdjustmentsPanel::onMainControllerAdjustmentsModelsUpdated(const CAdjustmentModelList &lstAdjustmentsModels)
{
//    DEBUG_OBJ_INFO

    setAdjustmentsModels(lstAdjustmentsModels);
}

void CAdjustmentsPanel::onAdjustmentSetSelected(CAdjustmentSet adjustmentSet)
{
//    DEBUG_OBJ_INFO
    mainController_.selectAdjustmentSet(adjustmentSet);
}

void CAdjustmentsPanel::onPathSelected(CGroup group, QString strPath, bool bOpenPath)
{
//    DEBUG_OBJ_INFO
    mainController_.selectPath(group, strPath, bOpenPath);
}

void CAdjustmentsPanel::onForwardToOpenPaths(CGroup group)
{
//    DEBUG_OBJ_INFO
    pTabWidget_->setCurrentIndex(2);
    pAdjustmentsTableOpenPaths_->showOpenPaths(group);
}

void CAdjustmentsPanel::onForwardToAdjustmenSets(CGroup group)
{
//    DEBUG_OBJ_INFO
    pTabWidget_->setCurrentIndex(1);
    pAdjustmentsTableSets_->showAdjustmentSets(group);
}

void CAdjustmentsPanel::updatePanel()
{
//    DEBUG_OBJ_INFO
    pAdjustmentsTableSets_->setAdjustmentsModels(lstAdjustmentsModels_);
    pAdjustmentsTableOpenPaths_->setAdjustmentsModels(lstAdjustmentsModels_);
    pAdjustmentsTableOverview_->setAdjustmentsModels(lstAdjustmentsModels_);
}
