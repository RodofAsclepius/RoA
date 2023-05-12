#include "treatmentpanel.h"
#include "panellabel.h"
#include "maincontroller.h"
#include "utils.h"
#include "distributionview.h"
#include "toolbar.h"

#include <QGridLayout>
#include <QListWidget>
#include <QLineEdit>
#include <QSpacerItem>
#include <QComboBox>
#include <QRadioButton>
#include <QButtonGroup>

#define COMBO_WIDTH 160

CTreatmentPanel::CTreatmentPanel(QWidget *parent)
    : CPanelWidget(parent),
    mainController_(CMainController::instance()),
    rController_(CRController::instance()),
    pDistributionView(new CDistributionView())
{
    applyStyleSheet();

    /* Label */
    panelLabel()->setText("Cause");

    /* Combobox treatment */
    pComboBoxTreatment_ = new QComboBox();
    QSpacerItem *pSpacerItem = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Fixed);
    toolBar()->addSpacer(pSpacerItem);
    toolBar()->addWidget(pComboBoxTreatment_);
    pComboBoxTreatment_->setFixedWidth(COMBO_WIDTH);
//    toolBar()->addSpacer();

    /* Histogram */
    int iHorMarCharts = 48;
    pDistributionView->setViewMode(CDistributionView::Histogram);
    pDistributionView->setOrientationMode(CDistributionView::North);
    pDistributionView->setDrawAxes(true, false);
    pDistributionView->setAlternatePrimarySecondaryGroupColorsOverLevels(true);
    pDistributionView->setBarWidthHistogram(24);
    pDistributionView->setBarSpacingHistogram(2);
    pDistributionView->setLevelSpacingHistogram(32);
    pDistributionView->setMarginsPlot(QMargins(iHorMarCharts, 24, iHorMarCharts, 24));
    pDistributionView->setMarginsLabels(QMargins(6, 0, 6, 0));
    pDistributionView->setAxesWidth(1);
    pDistributionView->setUseGroupColorsForLables(false);

    gridLayout()->addWidget(pDistributionView, 0, 0, 1, 1);
    gridLayout()->setRowStretch(0, 5);

    /* Estimand */
    QGridLayout* pRadioLayout = new QGridLayout();
    pRadioLayout->setContentsMargins(6, 0, 0, 0);
    gridLayout()->addLayout(pRadioLayout, 1, 0, 1, 1);

    pRadioLayout->addWidget(new QLabel("Estimand:"), 0, 0, 1, 1);
    pRadioEstimandATE_ = new QRadioButton("ATE");
    pRadioEstimandATE_->setChecked(false);
    pRadioEstimandATT_ = new QRadioButton("ATT");
    pRadioEstimandATT_->setChecked(true);
    pRadioLayout->addWidget(pRadioEstimandATE_, 0, 1, 1, 1);
    pRadioLayout->addWidget(pRadioEstimandATT_, 0, 2, 1, 1);

    connect(pRadioEstimandATE_, &QRadioButton::clicked, this, &CTreatmentPanel::onRadioButtonMethodClicked);
    connect(pRadioEstimandATT_, &QRadioButton::clicked, this, &CTreatmentPanel::onRadioButtonMethodClicked);

    /* Connect maincontroller */
    connect(&mainController_, &CMainController::groupsUpdated, this, &CTreatmentPanel::onMainControllerGroupsUpdated);
    connect(&mainController_, &CMainController::variablesUpdated, this, &CTreatmentPanel::onMainControllerVariablesUpdated);
    connect(&mainController_, &CMainController::treatmentUpdated, this, &CTreatmentPanel::onMainControllerTreatmentUpdated);
    connect(&rController_, &CRController::distributionsComputed, this, &CTreatmentPanel::onRControllerDistributionsComputed);
}

CTreatment CTreatmentPanel::treatment() const
{
    return treatment_;
}

void CTreatmentPanel::setTreatment(const CTreatment &treatment)
{
    treatment_ = treatment;
}

CVariablesList CTreatmentPanel::variables() const
{
    return lstVariables_;
}

void CTreatmentPanel::setVariables(const CVariablesList &lstVariables)
{
    lstVariables_ = lstVariables;
}

CGroupsList CTreatmentPanel::groups() const
{
    return lstGroups_;
}

void CTreatmentPanel::setGroups(const CGroupsList &lstGroups)
{
    lstGroups_ = lstGroups;
}

