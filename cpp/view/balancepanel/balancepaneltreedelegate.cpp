#include "balancepaneltreedelegate.h"

#include <QtWidgets>
#include <QVariant>
#include <QtCore>
#include <QColor>

#include "variable.h"
#include "distributionlist.h"

void CVariablesTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    int iMargin = 0;

    painter->setRenderHint(QPainter::Antialiasing, true);
    QVariant var = index.data(Qt::DisplayRole);

//    /* Background */
//    if(index.row() % 2 == 1)
//    {
//        painter->fillRect(option.rect, QColor(250, 250, 250));
//    }

    /* Variable */
    if(var.canConvert<CVariable>())
    {
        CVariable variable = var.value<CVariable>();
        QRect rect = option.rect.adjusted(iMargin, iMargin, -iMargin, -iMargin);
        QFont font = painter->font();

        font.setPointSize(16);
        font.setWeight(QFont::Bold);
        painter->setFont(font);

        QFontMetrics fm(font);
        QRect rectName = rect.adjusted(0, 10, 0, 10);

        QString strText = variable.name();

        QPoint pntOffset = option.rect.center();
        pntOffset += QPoint(0, fm.height()/4);
        pntOffset.setX(rect.left());

        QPen penText(QColor("#000000"));
        painter->setPen(penText);
        painter->drawText(pntOffset, strText);

//        rect = option.rect.adjusted(iMargin, iMargin + 6, -iMargin, -iMargin);
//        font.setPointSize(10);
//        font.setWeight(QFont::Normal);
//        painter->setFont(font);
//        painter->drawText(rect, variable.type(), QTextOption(Qt::AlignVCenter));
        return;
    }

    /* Variable level */
    if(var.canConvert<QString>())
    {
        QString strVariableLevel = var.value<QString>();
        /* Sub items */
        QRect rect = option.rect.adjusted(iMargin, iMargin, -iMargin, -iMargin);
        QFont font = painter->font();

        font.setPointSize(14);
        font.setBold(false);
//        font.setItalic(true);
        painter->setFont(font);
//            QString strText = index.data(Qt::DisplayRole).toString();
        QPen penText(QColor("#000000"));
        painter->setPen(penText);

        QFontMetrics fm(font);
        QRect rectName = rect.adjusted(0, 10, 0, 10);
        QPoint pntOffset = option.rect.center();
        pntOffset += QPoint(-fm.horizontalAdvance(strVariableLevel)/2, fm.height()/4);
        pntOffset.setX(rect.left() + 48);
        painter->drawText(pntOffset, strVariableLevel);

//        painter->drawText(rect, strVariableLevel, QTextOption(Qt::AlignHCenter));
    }

    if(var.canConvert<CDistributionsList>())
    {

    }
}

QSize CVariablesTableDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

QWidget *CVariablesTableDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const

{
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void CVariablesTableDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
    QStyledItemDelegate::setEditorData(editor, index);
}

void CVariablesTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
}

void CVariablesTableDelegate::commitAndCloseEditor()
{
    QWidget *editor = qobject_cast<QWidget *>(sender());

    emit commitData(editor);
    emit closeEditor(editor);
}
