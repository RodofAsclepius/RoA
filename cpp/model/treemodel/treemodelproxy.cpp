#include "treemodelproxy.h"
#include <QDebug>

CTreeModelProxy::CTreeModelProxy(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool CTreeModelProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    return true;
}

bool CTreeModelProxy::lessThan(const QModelIndex &left,  const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    /* Curves */
    if(left.column() == 3 && right.column() == 3)
    {
        QVariantList lstLeft = leftData.toList();
        QVariantList lstRight = rightData.toList();

        if(lstLeft.isEmpty() && !lstRight.isEmpty())
        {
            return false;
        }
        if(!lstLeft.isEmpty() && lstRight.isEmpty())
        {
            return true;
        }

        if(lstLeft.isEmpty() && lstRight.isEmpty())
        {
            return true;
        }

        /* Else find max */
        qreal rMaxLeft = 0;
        foreach(QVariant varData, lstLeft)
        {
            qreal rValue = varData.toReal();
            rMaxLeft = qMax(rMaxLeft, qAbs(rValue));
        }

        qreal rMaxRight = 0;
        foreach(QVariant varData, lstRight)
        {
            qreal rValue = varData.toReal();
            rMaxRight = qMax(rMaxRight, qAbs(rValue));
        }

        return rMaxLeft < rMaxRight;
    }

    QString leftString = leftData.toString();
    QString rightString = rightData.toString();
    return QString::localeAwareCompare(leftString, rightString) < 0;
}
