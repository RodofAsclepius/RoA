#include "balancepaneldistributionsview.h"
#include "ui_balancepaneldistributionsview.h"

#include "distributionview.h"
#include "applicationdata.h"
#include <QPainter>

#define VARIABLE_INT_TO_CAT_TRESHOLD 8

CBalancePanelDistributionsView::CBalancePanelDistributionsView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CBalancePanelDistributionsView)
{
    ui->setupUi(this);

//    setMinimumWidth(30);
//    setMinimumHeight(100);

    QString strStyleSheet = styleSheet();

    strStyleSheet += "QWidget { border: 1px solid black;}";

    setStyleSheet(strStyleSheet);

    pDistributionViewUntreated_ = new CDistributionView();
    pDistributionViewTreated_ = new CDistributionView();

    setContentsMargins(12, 12, 12, 12);

    QGridLayout * pLayoutViews = new QGridLayout();
    ui->gridLayout->addLayout(pLayoutViews, 0, 0, 1, 1);

    pLayoutViews->addWidget(pDistributionViewTreated_, 0, 0, 1, 1);
    pLayoutViews->addWidget(pDistributionViewUntreated_, 1, 0, 1, 1);
    pLayoutViews->setSpacing(0);
    pLayoutViews->setContentsMargins(0, 0, 0, 0);

    int iHorMarCharts = 12;

    /* Top one */
    pDistributionViewTreated_->setOrientationMode(CDistributionView::North);
    pDistributionViewTreated_->setDrawValueLabels(false);
    pDistributionViewTreated_->setDrawLevelLabelsHistogram(false);
    pDistributionViewTreated_->setDrawAxes(true, false);
    pDistributionViewTreated_->setStyleSheet("QWidget {border:none;}");
    pDistributionViewTreated_->setContentsMargins(0, 1, 0, 0);
    pDistributionViewTreated_->setMarginsPlot(QMargins(iHorMarCharts, 6, iHorMarCharts, 0));
    pDistributionViewTreated_->setMarginsLabels(QMargins(0, 0, 0, 0));
    pDistributionViewTreated_->setBarWidthHistogram(24);
    pDistributionViewTreated_->setBarSpacingHistogram(2);
    pDistributionViewTreated_->setAxesWidth(1);
    pDistributionViewTreated_->setUseGroupColorsForMessage(false);
    pDistributionViewTreated_->setShowMessage(false);
    pDistributionViewTreated_->setMessage("");

    /* Bottom one */
    pDistributionViewUntreated_->setOrientationMode(CDistributionView::South);
    pDistributionViewUntreated_->setDrawValueLabels(false);
    pDistributionViewUntreated_->setDrawLevelLabelsHistogram(false);
    pDistributionViewUntreated_->setDrawAxes(false, false);
    pDistributionViewUntreated_->setUseSecondaryGroupColors(true);
    pDistributionViewUntreated_->setStyleSheet("QWidget {border:none;}");
    pDistributionViewUntreated_->setContentsMargins(0, 0, 0, 1);
    pDistributionViewUntreated_->setMarginsPlot(QMargins(iHorMarCharts, 0, iHorMarCharts, 6));
    pDistributionViewUntreated_->setMarginsLabels(QMargins(0, 0, 0, 0));
    pDistributionViewUntreated_->setBarWidthHistogram(24);
    pDistributionViewUntreated_->setBarSpacingHistogram(2);
    pDistributionViewUntreated_->setAxesWidth(1);
    pDistributionViewUntreated_->setUseGroupColorsForMessage(false);
    pDistributionViewUntreated_->setShowMessage(false);
    pDistributionViewUntreated_->setMessage("");
}

void CBalancePanelDistributionsView::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);

    QPen pen(QColor("#555555"), 1);
    painter.setPen(pen);
    painter.drawRect(rect().adjusted(1, 0, 0, 0));
}

CBalancePanelDistributionsView::~CBalancePanelDistributionsView()
{
    delete ui;
}

CDistributionsList CBalancePanelDistributionsView::distributionsUntreated() const
{
    return lstDistributionsUntreated_;
}

void CBalancePanelDistributionsView::setDistributionsUntreated(const CDistributionsList &lstDistributionsUntreated)
{
    lstDistributionsUntreated_ = lstDistributionsUntreated;

    if(!lstDistributionsUntreated_.isEmpty())
    {
        if(lstDistributionsUntreated_.first().variable().type() == "factor")
        {
            pDistributionViewUntreated_->setViewMode(CDistributionView::Histogram);
        }
        pDistributionViewUntreated_->setDistributions(lstDistributionsUntreated_);
    }
}

CDistributionsList CBalancePanelDistributionsView::distributionsTreated() const
{
    return lstDistributionsTreated_;
}

void CBalancePanelDistributionsView::setDistributionsTreated(const CDistributionsList &lstDistributionsTreated)
{
    lstDistributionsTreated_ = lstDistributionsTreated;

    if(!lstDistributionsTreated_.isEmpty())
    {
        if(lstDistributionsTreated_.first().variable().type() == "factor")
        {
            pDistributionViewTreated_->setViewMode(CDistributionView::Histogram);
        }
        pDistributionViewTreated_->setDistributions(lstDistributionsTreated_);
    }
}

CDistributionView *CBalancePanelDistributionsView::distributionViewUntreated() const
{
    return pDistributionViewUntreated_;
}

CDistributionView *CBalancePanelDistributionsView::distributionViewTreated() const
{
    return pDistributionViewTreated_;
}
