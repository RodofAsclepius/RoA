#include "adjustmentstable.h"
#include "colorandfontmap.h"
#include <QHeaderView>
#include <QMessageBox>

#include "utils.h"

#define COLUMN_GROUP 1
#define COLUMN_ICON 0
#define COLUMN_OPENPATHS 2
#define COLUMN_ADJUSTMENTSETS 3
#define COLUMN_COUNT 1


CAdjustmentsTable::CAdjustmentsTable(QWidget *pParent)
    : QTreeWidget(pParent),
      mainController_(CMainController::instance()),
      mapColorsAndFont_(CColorAndFontMap::instance())
{
    applyStyleSheet();

    header()->hide();
    header()->setStretchLastSection(true);

    setColumnCount(COLUMN_COUNT);
    setHeaderLabels(QString("Group").split(";"));

    setFocusPolicy(Qt::NoFocus);
    setSelectionMode(QTreeWidget::SelectionMode::NoSelection);
    setSelectionBehavior(QTreeWidget::SelectionBehavior::SelectItems);

    connect(this, &CAdjustmentsTable::itemDoubleClicked, this, &CAdjustmentsTable::onItemDoubleClicked);
    connect(this, &CAdjustmentsTable::itemEntered, this, &CAdjustmentsTable::onItemEntered);

    setMouseTracking(true);
}

void CAdjustmentsTable::updateWidget()
{
    clear();

    switch(iShowMode_)
    {
    case Overview:
    {
        populateOverview();
    }
    break;
    case AdjustmenSets:
    {
        populateAdjustmentSets();
    }
    break;
    case OpenPaths:
    {
        populateOpenPaths();
    }
    break;
    }

//    expandToDepth(0);
}

void CAdjustmentsTable::populateOverview()
{
    setColumnCount(4);
    setHeaderLabels(QString("Status;Group;Open paths; Adj. sets").split(";"));
    header()->show();

    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

    foreach(CAdjustmentModel adjustmentsModel, lstAdjustmentsModels_)
    {
        CCausalModel causalModel = adjustmentsModel.causalModel();
        CGroup group = causalModel.group();

        /* Find correct item in tree */
        QList<QTreeWidgetItem *> lstItems = findItems(group.name(), Qt::MatchExactly, 0);
        QTreeWidgetItem *pGroupItem;
        if(lstItems.isEmpty())
        {
            pGroupItem = addGroupItem(group, COLUMN_GROUP);
        }
        else
        {
            pGroupItem = lstItems.first();
        }

        /* Add all open paths */
        int iOpenPaths = adjustmentsModel.openPaths().count();
        int iAdjustmentSets = adjustmentsModel.adjustmentSets().count();

        pGroupItem->setText(COLUMN_GROUP, group.name());
        pGroupItem->setForeground(COLUMN_GROUP, group.colorPrimary());
        pGroupItem->setData(COLUMN_GROUP, Qt::UserRole, QVariant::fromValue(group));
        pGroupItem->setFont(COLUMN_GROUP, fontGroups_);

        pGroupItem->setText(COLUMN_ICON, "   ");
        pGroupItem->setText(COLUMN_OPENPATHS, QString::number(iOpenPaths));
        pGroupItem->setForeground(COLUMN_OPENPATHS, group.colorPrimary());

        bool bAdjustmentSetsAreEmpty = true;
        foreach(CAdjustmentSet adjustmentSet, adjustmentsModel.adjustmentSets())
        {
            if(!adjustmentSet.nodesNames().isEmpty())
            {
                bAdjustmentSetsAreEmpty = false;

            }
        }

        if(iAdjustmentSets == 1 && bAdjustmentSetsAreEmpty)
        {
            iAdjustmentSets = 0;
        }

        pGroupItem->setText(COLUMN_ADJUSTMENTSETS, QString::number(iAdjustmentSets));
        pGroupItem->setForeground(COLUMN_ADJUSTMENTSETS, group.colorPrimary());

        /* Present icon? */
        pGroupItem->setIcon(COLUMN_ICON, QIcon());

        /* Open paths and no adjustments */
        if((iOpenPaths > 0) && iAdjustmentSets < 1)
        {
            QIcon icon(":/view/images/alert_red.png");
            pGroupItem->setIcon(COLUMN_ICON, icon);
        }
        /* Adjustments possible */
        if(iAdjustmentSets > 0)
        {
            QIcon icon(":/view/images/alert_yellow.png");
            pGroupItem->setIcon(COLUMN_ICON, icon);
        }
        /* Open paths and no adjustments */
        if((iOpenPaths < 1))
        {
            QIcon icon(":/view/images/alert_green.png");
            pGroupItem->setIcon(COLUMN_ICON, icon);
        }
    }

//    adjustSize();
//    header()->adjustSize();
    header()->resizeSection(COLUMN_ICON, 60);
}

