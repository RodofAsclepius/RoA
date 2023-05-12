#ifndef GROUPSLISTDELEGATE_H
#define GROUPSLISTDELEGATE_H

#include <QStyledItemDelegate>

class CGroupsListDelegateColor : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CGroupsListDelegateColor(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

private slots:

};

#endif // GROUPSLISTDELEGATE_H
