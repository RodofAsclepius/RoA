#ifndef GROUPSLISTDELEGATESIZE_H
#define GROUPSLISTDELEGATESIZE_H

#include <QStyledItemDelegate>

class CGroupsListDelegateSize : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CGroupsListDelegateSize(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

private slots:

};

#endif // GROUPSLISTDELEGATESIZE_H
