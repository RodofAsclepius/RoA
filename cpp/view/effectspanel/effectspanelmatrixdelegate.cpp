#include "effectspanelmatrixdelegate.h"

#include <QtWidgets>
#include <QVariant>
#include <QtCore>

#include "utils.h"

CEffectsPanelMatrixDelegate::CEffectsPanelMatrixDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{

}

int interpolate(int startValue, int endValue, int stepNumber, int lastStepNumber)
{
    return (endValue - startValue) * stepNumber / lastStepNumber + startValue;
}

void CEffectsPanelMatrixDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
//    DEBUG_OBJ_INFO

    painter->setRenderHint(QPainter::Antialiasing, true);
    qreal rDistance = index.data(Qt::UserRole).toReal();
    int groupNumber = index.data(Qt::UserRole+1).toInt();

    QColor color = Qt::green;

    if(groupNumber == 1)
    {
        color = Qt::red;

    } else if(groupNumber == 2)
    {
        color = Qt::blue;
    }
    else
    {
        color = Qt::lightGray;
    }

    color.setAlphaF(rDistance);

    QPen pen = painter->pen();
    pen.setWidthF(1.0);
    painter->setPen(pen);
    painter->fillRect(option.rect, color);

}

QSize CEffectsPanelMatrixDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

QWidget *CEffectsPanelMatrixDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const

{
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void CEffectsPanelMatrixDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
    QStyledItemDelegate::setEditorData(editor, index);
}

void CEffectsPanelMatrixDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
}

void CEffectsPanelMatrixDelegate::commitAndCloseEditor()
{
    QWidget *editor = qobject_cast<QWidget *>(sender());

    emit commitData(editor);
    emit closeEditor(editor);
}
