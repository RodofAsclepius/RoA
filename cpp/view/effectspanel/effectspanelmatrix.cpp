#include "maincontroller.h"
#include "utils.h"
#include "effectspanelmatrix.h"
#include "effectsmatrixheader.h"
#include "effectspanelmatrixdelegate.h"
#include "colorandfontmap.h"

#include <QHeaderView>
#include <QScrollBar>
#include <QApplication>
#include <QRandomGenerator>

#define ROWS_VISIBLE_MIN 8
#define COLUMNS_VISIBLE_MIN 8
#define NUMBER_OF_AGGREGATES 4
#define HEADER_SIZE 75

QSize CEffectsPanelMatrix::_cellSize = QSize(20, 20);

CEffectsPanelMatrix::CEffectsPanelMatrix(CEffectsPanelMatrix::Mode mode)
    : _mainController(CMainController::instance()),
      mapColorsAndFonts_(CColorAndFontMap::instance()),
      _mode(mode)
{
    applyStyleSheet();

    setHorizontalHeader(new CEffectsMatrixHeader());

    horizontalHeader()->setDefaultSectionSize(cellSize().width());
    verticalHeader()->setDefaultSectionSize(cellSize().height());

    horizontalHeader()->setFixedHeight(cellSize().height());
    verticalHeader()->setFixedWidth(cellSize().width());

    horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);

    horizontalHeader()->setDefaultAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    verticalHeader()->setDefaultAlignment(Qt::AlignRight | Qt::AlignVCenter);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setCornerButtonEnabled(false);

    int nColumnsDefault = 16;
    int nRowsDefault = 16;

    CEffectsPanelMatrixDelegate* pAssociationsPanelMatrixDelegate = new CEffectsPanelMatrixDelegate();
    setItemDelegate(pAssociationsPanelMatrixDelegate);

    switch(_mode)
    {
        case Normal:
        {
            setRowCount(nRowsDefault);
            setColumnCount(nColumnsDefault);
            horizontalHeader()->show();
            verticalHeader()->show();
            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            horizontalHeader()->setFixedHeight(HEADER_SIZE);
            verticalHeader()->setFixedWidth(HEADER_SIZE);
            setSelectionMode(QTableWidget::SelectionMode::SingleSelection);
//            setSelectionBehavior(QTableWidget::SelectionBehavior::SelectRows | QTableWidget::SelectionBehavior::SelectColumns);
        }
        break;
        case RightAggregates:
        {
            setColumnCount(NUMBER_OF_AGGREGATES);
            setRowCount(nRowsDefault);
            horizontalHeader()->show();
            verticalHeader()->hide();
            horizontalHeader()->setFixedHeight(HEADER_SIZE);
            verticalHeader()->setFixedWidth(0);
        }
        break;
        case BottomAggregates:
        {
            setColumnCount(nColumnsDefault);
            setRowCount(NUMBER_OF_AGGREGATES);
            horizontalHeader()->hide();
            verticalHeader()->show();
            horizontalHeader()->setFixedHeight(0);
            verticalHeader()->setFixedWidth(HEADER_SIZE);
        }
        break;
    }

    //    resizeToContents();
}

CEffectsPanelMatrix::~CEffectsPanelMatrix()
{

}

QSize CEffectsPanelMatrix::cellSize()
{
    return CEffectsPanelMatrix::_cellSize;
}

void CEffectsPanelMatrix::setCellSize(const QSize &cellSize)
{
    CEffectsPanelMatrix::_cellSize = cellSize;
}

