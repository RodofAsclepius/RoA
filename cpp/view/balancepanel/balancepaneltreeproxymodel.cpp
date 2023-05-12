#include "balancepaneltreeproxymodel.h"

#include <QtWidgets>
#include <QRegularExpression>
#include "variable.h"
#include "balancepaneltreemodel.h"

CBalancePanelTreeSortFilterProxyModel::CBalancePanelTreeSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

void CBalancePanelTreeSortFilterProxyModel::setSortingMode(CBalancePanelTreeSortFilterProxyModel::SortingMode sortingMode)
{
//    qDebug() << "sorting mode in proxy model";
    iSortingMode_ = sortingMode;
}

bool CBalancePanelTreeSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    return true;
}

bool CBalancePanelTreeSortFilterProxyModel::lessThan(const QModelIndex &left,  const QModelIndex &right) const
{
//    CBalancePanelTreeModel* pSourceModel = dynamic_cast<CBalancePanelTreeModel*>(sourceModel());
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    /* Variable */
    if(left.column() == COLUMN_VARIABLE && right.column() ==  COLUMN_VARIABLE)
    {
        /* Variables */
        if(leftData.canConvert<CVariable>() && rightData.canConvert<CVariable>())
        {
            QString leftString = leftData.value<CVariable>().name();
            QString rightString = rightData.value<CVariable>().name();
//            qDebug() << "variable :" << leftString << rightString;
            return QString::localeAwareCompare(leftString, rightString) < 0;
        }

        /* Levels */
        if(leftData.canConvert<QString>() && rightData.canConvert<QString>())
        {
            QString leftString = leftData.value<QString>();
            QString rightString = rightData.value<QString>();
//            qDebug() << "levels: " << leftString << rightString;
            return QString::localeAwareCompare(leftString, rightString) < 0;
        }
    }

    /* Distributions */
    if(left.column() == COLUMN_DISTRIBUTION && right.column() == COLUMN_DISTRIBUTION)
    {
        return true;
    }

    /* Curves */
    if(left.column() == COLUMN_CURVES && right.column() == COLUMN_CURVES)
    {
        QVariant leftData = sourceModel()->data(left);
        QVariant rightData = sourceModel()->data(right);

        QPair<qreal, qreal> pairValuesUnAdjLeft = leftData.value<QPair<qreal, qreal>>();
        QPair<qreal, qreal> pairValuesUnAdjRight = rightData.value<QPair<qreal, qreal>>();

        qreal rMaxValueUnLeft = pairValuesUnAdjLeft.first;
        qreal rMaxValueUnRight = pairValuesUnAdjRight.first;
        qreal rMaxValueAdjLeft = pairValuesUnAdjLeft.second;
        qreal rMaxValueAdjRight = pairValuesUnAdjRight.second;

        qreal rMaxValueLeft = qMax(rMaxValueUnLeft, rMaxValueAdjLeft);
        qreal rMaxValueRight = qMax(rMaxValueUnRight, rMaxValueAdjRight);

        if(iSortingMode_ == SortingMode::Adjusted)
        {
            return rMaxValueAdjLeft < rMaxValueAdjRight;
        }
        if(iSortingMode_ == SortingMode::Unadjusted)
        {
            return rMaxValueUnLeft < rMaxValueUnRight;
        }
        if(iSortingMode_ == SortingMode::Max)
        {
            return rMaxValueLeft < rMaxValueRight;
        }
    }

    return left.row() < right.row();
}