void CTreatmentPanel::updatePanel()
{
//    DEBUG_OBJ_INFO
    disconnect(pComboBoxTreatment_, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxTreatmentIndexChanged(int)));
    pComboBoxTreatment_->clear();

    pComboBoxTreatment_->addItem("", QVariant());
    foreach(CVariable variable, lstVariables_)
    {
        if(variable.type() == "factor" && variable.levels().count() == 2)
        {
            pComboBoxTreatment_->addItem(variable.name(), QVariant::fromValue(variable));
        }
    }

    if(treatment_.isValid())
    {
        QString strTreatmentVariableName = treatment().variable().name();

        int iIndex = pComboBoxTreatment_->findText(strTreatmentVariableName);
        if(iIndex > -1)
        {
            pComboBoxTreatment_->setCurrentIndex(iIndex);

        }

        lstGroups_ = mainController_.applicationData().groups().groupsEnabled();
        /* Compute distributions */
        CDistributionsList lstDistributions;
        foreach(CGroup group, lstGroups_)
        {
            if(group.enabled())
            {
                CDistribution distribution(group, treatment_.variable());
                lstDistributions.append(distribution);
            }
        }

//        qDebug() << lstDistributions;  // Fix display
        rController_.computeDistributions(lstDistributions);
    }


    if(pComboBoxTreatment_ && pComboBoxTreatment_->lineEdit())
        pComboBoxTreatment_->lineEdit()->setAlignment(Qt::AlignHCenter);

    /* Estimation method */
    if(treatment_.estimationMethod() == "ATE")
    {
        pRadioEstimandATE_->setChecked(true);
        pRadioEstimandATT_->setChecked(false);
    }
    if(treatment_.estimationMethod() == "ATT")
    {
        pRadioEstimandATE_->setChecked(false);
        pRadioEstimandATT_->setChecked(true);
    }

//    connect(pRadioEstimandATE_, &QRadioButton::clicked, this, &CTreatmentPanel::onRadioButtonMethodClicked);
//    connect(pRadioEstimandATT_, &QRadioButton::clicked, this, &CTreatmentPanel::onRadioButtonMethodClicked);
    connect(pComboBoxTreatment_, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxTreatmentIndexChanged(int)));

    pComboBoxTreatment_->setFixedWidth(COMBO_WIDTH);
}

void CTreatmentPanel::applyStyleSheet()
{
    QString strStyleSheet = styleSheet();

//    strStyleSheet += "QWidget"
//                     "{"
//                        "border: none;"
//                     "};";
//    setStyleSheet(strStyleSheet);


//    strStyleSheet += "QComboBox"
//                     "{"
//                     "border: none"
//                     "width: 100px;"
//                         "height: 100px;"
//                     "}";

    setStyleSheet(strStyleSheet);

    QString strStyleSheetDistributionView = pDistributionView->styleSheet();
    strStyleSheetDistributionView += "QWidget { border: none;}";
    pDistributionView->setStyleSheet(strStyleSheetDistributionView);
}

void CTreatmentPanel::onMainControllerVariablesUpdated(const CVariablesList &lstVariables)
{
    setVariables(lstVariables);
    updatePanel();
}

void CTreatmentPanel::onMainControllerGroupsUpdated(const CGroupsList &lstGroups)
{
    setGroups(lstGroups);
    updatePanel();
}

void CTreatmentPanel::onMainControllerTreatmentUpdated(const CTreatment &aTreatment)
{
    treatment_ = aTreatment;
    updatePanel();
}

void CTreatmentPanel::onRControllerDistributionsComputed(const CDistributionsList &lstDistributions)
{
//    DEBUG_OBJ_INFO

//    qDebug() << lstDistributions;

    if(pComboBoxTreatment_->currentIndex() != -1)
    {
        CDistributionsList lstDistributionsFiltered;
        foreach(CDistribution distribution, lstDistributions)
        {
            if(distribution.variable().name() == treatment_.variable().name())
            {
                lstDistributionsFiltered.append(distribution);
            }
        }
        pDistributionView->setTreatmentLevel(treatment_.levelTreatment());
        pDistributionView->setDistributions(lstDistributionsFiltered);
    }
}

void CTreatmentPanel::onComboBoxTreatmentIndexChanged(int ICurrentIndex)
{
    pComboBoxTreatment_->setFixedWidth(COMBO_WIDTH);
//    DEBUG_OBJ_INFO
    if(pComboBoxTreatment_->currentText().isEmpty())
    {
        CTreatment treatment;
        mainController_.setTreatment(treatment);
        return;
    }

    QVariant varData = pComboBoxTreatment_->itemData(ICurrentIndex, Qt::UserRole);
    if(varData.canConvert<CVariable>())
    {
        CVariable variable = varData.value<CVariable>();
        CTreatment treatment;
        treatment.setVariable(variable);
        mainController_.setTreatment(treatment);
    }
}

void CTreatmentPanel::onRadioButtonMethodClicked()
{
//    DEBUG_OBJ_INFO

    if(pRadioEstimandATE_->isChecked())
    {
        treatment_.setEstimationMethod("ATE");
    }
    if(pRadioEstimandATT_->isChecked())
    {
        treatment_.setEstimationMethod("ATT");
    }
    mainController_.setTreatment(treatment_);
}

