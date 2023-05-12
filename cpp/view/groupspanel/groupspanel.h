#ifndef CGROUPSPANEL_H
#define CGROUPSPANEL_H

#include "panelwidget.h"
#include "applicationdata.h"

class CMainController;
class CRController;
class QTableWidget;
class QTableWidgetItem;
class CColorAndFontMap;
class CRefreshButton;

class CGroupsPanel : public CPanelWidget
{
    Q_OBJECT

public:
    explicit CGroupsPanel(QWidget *parent = nullptr);

protected:
    CMainController &mainController_;
    CRController &rController_;
    CColorAndFontMap& mapColorsAndFonts_;
    CGroupsList lstGroups_;
    QTableWidget* pTableGroups_;
    CRefreshButton *pRefreshButton_;

    void applyStyleSheet();
    void updatePanel();

//    void onPanelUpdated();
//    void onGroupsUpdated(const CGroupsList &lstGroups);
//    void onGroupAdded();

    void updateGroupSizes();
    bool updateGroupColors();

    void sendDataToMainController();

protected slots:
    void onMainControllerGroupsUpdated(const CGroupsList &lstGroups);

    void tableWidgetItemChanged(QTableWidgetItem *item);
    void onColorsAndFontsMapUpdated();
    void onRefreshButtonClicked();

    void onAddButtonClicked();
    void tableWidgetItemDoubleClicked(QTableWidgetItem *item);
};

#endif // CGROUPSPANEL_H
