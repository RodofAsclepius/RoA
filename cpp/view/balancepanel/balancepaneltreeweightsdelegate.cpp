#include "balancepaneltreeweightsdelegate.h"

#include <QtWidgets>
#include <QVariant>
#include <QtCore>
#include <QColor>
#include <QWidget>

#include "variable.h"
#include "distributionlist.h"
#include "balancepaneltreemodel.h"

void CBalancePanelTreeWeightsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

//    QStyleOptionButton cbOpt;
//    cbOpt.rect = GetCheckboxRect(option);

//    bool isChecked = index.data(CHECK_ROLE ).toBool();
//    if (isChecked)
//    {
//        cbOpt.state |= QStyle::State_On;
//    }
//    else
//    {
//        cbOpt.state |= QStyle::State_Off;
//    }

//    QVariant enabled = index.data(Qt::ItemIsEnabled);
//    if(enabled.isNull() || enabled.toBool() )
//    {
//        cbOpt.state |= QStyle::State_Enabled;
//    }

//    QApplication::style()->drawControl(QStyle::CE_CheckBox, &cbOpt, painter);

//    QColor clrBackground = QColor("#313131"); // dark gray
    QColor clrBackground = QColor("white");
//    QPen penBorder = QPen(QColor("#525252"), 1.0);
    QPen penBorder = QPen(QColor("#555555"), 1);

    /* Background and border */
    QRect rectBorder = QRect(option.rect.left(), option.rect.top()+2, option.rect.width(), option.rect.height()-2);
    painter->setPen(penBorder);
//    painter->fillRect(rectBorder, clrBackground);

    if(iGroupIndex_ == 0 || iGroupIndex_ == iGroupCount_-1)
    {
        /* Background and border */
        QRect rectBorder = QRect(0, option.rect.top(), iTotalWidth_-2, option.rect.height());
        painter->setPen(penBorder);
        painter->setBrush(Qt::transparent);
        painter->drawRect(rectBorder);
    }

    /* Is this the top level item ? */
    if(!index.parent().isValid())
    {
        /* Draw variable name */
        QModelIndex idxVariable = index.model()->index(index.row(), COLUMN_VARIABLE);
        CVariable variable;
        QVariant varVariable = idxVariable.data(Qt::DisplayRole);
        if(varVariable.canConvert<CVariable>())
        {
            variable = varVariable.value<CVariable>();
        }
        QFont fontText = painter->font();
        fontText.setBold(true);
        painter->setFont(fontText);
        QFontMetrics fm(fontText);
        int iPixelsWidth = fm.horizontalAdvance(variable.name());
        QPen penText(Qt::black, 2);
        painter->setPen(penText);
        painter->drawText(iTotalWidth_/2 - iPixelsWidth/2, option.rect.top()+1.5*fm.height(), variable.name());

        /* Draw buttons */
        QVariant varData = index.data(Qt::CheckStateRole);
        int iCheckState = varData.toInt();
        int iButtonSize = 10;
        QBrush brushGroupColor(group_.colorPrimary());

        if(iCheckState == Qt::CheckState::Checked) // On
        {
            QColor clrEdge = Qt::black;
            QPen pen(clrEdge, 1);
            painter->setPen(pen);
            painter->setBrush(brushGroupColor);
            painter->drawRect(option.rect.center().x() - iButtonSize/2,
                                 option.rect.center().y() - iButtonSize/2, iButtonSize, iButtonSize);
    //        painter->drawEllipse(option.rect.center().x() - iButtonSize/2,
    //                             option.rect.center().y() - iButtonSize/2, iButtonSize, iButtonSize);
        }
        else if(iCheckState == Qt::CheckState::Unchecked) // Off
        {
            iButtonSize = 8;
            QPen pen(group_.colorPrimary(), .5);
            painter->setPen(pen);
            painter->setBrush(clrBackground);
            painter->drawRect(option.rect.center().x() - iButtonSize/2,
                                 option.rect.center().y() - iButtonSize/2, iButtonSize, iButtonSize);
    //        painter->drawEllipse(option.rect.center().x() - iButtonSize/2,
    //                             option.rect.center().y() - iButtonSize/2, iButtonSize, iButtonSize);
        }
    }
    else
    {   // Not valid parent index
        /* Variable level */
        QModelIndex idxParentItem = index.parent();

        QModelIndex idxVariable = idxParentItem.model()->index(idxParentItem.row(), COLUMN_VARIABLE);
        QVariant varVariable = idxVariable.data(Qt::DisplayRole);
        CVariable variable = varVariable.value<CVariable>();

        QModelIndex idxVariableNameLevel = index.model()->index(index.row(), COLUMN_VARIABLE, idxParentItem);
        QVariant varLevelData = idxVariableNameLevel.data(Qt::DisplayRole);

        if(varLevelData.canConvert<QString>())
        {
            /* Draw variable name */
            QFont fontTextVariable = painter->font();
            fontTextVariable.setBold(true);
            painter->setFont(fontTextVariable);
            QFontMetrics fm(fontTextVariable);
            int iPixelsWidth = fm.horizontalAdvance(variable.name());
            QPen penText(Qt::black, 2);
            painter->setPen(penText);
            painter->drawText(iTotalWidth_/2 - iPixelsWidth/2, option.rect.top()+1.5*fm.height(), variable.name());

            /* Draw level */
            QString strLevel = varLevelData.value<QString>();
            QFont fontTextLevel = painter->font();
//            fontTextLevel.setBold(true);
            painter->setFont(fontTextLevel);
            QFontMetrics fmLevel(fontTextLevel);
            int iPixelsWidthLevel = fmLevel.horizontalAdvance(strLevel);
            QPen penTextLevel(Qt::black, 1);
            painter->setPen(penTextLevel);
            painter->drawText(iTotalWidth_/2 - iPixelsWidthLevel/2, option.rect.center().y()+1.5*fm.height()/2, strLevel);
        }
    }
}

QSize CBalancePanelTreeWeightsDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

QRect CBalancePanelTreeWeightsDelegate::checkboxRect(const QStyleOptionViewItem &option) const
{
    QStyleOptionButton opt_button;
    opt_button.QStyleOption::operator=(option); //SE_ItemViewItemCheckIndicator
    QRect sz = QApplication::style()->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &opt_button);
    QRect r = option.rect;
    // center 'sz' within 'r'
    int dx = (r.width() - sz.width())/2;
    int dy = (r.height()- sz.height())/2;
    r.setTopLeft(r.topLeft() + QPoint(dx,dy));
    r.setWidth(sz.width());
    r.setHeight(sz.height());

    return r;
}

QWidget *CBalancePanelTreeWeightsDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const

{
    Q_UNUSED(parent)
    Q_UNUSED(option)
    Q_UNUSED(index)

    return nullptr;
//    return QStyledItemDelegate::createEditor(parent, option, index);
}

bool CBalancePanelTreeWeightsDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* pME = static_cast<QMouseEvent*>(event);
        if(pME->button() == Qt::LeftButton)
        {
            QRect ro = checkboxRect(option);
            QPoint pte = pME->pos();
            if(ro.contains(pte) )
            {
                QVariant varData = index.data(Qt::CheckStateRole);
                int iCheckState = varData.toInt();
                if(iCheckState == Qt::CheckState::Checked)
                {
                    model->setData(index, Qt::CheckState::Unchecked, Qt::CheckStateRole);
                }
                else
                {
                    model->setData(index, Qt::CheckState::Checked, Qt::CheckStateRole);
                }
                return true;
            }
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void CBalancePanelTreeWeightsDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
    QStyledItemDelegate::setEditorData(editor, index);
}

void CBalancePanelTreeWeightsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
}

void CBalancePanelTreeWeightsDelegate::commitAndCloseEditor()
{
    QWidget *editor = qobject_cast<QWidget *>(sender());

    emit commitData(editor);
    emit closeEditor(editor);
}

CGroup CBalancePanelTreeWeightsDelegate::group() const
{
    return group_;
}

void CBalancePanelTreeWeightsDelegate::setGroup(const CGroup &group)
{
    group_ = group;
}

int CBalancePanelTreeWeightsDelegate::totalWidth() const
{
    return iTotalWidth_;
}

void CBalancePanelTreeWeightsDelegate::setTotalWidth(int totalWidth)
{
    iTotalWidth_ = totalWidth;
}

int CBalancePanelTreeWeightsDelegate::groupCount() const
{
    return iGroupCount_;
}

void CBalancePanelTreeWeightsDelegate::setGroupCount(int groupCount)
{
    iGroupCount_ = groupCount;
}

int CBalancePanelTreeWeightsDelegate::groupIndex() const
{
    return iGroupIndex_;
}

void CBalancePanelTreeWeightsDelegate::setGroupIndex(int groupIndex)
{
    iGroupIndex_ = groupIndex;
}

