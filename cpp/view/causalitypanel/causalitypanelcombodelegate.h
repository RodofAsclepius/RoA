#ifndef CCAUSALITYPANELCOMBODELEGATE_H
#define CCAUSALITYPANELCOMBODELEGATE_H

#include <QItemDelegate>

class CCausalityPanelComboDelegate : public QItemDelegate {
public:
    CCausalityPanelComboDelegate();
    ~CCausalityPanelComboDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // CCAUSALITYPANELCOMBODELEGATE_H
