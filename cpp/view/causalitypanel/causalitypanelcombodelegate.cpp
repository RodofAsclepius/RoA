#include "causalitypanelcombodelegate.h"
#include <QPainter>

CCausalityPanelComboDelegate::CCausalityPanelComboDelegate()
    :QItemDelegate()
{

}

CCausalityPanelComboDelegate::~CCausalityPanelComboDelegate()
{

}

void CCausalityPanelComboDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem newOption(option);
    if(index.data().toString()=="Black"){
        newOption.palette.setColor(QPalette::Text, Qt::white);
    }

    drawBackground(painter, newOption, index);
    QItemDelegate::paint(painter, newOption, index);
}

void CCausalityPanelComboDelegate::drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QString text=index.data().toString();
    if(text=="Red") painter->fillRect(option.rect, Qt::red);
    else if(text=="Black") painter->fillRect(option.rect, Qt::black);
}
