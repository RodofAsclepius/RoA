#ifndef BALANCEPANELDISTRIBUTIONSVIEW_H
#define BALANCEPANELDISTRIBUTIONSVIEW_H

#include <QWidget>
#include "distributionlist.h"

namespace Ui {
class CBalancePanelDistributionsView;
}

class CDistributionView;

class CBalancePanelDistributionsView : public QWidget
{
    Q_OBJECT

public:
    explicit CBalancePanelDistributionsView(QWidget *parent = nullptr);
    ~CBalancePanelDistributionsView();

    CDistributionsList distributionsUntreated() const;
    void setDistributionsUntreated(const CDistributionsList &distributionsUntreated);

    CDistributionsList distributionsTreated() const;
    void setDistributionsTreated(const CDistributionsList &distributionsTreated);

    CDistributionView *distributionViewUntreated() const;
    CDistributionView *distributionViewTreated() const;

private:
    Ui::CBalancePanelDistributionsView *ui;
    CDistributionsList lstDistributionsUntreated_;
    CDistributionsList lstDistributionsTreated_;

    CDistributionView *pDistributionViewUntreated_;
    CDistributionView *pDistributionViewTreated_;
    void paintEvent(QPaintEvent *e);
};

#endif // BALANCEPANELDISTRIBUTIONSVIEW_H