void CAdjustmentsTable::populateAdjustmentSets()
{
    setColumnCount(1);
    setHeaderLabels(QString("Group").split(";"));

    foreach(CAdjustmentModel adjustmentsModel, lstAdjustmentsModels_)
    {
        CCausalModel causalModel = adjustmentsModel.causalModel();
        CGroup group = causalModel.group();

        /* Find correct item in tree */
        QList<QTreeWidgetItem *> lstItems = findItems(group.name(), Qt::MatchExactly, 0);
        QTreeWidgetItem *pGroupItem;
        if(lstItems.isEmpty())
        {
            pGroupItem = addGroupItem(group);
        }
        else
        {
            pGroupItem = lstItems.first();
        }

        /* Add all adjustment sets */
        foreach(CAdjustmentSet adjustmentSet, adjustmentsModel.adjustmentSets())
        {
            QTreeWidgetItem *pItemAdjustmentSet = new QTreeWidgetItem();
            pGroupItem->addChild(pItemAdjustmentSet);

            QString strAdjustmentSet = "{" + adjustmentSet.nodesNames().join(", ") + "}";
            pItemAdjustmentSet->setText(0, strAdjustmentSet);
            pItemAdjustmentSet->setForeground(0, group.colorPrimary());
            pItemAdjustmentSet->setData(COLUMN_GROUP, Qt::UserRole, QVariant::fromValue(adjustmentSet));
            pItemAdjustmentSet->setFont(0, fontItems_);
        }
    }
}

void CAdjustmentsTable::populateOpenPaths()
{
    setColumnCount(1);
    setHeaderLabels(QString("Group").split(";"));

    foreach(CAdjustmentModel adjustmentsModel, lstAdjustmentsModels_)
    {
        CCausalModel causalModel = adjustmentsModel.causalModel();
        CGroup group = causalModel.group();

        /* Find correct item in tree */
        QList<QTreeWidgetItem *> lstItems = findItems(group.name(), Qt::MatchExactly, 0);
        QTreeWidgetItem *pGroupItem;
        if(lstItems.isEmpty())
        {
            pGroupItem = addGroupItem(group);
        }
        else
        {
            pGroupItem = lstItems.first();
        }

        /* Add all open paths */
        QStringList lstPaths = adjustmentsModel.paths();
        QVariantList lstOpenPaths = adjustmentsModel.openPaths();

        for(int iIndex=0; iIndex<lstPaths.count(); ++iIndex)
        {
            QString strPath = lstPaths.at(iIndex);
            bool bPathIsOpen = lstOpenPaths.at(iIndex).toBool();

            QTreeWidgetItem *pItemPath= new QTreeWidgetItem();
            pItemPath->setText(0, strPath);
            pItemPath->setForeground(0, group.colorPrimary());
            pItemPath->setData(COLUMN_GROUP, Qt::UserRole, QVariant::fromValue(strPath));
            pItemPath->setFont(0, fontItems_);

            /* Open? */
            if(bPathIsOpen)
            {
                pGroupItem->addChild(pItemPath);
            }
        }
    }
}

CAdjustmentModelList CAdjustmentsTable::adjustmentsModels() const
{
    return lstAdjustmentsModels_;
}

void CAdjustmentsTable::setAdjustmentsModels(const CAdjustmentModelList &lstAdjustmentsModels)
{
    lstAdjustmentsModels_ = lstAdjustmentsModels;
    updateWidget();
}

