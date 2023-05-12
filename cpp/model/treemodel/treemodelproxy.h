#ifndef TREEMODELPROXY_H
#define TREEMODELPROXY_H

#include <QSortFilterProxyModel>

class CTreeModelProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    CTreeModelProxy(QObject *parent = 0);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
};

#endif // TREEMODELPROXY_H
