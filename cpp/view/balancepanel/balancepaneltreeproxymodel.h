#ifndef BALANCEPANELTREEFILTERMODEL_H
#define BALANCEPANELTREEFILTERMODEL_H

#include <QDate>
#include <QSortFilterProxyModel>

class CBalancePanelTreeSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    CBalancePanelTreeSortFilterProxyModel(QObject *parent = 0);

    typedef enum
    {
        Adjusted,
        Unadjusted,
        Max
    } SortingMode;

    void setSortingMode(SortingMode sortingMode);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

    int iSortingMode_;
private:
};

#endif // BALANCEPANELTREEFILTERMODEL_H