QSize CEffectsPanelMatrix::resizeToContents()
{
    QSize sizeResult;

    int iRowount = ROWS_VISIBLE_MIN;//qMin(rowCount(), ROWS_VISIBLE_MAX);
    int iColumnCount = COLUMNS_VISIBLE_MIN;//qMin(columnCount(), COLUMNS_VISIBLE_MAX);
    int iBorderWidth = 1;
    int iColumnSpacing = 0;
    int iTotalColumnSpacing = qMax(0, (iColumnCount - 1) * iColumnSpacing);
    int iRowSpacing = 0;
    int iTotalRowSpacing = qMax(0, (iRowount - 1) * iRowSpacing);
    int iVerticalScrollBarWidth = QApplication::style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    int iHorizontalScollBarHeight = QApplication::style()->pixelMetric(QStyle::PM_ScrollBarExtent);

    switch(_mode)
    {
        case Normal:
        {
        return sizeResult;
            /* Width x height */
            sizeResult = QSize(verticalHeader()->width() + (iColumnCount * cellSize().width()) +
                                            iTotalColumnSpacing + 2*iBorderWidth + iVerticalScrollBarWidth,
                                horizontalHeader()->height() + (iRowount * cellSize().height()) +
                               iTotalRowSpacing + 2*iBorderWidth + iHorizontalScollBarHeight);

//            sizeResult = QSize(verticalHeader()->width() + (iColumnCount * cellSize().width()) +
//                               iTotalColumnSpacing + 2*iBorderWidth + iHorizontalScollBarHeight,
//                               horizontalHeader()->height() + (iRowount * cellSize().height()) +
//                              iTotalRowSpacing + 2*iBorderWidth + iHorizontalScollBarHeight);
        }
        break;
        case RightAggregates:
        {
            sizeResult = QSize((NUMBER_OF_AGGREGATES * cellSize().width()) + iTotalColumnSpacing + 2*iBorderWidth,
                    horizontalHeader()->height() + (iRowount * cellSize().height()) + iTotalRowSpacing
                               + 2*iBorderWidth);// + iHorizontalScollBarHeight);
        }
        break;
        case BottomAggregates:
        {
            sizeResult = QSize(verticalHeader()->width() + (iColumnCount * cellSize().width()) +
                               iTotalColumnSpacing + 2*iBorderWidth,
                            (NUMBER_OF_AGGREGATES * cellSize().height()) + iTotalRowSpacing + 2*iBorderWidth);
        }
        break;
    }

    setFixedSize(sizeResult);

    return sizeResult;
}

//CApplicationData CEffectsPanelMatrix::applicationData() const
//{
//    return _applicationData;
//}

//void CEffectsPanelMatrix::setApplicationData(const CApplicationData &applicationData)
//{
////    DEBUG_OBJ_INFO

//    _applicationData = applicationData;
//    _associationsModel = applicationData.associationsModel();

//    updateTable();
//}

CAssociationsModel CEffectsPanelMatrix::associationsModel() const
{
    return _associationsModel;
}

void CEffectsPanelMatrix::setAssociationsModel(const CAssociationsModel &associationsModel)
{
    _associationsModel = associationsModel;
    updateTable();
}

void CEffectsPanelMatrix::applyStyleSheet()
{
    QString strStyleSheet = styleSheet();
    strStyleSheet += "QTableWidget { background-color: " +  mapColorsAndFonts_.getColor("widget/generic").name() + ";" + "}";
    strStyleSheet += "QTableWidget { gridline-color: " +  mapColorsAndFonts_.getColor("widget/generic").name() + ";" + "}";
    strStyleSheet += "QHeaderView { border: none; background-color: " +mapColorsAndFonts_.getColor("widget/generic").name() + ";" + "}";
    strStyleSheet += "QHeaderView::section { border: none; background-color: " +mapColorsAndFonts_.getColor("widget/generic").name() + ";" + "}";
    strStyleSheet += "QTableView QTableCornerButton::section {border: none; background-color: " +mapColorsAndFonts_.getColor("widget/generic").name() + ";" + "}";
    setStyleSheet(strStyleSheet);
}

