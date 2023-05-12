#ifndef BALANCEPANELTREEWEIGHTSDELETE_H
#define BALANCEPANELTREEWEIGHTSDELETE_H

#include <QStyledItemDelegate>
#include "group.h"
#include "variable.h"

class CBalancePanelTreeWeightsDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    const int CHECK_ROLE = Qt::CheckStateRole;

    CBalancePanelTreeWeightsDelegate(QObject *parent = 0) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    CGroup group() const;
    void setGroup(const CGroup &group);

    int totalWidth() const;
    void setTotalWidth(int totalWidth);

    int groupCount() const;
    void setGroupCount(int groupCount);

    int groupIndex() const;
    void setGroupIndex(int groupIndex);

    QRect checkboxRect(const QStyleOptionViewItem &option) const;

protected:
    CGroup group_;
    int iTotalWidth_;
    int iGroupCount_;
    int iGroupIndex_;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
private slots:
    void commitAndCloseEditor();
};

#endif // BALANCEPANELTREEWEIGHTSDELETE_H
