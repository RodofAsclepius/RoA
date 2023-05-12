#ifndef CADJUSTMENTSTABLE_H
#define CADJUSTMENTSTABLE_H

#include <QTreeWidget>
#include "adjustmentmodellist.h"
#include "maincontroller.h"

class CColorAndFontMap;

class CAdjustmentsTable : public QTreeWidget
{
    Q_OBJECT
public:
    CAdjustmentsTable(QWidget *pParent = nullptr);

    CAdjustmentModelList adjustmentsModels() const;
    void setAdjustmentsModels(const CAdjustmentModelList &adjustmentsModels);

    void updateWidget();
    void populateAdjustmentSets();
    void populateOpenPaths();
    void populateOverview();

    typedef enum
    {
            Overview,
            AdjustmenSets,
            OpenPaths,
    } ShowMode;

    void setShowMode(ShowMode mode) { iShowMode_ = mode; }
    void showOpenPaths(CGroup group);
    void showAdjustmentSets(CGroup group);

protected:
    CMainController &mainController_;
    CColorAndFontMap& mapColorsAndFont_;
    CAdjustmentModelList lstAdjustmentsModels_;

    void applyStyleSheet();
    QTreeWidgetItem *addGroupItem(CGroup &group, int iColumn=0);

    void onItemDoubleClicked(QTreeWidgetItem * item, int column);
    void onItemEntered(QTreeWidgetItem *item, int column);

    int iShowMode_ = ShowMode::AdjustmenSets;
    bool bShowAllGroups_ = false;
    QFont fontGroups_;
    QFont fontItems_;

signals:
    void adjustmentSetSelected(CAdjustmentSet set);
    void pathSelected(CGroup group, QString strPath, bool bOpen);
    void forwardToOpenPaths(CGroup group);
    void forwardToAdjustmentSets(CGroup group);

};

#endif // CADJUSTMENTSTABLE_H

