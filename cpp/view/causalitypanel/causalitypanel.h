#ifndef CCAUSALITYSPANEL_H
#define CCAUSALITYSPANEL_H

#include "panelwidget.h"
#include "applicationdata.h"
#include "markovmodellist.h"

class QLabel;
class CMainController;
class CRController;
class QComboBox;
class QRadioButton;
class CCausalityPanelGraphViewLegend;
class CRefreshButton;
class CSplitMergeButton;
class QSpinBox;
class QCheckBox;
class QPushButton;
class CCausalityPanelTable;
class CCausalityPanelGraphView;
class CCausalityPanelGraphNode;
class CAdjustmentSetList;
class CCausalityPanelGraphView;

class CCausalityPanel : public CPanelWidget
{
    Q_OBJECT

public:
    explicit CCausalityPanel(QWidget *parent = nullptr);
    void clearPanel();

    CCausalityPanelTable *tableBackdoorPaths() const;

    CCausalModelList causalModels() const;
    void setCausalModels(const CCausalModelList &causalModels);

protected:
    CMainController &mainController_;
    CRController &rController_;
    CCausalityPanelGraphView* pGraphView_;
    CCausalityPanelGraphViewLegend *pLegend_;

    /* Data */
    CCausalModelList lstCausalModels_;
    CCausalModelCacheList lstCausalModelCaches_;
    CMarkovModelList lstMarkovModels_;

    /* UI */
    CRefreshButton *pRefreshButton_;
    QComboBox *pComboBoxGroup_;
    QComboBox *pComboBoxGroupComparison_;

    QRadioButton *pRadioShowFullGraph_;
    QRadioButton *pRadioShowMarkovBlanket_;
    QSpinBox *pSpinMarkovBlanketDistance_;
    QCheckBox *pCheckShowOutliers_;

    QPushButton *pButtonAutoLayout_;
    QPushButton *pButtonMine_;
    QPushButton *pButtonRemoveEdge_;
    QPushButton *pButtonViewOptions_;

    bool bShowUnconnectedNodes_ = false;
    bool bShowMarkovBlanketPlus_ = false;
    int iMarkovBlanketPlusDistance_ = 5;


    void updateLegend();
    void updateLegendPosition();

    void updatePanel();
    void updateCombos();
    void manageCombos();
    void updateCausalModels();
    void recomputeMarkovModels();
    void selectPath(const CGroup &group, const QString &strPath, bool bOpenPath);
    void setMarkovModels(const CMarkovModelList &lstMarkovModels);
//    void applyMarkovBlanketOptions();
//    void applyOutlierOptions();

    void applyViewSettings();

    void applyStyleSheet();
    void mineCausalModel(const CCausalModel &causalModel, qreal rAlpha = 0.1);
    void sortGraph();
    void addNode();
    void addEdge();
    void removeNode();
    void removeEdge();
    void resizeEvent(QResizeEvent *event) override;

protected slots:
    void onMainControllerCausalModelsUpdated(const CCausalModelList &lstCausalModels);
    void onRefreshButtonClicked();
    void onPathSelected(CGroup group, QString strPath, bool bOpenPath);
    void onViewCausalModelUpdated(const CCausalModel &causalModel);
    void onRControllerCausalModelsUpdated(const CCausalModelList &lstCausalModels);
    void onRControllerMarkovModelsComputed(const CMarkovModelList &lstMarkovModels);
    void onComboBoxGroupIndexChanged(int);
    void onComboBoxGroupComparisonIndexChanged(int);
    void onPushButtonSortLayoutClicked();
    void onPushButtonMineClicked();
    void onPushButtonRemoveEdgeClicked();
    void onPushButtonViewOptionsClicked();
    void onDataCheckCompleted();
};

#endif // CCAUSALITYSPANEL_H