void CAdjustmentsTable::applyStyleSheet()
{
    QString strStyleSheet = styleSheet();
    
    QColor altRowColor = palette().alternateBase().color();
//    strStyleSheet += "QTreeWidget {"
//                        "background-color: " + mapColorsAndFont_.getColor("widget/chart").name() + ";"
//                     "}"
//                    "QHeaderView::section {"
//                        "background-color: " + mapColorsAndFont_.getColor("widget/generic").name() + ";" //#525252;"
//                        "border: none;"
//                    "}"
//                    "QTreeView::item {  border: 0px;  padding: 0px 6px; }"
//                    ;

    setStyleSheet(strStyleSheet);

    fontGroups_ = font();
    fontGroups_.setBold(true);
    fontGroups_.setPixelSize(18);

    fontItems_ = font();
    fontItems_.setPixelSize(16);
}

QTreeWidgetItem *CAdjustmentsTable::addGroupItem(CGroup &group, int iColumn)
{
    QTreeWidgetItem *pGroupItem = new QTreeWidgetItem();
    pGroupItem->setText(iColumn, group.name());
    pGroupItem->setForeground(iColumn, group.colorPrimary());
    pGroupItem->setData(iColumn, Qt::UserRole, QVariant::fromValue(group));
    pGroupItem->setFont(iColumn, fontGroups_);
    addTopLevelItem(pGroupItem);

    return pGroupItem;
}

void CAdjustmentsTable::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
//    DEBUG_OBJ_INFO

//    if(item->parent() != nullptr)
//    {
        switch(iShowMode_)
        {
        case Overview:
        {
            if(column == COLUMN_OPENPATHS)
            {
                QVariant varData = item->data(COLUMN_GROUP, Qt::UserRole);
                CGroup group = varData.value<CGroup>();
                emit forwardToOpenPaths(group);
            }
            if(column == COLUMN_ADJUSTMENTSETS)
            {
                QVariant varData = item->data(COLUMN_GROUP, Qt::UserRole);
                CGroup group = varData.value<CGroup>();
                emit forwardToAdjustmentSets(group);
            }
        }
        break;
        case AdjustmenSets:
        {
            QVariant varData = item->data(COLUMN_GROUP, Qt::UserRole);
            CAdjustmentSet set = varData.value<CAdjustmentSet>();
            emit adjustmentSetSelected(set);
        }
        break;
        case OpenPaths:
        {
            QVariant varData = item->data(COLUMN_GROUP, Qt::UserRole);
            QString strPath = varData.value<QString>();

            QVariant varParentData = item->parent()->data(COLUMN_GROUP, Qt::UserRole);
            CGroup group = varParentData.value<CGroup>();

            emit pathSelected(group, strPath, true);
        }
        break;
        }
//    }
}

void CAdjustmentsTable::onItemEntered(QTreeWidgetItem *item, int column)
{
    setCursor(Qt::ArrowCursor);

    switch(iShowMode_)
    {
    case Overview:
    {
        if(column == COLUMN_OPENPATHS || column == COLUMN_ADJUSTMENTSETS)
        {
            setCursor(Qt::PointingHandCursor);
        }
    }
    break;
    case AdjustmenSets:
    {
        if(item->parent() != nullptr)
        {
            setCursor(Qt::PointingHandCursor);
        }
    }
    break;
    case OpenPaths:
    {
        if(item->parent() != nullptr)
        {
            setCursor(Qt::PointingHandCursor);
        }
    }
    break;
    }
}

void CAdjustmentsTable::showOpenPaths(CGroup group)
{
    if(iShowMode_ == OpenPaths)
    {
        collapseAll();

        QList<QTreeWidgetItem*> clist = findItems(group.name(), Qt::MatchExactly, 0);
        foreach(QTreeWidgetItem* item, clist)
        {
            item->setExpanded(true);
            setCurrentItem(item);
        }
    }
}

void CAdjustmentsTable::showAdjustmentSets(CGroup group)
{
    if(iShowMode_ == AdjustmenSets)
    {
        collapseAll();

        QList<QTreeWidgetItem*> clist = findItems(group.name(), Qt::MatchExactly, 0);
        foreach(QTreeWidgetItem* item, clist)
        {
            item->setExpanded(true);
            setCurrentItem(item);
        }
    }
}
