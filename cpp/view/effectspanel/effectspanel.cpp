#include "effectspanel.h"
#include "utils.h"

#include "panellabel.h"
#include "maincontroller.h"
#include "effectstable.h"
#include "effectspanelmatrix.h"
#include "colorandfontmap.h"
#include "rcontroller.h"
#include "refreshbutton.h"

#include <QGridLayout>
#include <QComboBox>
#include <QSplitter>
#include <QLineEdit>
#include <QHeaderView>

#define COMBOWIDTH 200

using namespace utils;

CEffectsPanel::CEffectsPanel(QWidget *parent)
    : CPanelWidget(parent),
      mainController_(CMainController::instance()),
      rController_(CRController::instance()),
      mapColorsAndFont_(CColorAndFontMap::instance()),
      pEffectsTable_(new CEffectsTable()),
      pRefreshButton_(new CRefreshButton())
{
    applyStyleSheet();

    /* Label */
    panelLabel()->setText("Effect");

    pComboBoxEffects_ = new QComboBox();
    pComboBoxEffects_->setMinimumWidth(COMBOWIDTH);

    QSpacerItem* pSpacerItem2 = new QSpacerItem(20, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);
//    toolBar()->addSpacer(pSpacerItem);
//    toolBar()->addWidget(pRefreshButton_);
    toolBar()->addSpacer(pSpacerItem2);
    toolBar()->addWidget(pComboBoxEffects_);
    QSpacerItem* pSpacerItem3 = new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    toolBar()->addSpacer(pSpacerItem3);

    connect(pRefreshButton_, &QPushButton::clicked, this, &CEffectsPanel::onRefreshButtonClicked);

    gridLayout()->addWidget(pEffectsTable_, 0, 0, 1, 1);
    pEffectsTable_->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);

    /* Connect maincontroller */
    connect(&mainController_, &CMainController::variablesUpdated, this, &CEffectsPanel::onMainControllerVariablesUpdated);
    connect(&mainController_, &CMainController::effectUpdated, this, &CEffectsPanel::onMainControllerEffectUpdated);
    connect(&mainController_, &CMainController::effectsModelsUpdated, this, &CEffectsPanel::onMainControllerEffectsModelsComputed);
    connect(&mainController_, &CMainController::effectModelsNeedUpdate, this, &CEffectsPanel::onMainControllerEffectsModelNeedUpdate);
    connect(&mainController_, &CMainController::effectModelsAutoUpdate, this, &CEffectsPanel::onMainControllerEffectsModelAutoUpdate);

    connect(pComboBoxEffects_, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxEffectIndexChanged(int)));
}

CEffectsPanel::~CEffectsPanel()
{

}

void CEffectsPanel::updatePanel()
{
//    DEBUG_OBJ_INFO

    disconnect(pComboBoxEffects_, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxEffectIndexChanged(int)));
    pComboBoxEffects_->clear();

    pComboBoxEffects_->addItem("", QVariant());
    foreach(CVariable variable, lstVariables_)
    {
        if(variable.type() != "factor")
        {
            pComboBoxEffects_->addItem(variable.name(), QVariant::fromValue(variable));
        }
    }
    pComboBoxEffects_->adjustSize();

    if(effect_.isValid())
    {
        QString strTreatmentVariableName = effect_.variable().name();

        int iIndex = pComboBoxEffects_->findText(strTreatmentVariableName);
        if(iIndex > -1)
        {
            pComboBoxEffects_->setCurrentIndex(iIndex);

        }
    }

    connect(pComboBoxEffects_, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxEffectIndexChanged(int)));

    /* Table effect estimates */
    pEffectsTable_->setEffectModels(lstEffectsModels_);
    pEffectsTable_->updateTable();

    pComboBoxEffects_->setFixedWidth(COMBOWIDTH);
}

CEffect CEffectsPanel::effect() const
{
    return effect_;
}

void CEffectsPanel::setEffect(const CEffect &effect)
{
    effect_ = effect;
}

CVariablesList CEffectsPanel::variables() const
{
    return lstVariables_;
}

void CEffectsPanel::setVariables(const CVariablesList &lstVariables)
{
    lstVariables_ = lstVariables;
}

CEffectsModelsList CEffectsPanel::effectsModels() const
{
    return lstEffectsModels_;
}

void CEffectsPanel::setEffectsModels(const CEffectsModelsList &lstEffectsModels)
{
    lstEffectsModels_ = lstEffectsModels;
}

void CEffectsPanel::applyStyleSheet()
{
    QString strStyleSheet;

    strStyleSheet = styleSheet();
//    strStyleSheet += "QComboBox"
//                     "{"
//                     "border: none"
//                     "width: 100px;"
//                         "height: 100px;"
//                     "}";

    setStyleSheet(strStyleSheet);
}

void CEffectsPanel::determineGlobalMinAndMaxValues()
{

}

void CEffectsPanel::computeEffectModels()
{
    CApplicationData appData = mainController_.applicationData();
    CEffectsModelsList lstEffectsModelsNew;
    CEffect effect = appData.effect();
    if(effect.isValid())
    {
        foreach(CBalanceModel balanceModel2, appData.balanceModels())
        {
           CEffectsModel effectsModel;
           effectsModel.setEffect(effect);
           effectsModel.setBalanceModel(balanceModel2);
           lstEffectsModelsNew.append(effectsModel);
        }
        rController_.computeEffectsModels(lstEffectsModelsNew);
    }
}

void CEffectsPanel::onMainControllerVariablesUpdated(const CVariablesList & lstVariables)
{
//    DEBUG_OBJ_INFO

    setVariables(lstVariables);
    updatePanel();
}

void CEffectsPanel::onMainControllerEffectUpdated(const CEffect &AEffect)
{
//    DEBUG_OBJ_INFO

    setEffect(AEffect);
    updatePanel();
}

void CEffectsPanel::onMainControllerEffectsModelsComputed(const CEffectsModelsList &lstEffectsModels)
{
//    DEBUG_OBJ_INFO

    setEffectsModels(lstEffectsModels);
    updatePanel();
}

void CEffectsPanel::onMainControllerEffectsModelNeedUpdate()
{
    pRefreshButton_->setNeedRefreshState(true);
}

void CEffectsPanel::onMainControllerEffectsModelAutoUpdate()
{
    pRefreshButton_->setNeedRefreshState(false);
}

void CEffectsPanel::onEffectsPanelModelUpdated(const CEffectsModel &effectsModel)
{
//    DEBUG_OBJ_INFO
}

void CEffectsPanel::onComboBoxEffectIndexChanged(int ICurrentIndex)
{
//    DEBUG_OBJ_INFO
    if(pComboBoxEffects_->currentText().isEmpty())
    {
        CEffect effect;
        mainController_.setEffect(effect);
        return;
    }

    QVariant varData = pComboBoxEffects_->itemData(ICurrentIndex, Qt::UserRole);
    if(varData.canConvert<CVariable>())
    {
        CVariable variable = varData.value<CVariable>();
        CEffect effect;
        effect.setVariable(variable);
        effect.setIsValid(true);
        mainController_.setEffect(effect);
    }
}

void CEffectsPanel::onRefreshButtonClicked()
{
//    DEBUG_OBJ_INFO

    computeEffectModels();
}
