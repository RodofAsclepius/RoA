#ifndef CBALANCEPANELTREEHEADER_H
#define CBALANCEPANELTREEHEADER_H

#include <QHeaderView>

class CBalancePanelTreeHeader : public QHeaderView
{
    Q_OBJECT
public:
    CBalancePanelTreeHeader(Qt::Orientation orientation, QWidget *parent = nullptr);
    ~CBalancePanelTreeHeader();

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
};

#endif // CBALANCEPANELTREEHEADER_H
