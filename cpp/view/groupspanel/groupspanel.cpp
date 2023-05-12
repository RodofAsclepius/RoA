#include "groupspanel.h"
#include "panellabel.h"
#include "maincontroller.h"
#include "utils.h"
#include "groupspaneleditgroupdialog.h"
#include "groupslistdelegatecolor.h"
#include "groupslistdelegatesize.h"
#include "colorandfontmap.h"
#include "refreshbutton.h"
#include "toolbar.h"

#include <QGridLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QDebug>
#include <QCheckBox>

#define COLUMN_ENABLED 0
#define COLUMN_COLOR 1
#define COLUMN_NAME 2
#define COLUMN_SIZE 3
#define COLUMN_SIZE_BAR 4

#define COLUMN_COUNT 5
#define COLUMN_DATA COLUMN_NAME
//#define COLUMN_FILTERQUERY 5

#define MIN_SIZE 200

CGroupsPanel::CGroupsPanel(QWidget *parent)
    : CPanelWidget(parent),
      mainController_(CMainController::instance()),
      rController_(CRController::instance()),
      mapColorsAndFonts_(CColorAndFontMap::instance())
{
    applyStyleSheet();

    /* Label */
    panelLabel()->setText("Groups");

    /* Buttons */
    pRefreshButton_ = new CRefreshButton();
    toolBar()->addWidget(pRefreshButton_);
    connect(pRefreshButton_, &QPushButton::clicked, this, &CGroupsPanel::onRefreshButtonClicked);

    QSpacerItem *pSpacerItem = new QSpacerItem(10, 10, QSizePolicy::Expanding);
    toolBar()->addSpacer(pSpacerItem);

    QPushButton* pButtonAdd = new QPushButton();
    QPixmap pix(":/view/images/ui/add.png");
    QIcon icon(pix);
    pButtonAdd->setIcon(icon);
    pButtonAdd->setIconSize(QSize(16, 16));

    pButtonAdd->setFixedSize(24, 24);
    pButtonAdd->setStyleSheet("border: 1px solid gray;");
    pButtonAdd->setAttribute(Qt::WA_TranslucentBackground);

    toolBar()->addWidget(pButtonAdd);
    connect(pButtonAdd, &QPushButton::clicked, this, &CGroupsPanel::onAddButtonClicked);

    /* Table */
    pTableGroups_ = new QTableWidget();
    pTableGroups_->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    pTableGroups_->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
    pTableGroups_->setShowGrid(false);
//    pTableGroups_->setMinimumWidth(MIN_SIZE);

    QHeaderView *pHorizontalHeader = pTableGroups_->horizontalHeader();
//    pHorizontalHeader->setSizeAdjustPolicy(QHeaderView::SizeAdjustPolicy::AdjustToContents);
    pHorizontalHeader->setStretchLastSection(true);
    pHorizontalHeader->setVisible(false);
    QHeaderView *pVerticalHeader = pTableGroups_->verticalHeader();
    pVerticalHeader->setVisible(false);
    connect(pTableGroups_, &QTableWidget::itemDoubleClicked, this, &CGroupsPanel::tableWidgetItemDoubleClicked);
    gridLayout()->addWidget(pTableGroups_, 0, 0, 1, 1);
    pTableGroups_->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    /* Delegates */
    CGroupsListDelegateColor* pColorDelegate = new CGroupsListDelegateColor();
    pTableGroups_->setItemDelegateForColumn(COLUMN_COLOR, pColorDelegate);

    CGroupsListDelegateSize* pSizeDelegate = new CGroupsListDelegateSize();
    pTableGroups_->setItemDelegateForColumn(COLUMN_SIZE_BAR, pSizeDelegate);

    /* Connect controllers */
    connect(&mainController_, &CMainController::groupsUpdated, this, &CGroupsPanel::onMainControllerGroupsUpdated);
    connect(&mapColorsAndFonts_, &CColorAndFontMap::updated, this, &CGroupsPanel::onColorsAndFontsMapUpdated);
}

void CGroupsPanel::applyStyleSheet()
{
    QString strStyleSheet;

    strStyleSheet = styleSheet();
    strStyleSheet += "QTableWidget"
                     "{"
                        "border: 0px solid lightgray;"
                     "}"
                    "QCheckBox::indicator {"
                        "width: 18px;"
                        "height: 18px;"
                    "}"
                  "QTableView::indicator:checked"
                  "{"
                    "image: url(:/view/images/visible.png);"
                  "}"
                  "QTableView::indicator:unchecked"
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
                  "}";

    setStyleSheet(strStyleSheet);
}

