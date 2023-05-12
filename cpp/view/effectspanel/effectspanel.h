#ifndef EFFECTSPANEL_H
#define EFFECTSPANEL_H

#include "panelwidget.h"
#include "applicationdata.h"
#include "maincontroller.h"

class CColorAndFontMap;
class CEffectsTable;
class CEffectsPanelMatrix;
class QComboBox;
class CRefreshButton;

class CEffectsPanel : public CPanelWidget
{
    Q_OBJECT
public:
    explicit CEffectsPanel(QWidget *parent = nullptr);
    ~CEffectsPanel() override;

    CVariablesList variables() const;
    void setVariables(const CVariablesList &variables);

    CEffect effect() const;
    void setEffect(const CEffect &AEffect);

    CEffectsModelsList effectsModels() const;
    void setEffectsModels(const CEffectsModelsList & lstEffectsModels);

    void updatePanel();

protected:
    CMainController& mainController_;
    CRController &rController_;
    CColorAndFontMap& mapColorsAndFont_;
    QComboBox* pComboBoxEffects_;
    CEffectsTable* pEffectsTable_;

    CVariablesList lstVariables_;
    CEffect effect_;
    CEffectsModelsList lstEffectsModels_;
    CRefreshButton *pRefreshButton_;

    void onEffectsPanelModelUpdated(const CEffectsModel &effectsModel);
    void applyStyleSheet();

    void determineGlobalMinAndMaxValues();
    void computeEffectModels();
public slots:
    void onMainControllerVariablesUpdated(const CVariablesList& lstVariables);
    void onMainControllerEffectUpdated(const CEffect &effect);
    void onMainControllerEffectsModelsComputed(const CEffectsModelsList &effectsModels);
    void onMainControllerEffectsModelNeedUpdate();
    void onMainControllerEffectsModelAutoUpdate();

    /* UI */
    void onComboBoxEffectIndexChanged(int ICurrentIndex);
    void onRefreshButtonClicked();

};

#endif // EFFECTSPANEL_H