void CEffectsPanelMatrix::updateTable()
{
//    DEBUG_OBJ_INFO

    QStringList lstVariableNames = _associationsModel.variables().variablesNames();
    int iVariablesCount = lstVariableNames.count();

    /* Data */
    switch(_mode)
    {
        case Normal:
        {
            clear();

            setRowCount(iVariablesCount);
            setColumnCount(iVariablesCount);

            setHorizontalHeaderLabels(lstVariableNames);
            setVerticalHeaderLabels(lstVariableNames);

            /* Group 1 */
            QVector<QVector<qreal>> vecValuesGroup1 = _associationsModel.dataFirstGroup();
            for(int iRow=0; iRow<iVariablesCount; ++iRow)
            {
                QVector<qreal> valuesGroup1 = vecValuesGroup1.at(iRow);
                for(int iCol=0; iCol<iRow; ++iCol)
                {
                    qreal rDistance = abs(valuesGroup1.at(iCol));
                    QTableWidgetItem* pItemGroup1 = new QTableWidgetItem();
                    pItemGroup1->setFlags(pItemGroup1->flags() ^ Qt::ItemIsEditable);
                    pItemGroup1->setData(Qt::UserRole, QVariant::fromValue(rDistance));
                    pItemGroup1->setData(Qt::UserRole+1, QVariant::fromValue(1)); //red
                    pItemGroup1->setToolTip(QString::number(iRow) + ", " + QString::number(iCol));

                   setItem(iRow, iCol, pItemGroup1);
                }
            }

            /* Group 2 */
            QVector<QVector<qreal>> vecValuesGroup2 = _associationsModel.dataSecondGroup();
            for(int iCol=0; iCol<iVariablesCount; ++iCol)
            {
                QVector<qreal> valuesGroup2 = vecValuesGroup2.at(iCol);
                for(int iRow=0; iRow < iCol; ++iRow)
                {
                    qreal rDistance = abs(valuesGroup2.at(iRow));
                    QTableWidgetItem* pItemGroup2 = new QTableWidgetItem();
                    pItemGroup2->setFlags(pItemGroup2->flags() ^ Qt::ItemIsEditable);
                    pItemGroup2->setData(Qt::UserRole, QVariant::fromValue(rDistance));
                    pItemGroup2->setData(Qt::UserRole+1, QVariant::fromValue(2));
                    pItemGroup2->setToolTip(QString::number(iRow) + ", " + QString::number(iCol));

                   setItem(iRow, iCol, pItemGroup2);
                }
            }

            /* Group 0 */
            for(int iRow=0; iRow<iVariablesCount; ++iRow)
            {
//                for(int iCol=0; iCol<iRow+1; ++iCol)
//                {
                    QTableWidgetItem* pItemGroup0 = new QTableWidgetItem();
                    pItemGroup0->setFlags(pItemGroup0->flags() ^ Qt::ItemIsEditable);
                    pItemGroup0->setData(Qt::UserRole, QVariant::fromValue(1));
                    pItemGroup0->setData(Qt::UserRole+1, QVariant::fromValue(0));

                   setItem(iRow, iRow, pItemGroup0);
//                }
            }
        }
        break;
        case RightAggregates:
        {
            clear();
//            QString strHeaders("Others;Avg;Min;Max");
//            QStringList lstHeaders = transposeHorizontalHeaderLabels(strHeaders.split(";"));
//            setHorizontalHeaderLabels(lstHeaders);

//            for(int iRow=0; iRow<_groupsDistancesData.rowsCount(); ++iRow)
//            {
//                qreal rOthers = 0, rAvg = 0, rMin = 0, rMax = 0;
//                for(int iCol=0; iCol<NUMBER_OF_AGGREGATES; ++iCol)
//                {
//                    qreal rDistance = _groupsDistancesData.distance(iRow, iCol, _strActiveDistanceMetric);
//                    rAvg += rDistance / _groupsDistancesData.columnCount();
//                    rMin = qMin(rMin, rDistance);
//                    rMax = qMax(rMax, rDistance);
//                }

//                /* Others */
//                QTableWidgetItem* pItemNameOthers = new QTableWidgetItem(QString::number(rOthers));
//                pItemNameOthers->setFlags(pItemNameOthers->flags() ^ Qt::ItemIsEditable);
//                pItemNameOthers->setData(Qt::UserRole, QVariant::fromValue(rOthers));
//                setItem(iRow, 0, pItemNameOthers);

//                /* Avg */
//                QTableWidgetItem* pItemNameAvg = new QTableWidgetItem(QString::number(rAvg));
//                pItemNameAvg->setFlags(pItemNameAvg->flags() ^ Qt::ItemIsEditable);
//                pItemNameAvg->setData(Qt::UserRole, QVariant::fromValue(rAvg));
//                setItem(iRow, 1, pItemNameAvg);

//                /* Min */
//                QTableWidgetItem* pItemNameMin = new QTableWidgetItem(QString::number(rMin));
//                pItemNameMin->setFlags(pItemNameMin->flags() ^ Qt::ItemIsEditable);
//                pItemNameMin->setData(Qt::UserRole, QVariant::fromValue(rMin));
//                setItem(iRow, 2, pItemNameMin);

//                /* Max */
//                QTableWidgetItem* pItemNameMax = new QTableWidgetItem(QString::number(rMax));
//                pItemNameMax->setFlags(pItemNameMin->flags() ^ Qt::ItemIsEditable);
//                pItemNameMax->setData(Qt::UserRole, QVariant::fromValue(rMax));
//                setItem(iRow, 3, pItemNameMax);
//            }
        }
        break;
        case BottomAggregates:
        {
//            clear();
//            QString strHeaders("Others;Avg;Min;Max");
//            setVerticalHeaderLabels(strHeaders.split(";"));

//            for(int iCol=0; iCol<_groupsDistancesData.columnCount(); ++iCol)
//            {
//                qreal rOthers = 0, rAvg = 0, rMin = 0, rMax = 0;
//                for(int iRow=0; iRow<NUMBER_OF_AGGREGATES; ++iRow)
//                {
//                    qreal rDistance = _groupsDistancesData.distance(iRow, iCol, _strActiveDistanceMetric);
//                    rAvg += rDistance / _groupsDistancesData.rowsCount();
//                    rMin = qMin(rMin, rDistance);
//                    rMax = qMax(rMax, rDistance);
//                }

//                /* Others */
//                QTableWidgetItem* pItemNameOthers = new QTableWidgetItem(QString::number(rOthers));
//                pItemNameOthers->setFlags(pItemNameOthers->flags() ^ Qt::ItemIsEditable);
//                pItemNameOthers->setData(Qt::UserRole, QVariant::fromValue(rOthers));
//                setItem(0, iCol, pItemNameOthers);

//                /* Avg */
//                QTableWidgetItem* pItemNameAvg = new QTableWidgetItem(QString::number(rAvg));
//                pItemNameAvg->setFlags(pItemNameAvg->flags() ^ Qt::ItemIsEditable);
//                pItemNameAvg->setData(Qt::UserRole, QVariant::fromValue(rAvg));
//                setItem(1, iCol, pItemNameAvg);

//                /* Min */
//                QTableWidgetItem* pItemNameMin = new QTableWidgetItem(QString::number(rMin));
//                pItemNameMin->setFlags(pItemNameMin->flags() ^ Qt::ItemIsEditable);
//                pItemNameMin->setData(Qt::UserRole, QVariant::fromValue(rMin));
//                setItem(2, iCol, pItemNameMin);

//                /* Max */
//                QTableWidgetItem* pItemNameMax = new QTableWidgetItem(QString::number(rMax));
//                pItemNameMax->setFlags(pItemNameMin->flags() ^ Qt::ItemIsEditable);
//                pItemNameMax->setData(Qt::UserRole, QVariant::fromValue(rMax));
//                setItem(3, iCol, pItemNameMax);
//            }
        }
        break;
    }

//    resizeToContents();
}