void CGroupsPanel::updatePanel()
{
//    DEBUG_OBJ_INFO

    /* Update colors */
    bool bColorsNew = updateGroupColors();

    disconnect(pTableGroups_, &QTableWidget::itemChanged, this, &CGroupsPanel::tableWidgetItemChanged);

//    /* Tables groups */
    pTableGroups_->clear();
    pTableGroups_->setColumnCount(COLUMN_COUNT);
    pTableGroups_->horizontalHeader()->setStretchLastSection(true);
    pTableGroups_->setHorizontalHeaderLabels(QString("V;Col;Name;Size;Size bar").split(";"));

    pTableGroups_->setRowCount(lstGroups_.count());
    pTableGroups_->setSortingEnabled(false);
    int iRow = 0;

    foreach(CGroup group, lstGroups_)
    {
        /* Enabled checkbox */
        QTableWidgetItem* pItem = new QTableWidgetItem();
        if(group.enabled())
        {
            pItem->setCheckState(Qt::Checked);
        }
        else
        {
            pItem->setCheckState(Qt::Unchecked);
        }

        pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
        pTableGroups_->setItem(iRow, COLUMN_ENABLED, pItem);

        /* Color */
        QTableWidgetItem *pItemColor = new QTableWidgetItem("");
//        pItemColor->setBackgroundColor(group.colorPrimary());
        pItemColor->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
        pTableGroups_->setItem(iRow, COLUMN_COLOR, pItemColor);
        pItemColor->setData(Qt::UserRole, QVariant::fromValue(group));
        /* Name */
        QTableWidgetItem *pItemName = new QTableWidgetItem(group.name());
        pTableGroups_->setItem(iRow, COLUMN_NAME, pItemName);
        pItemName->setFlags(pItemName->flags() ^ Qt::ItemIsEditable);
        pItemName->setData(Qt::UserRole, QVariant::fromValue(group));

        QTableWidgetItem *pItemData = pTableGroups_->item(iRow, COLUMN_DATA);
        pItemData->setData(Qt::UserRole, QVariant::fromValue(group));

        ++iRow;
    }

    pTableGroups_->resizeColumnToContents(COLUMN_ENABLED);
    pTableGroups_->resizeColumnToContents(COLUMN_COLOR);
    pTableGroups_->resizeColumnToContents(COLUMN_NAME);
    pTableGroups_->resizeColumnToContents(COLUMN_SIZE);

    updateGroupSizes();

    pTableGroups_->setSortingEnabled(false);

    connect(pTableGroups_, &QTableWidget::itemChanged, this, &CGroupsPanel::tableWidgetItemChanged);

    if(bColorsNew)
    {
        sendDataToMainController();
    }
}

void CGroupsPanel::sendDataToMainController()
{
//    DEBUG_OBJ_INFO
    mainController_.setForceRecomputeBalanceModels(true);
    mainController_.setForceRecomputeAdjustmentModels(true);
    mainController_.setForceRecomputeEffectsModels(true);
    mainController_.setGroups(lstGroups_);
}

void CGroupsPanel::updateGroupSizes()
{
//    DEBUG_OBJ_INFO

    disconnect(pTableGroups_, &QTableWidget::itemChanged, this, &CGroupsPanel::tableWidgetItemChanged);

    for(int iRow = 0; iRow < pTableGroups_->rowCount(); ++iRow)
    {
        QTableWidgetItem* pItemFirstColumn = pTableGroups_->item(iRow, COLUMN_DATA);
        CGroup groupInTable = pItemFirstColumn->data(Qt::UserRole).value<CGroup>();
        CGroup groupSize = lstGroups_.byName(groupInTable.name());

        /* Size */
        QTableWidgetItem *pitemSize = new QTableWidgetItem(QString::number(groupSize.size()));
        pitemSize->setFlags(pitemSize->flags() ^ Qt::ItemIsEditable);
        pitemSize->setData(Qt::UserRole, QVariant::fromValue(groupSize));

        pTableGroups_->setItem(iRow, COLUMN_SIZE, pitemSize);
        pTableGroups_->resizeColumnToContents(COLUMN_SIZE);

        /* Size bar */
        QTableWidgetItem *pitemSizeBar = new QTableWidgetItem(QString::number(groupSize.size()));
        pitemSizeBar->setFlags(pitemSizeBar->flags() ^ Qt::ItemIsEditable);
        pitemSizeBar->setData(Qt::UserRole, QVariant::fromValue(groupSize));

        pTableGroups_->setItem(iRow, COLUMN_SIZE_BAR, pitemSizeBar);
        pTableGroups_->resizeColumnToContents(COLUMN_SIZE_BAR);
    }

    connect(pTableGroups_, &QTableWidget::itemChanged, this, &CGroupsPanel::tableWidgetItemChanged);
}

bool CGroupsPanel::updateGroupColors()
{
    QColor clrGroupPrimary;
    QColor clrGroupSecondary;

    CGroupsList lstGroupsUpdated;

    int iGroupIndex = 1;
    bool bNewColor = false;

    foreach(CGroup group, lstGroups_)
    {
        if(iGroupIndex >=1 && iGroupIndex <= 3)
        {
            clrGroupPrimary = mapColorsAndFonts_.getColor("groups/" + QString::number(iGroupIndex) + "/primary");
            clrGroupSecondary = mapColorsAndFonts_.getColor("groups/" + QString::number(iGroupIndex) + "/secondary");
        }
        else {
            clrGroupPrimary = mapColorsAndFonts_.randomColor();
            clrGroupSecondary = clrGroupPrimary.darker(150);
        }

        if(group.colorPrimary() != clrGroupPrimary)
        {
            group.setColorPrimary(clrGroupPrimary);
            bNewColor = true;
        }

        if(group.colorSecondary() != clrGroupSecondary)
        {
            group.setColorSecondary(clrGroupSecondary);
            bNewColor = true;
        }

        lstGroupsUpdated.append(group);

        ++iGroupIndex;
    }

    lstGroups_ = lstGroupsUpdated;

    return bNewColor;
}

void CGroupsPanel::onMainControllerGroupsUpdated(const CGroupsList &lstGroups)
{
//    DEBUG_OBJ_INFO
    lstGroups_ = lstGroups;
    updatePanel();
}

void CGroupsPanel::tableWidgetItemChanged(QTableWidgetItem *item)
{
//    DEBUG_OBJ_INFO

    /* Handle enable status */
    if(item->column() != COLUMN_ENABLED)
        return;

    QTableWidgetItem* pItemData = pTableGroups_->item(item->row(), COLUMN_DATA);
    if(!pItemData)
        return;

    QVariant varGroup = pItemData->data(Qt::UserRole);
    if(!varGroup.canConvert<CGroup>())
        return;

    CGroup group = varGroup.value<CGroup>();
//    qDebug() << "change group " << group;
    if(item->checkState() == Qt::Checked)
        group.setEnabled(true);
    if(item->checkState() == Qt::Unchecked)
        group.setEnabled(false);

//    qDebug() << "new changed group " << group;
    lstGroups_.replace(group);
//    qDebug() << lstGroups_ << group.enabled();

    updatePanel();
    pRefreshButton_->setNeedRefreshState();
}

void CGroupsPanel::onAddButtonClicked()
{
    CGroup groupNew;
    groupNew.setName(groupNew.name() + "_" + QString::number(groupNew.getID()));
    lstGroups_.append(groupNew);
    updatePanel();
}

void CGroupsPanel::tableWidgetItemDoubleClicked(QTableWidgetItem *item)
{
//    DEBUG_OBJ_INFO

    // Skip enable checkbox
    if(item->column() == COLUMN_ENABLED)
        return;

    QTableWidgetItem* pItemtData = pTableGroups_->item(item->row(), COLUMN_DATA);
    QVariant varGroup = pItemtData->data(Qt::UserRole);
    if(!varGroup.canConvert<CGroup>())
        return;

    CGroup groupForID = varGroup.value<CGroup>();
    CGroup group = lstGroups_.byID(groupForID.getID());

    CGroupsPanelEditGroupDialog dialog;
    dialog.setGroup(group);

    if(dialog.exec() == QDialog::Accepted)
    {
        group = dialog.group();

        lstGroups_.replace(group);
    }
    else
    {
        if(dialog.removeGroup())
        {
           lstGroups_.removeOne(group);
        }
    }

    updatePanel();
    pRefreshButton_->setNeedRefreshState();
}

void CGroupsPanel::onColorsAndFontsMapUpdated()
{
    /* Update colors of first three groups */
    //    for()
}

void CGroupsPanel::onRefreshButtonClicked()
{
    sendDataToMainController();
}
