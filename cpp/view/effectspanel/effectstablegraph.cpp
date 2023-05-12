#include "effectstablegraph.h"
#include <QGridLayout>
#include "distributionview.h"
#include <QLabel>
#include "colorandfontmap.h"
#include <QDebug>

#define COLUMN_DISTRIBUTION_U 0
#define COLUMN_DISTRIBUTION_T 1
#define COLUMN_COUNT 3

CEffectsTableGraph::CEffectsTableGraph(QWidget *parent)
    : QWidget(parent),
      colorAndFontMap_(CColorAndFontMap::instance()),
      pDistributionViewU_(new CDistributionView()),
      pDistributionViewT_(new CDistributionView()),
      pLabelGlassDelta_(new QLabel())
{
    setContentsMargins(0, 3, 0, 3);
    QGridLayout* pGridLayout = new QGridLayout;
    pGridLayout->setContentsMargins(0, 0, 0, 0);
//    pGridLayout->setSpacing(2);
//    pGridLayout->setHorizontalSpacing(2);
    setLayout(pGridLayout);

    pGridLayout->addWidget(pLabelGlassDelta_, 0, 0, 1, 2);

    pDistributionViewU_->setFixedWidth(COLUMN_DISTRIBUTION_U_WIDTH);
    pDistributionViewU_->setViewMode(CDistributionView::ViolinPlot);
    pDistributionViewU_->setDrawAxes(false, true);
    pDistributionViewU_->setStyleSheet("border: none");
    pDistributionViewU_->setMarginsLabels(QMargins(0, 0, 0, 14));
    pDistributionViewU_->setMarginsPlot(QMargins(12, 0, 0, 12));
    pDistributionViewU_->setUseSecondaryGroupColors(true);
    pDistributionViewU_->setUseGroupColorsForLables(true);

    pDistributionViewT_->setFixedWidth(COLUMN_DISTRIBUTION_T_WIDTH);
    pDistributionViewT_->setViewMode(CDistributionView::ViolinPlot);
    pDistributionViewT_->setDrawAxes(false, true);
    pDistributionViewT_->setStyleSheet("border: none");
    pDistributionViewT_->setMarginsLabels(QMargins(0, 0, 0, 14));
    pDistributionViewT_->setMarginsPlot(QMargins(12, 0, 0, 12));
    pDistributionViewT_->setBoldLevelLabel(true);
    pDistributionViewT_->setUseGroupColorsForLables(true);

    pGridLayout->addWidget(pDistributionViewU_, 1, 0, 1, 1);
    pGridLayout->addWidget(pDistributionViewT_, 1, 1, 1, 1);


    QSpacerItem *pSpacerItem = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Fixed);
    pGridLayout->addItem(pSpacerItem, 2, 0, 1, 2);
//    pLabelGlassDelta_->setFixedHeight(20);
    updateGraph();
}

void CEffectsTableGraph::setDistributionU(const CDistributionsList &lstDistributions)
{
    pDistributionViewU_->setDistributions(lstDistributions);
}

void CEffectsTableGraph::setDistributionT(const CDistributionsList &lstDistributions)
{
    pDistributionViewT_->setDistributions(lstDistributions);
}

void CEffectsTableGraph::updateGraph()
{
    QString strDMeans = "Glass' Δ: " + QString::number(dMeans_);
    QFont font = pLabelGlassDelta_->font();
    font.setPixelSize(18);
    font.setBold(true);
    pLabelGlassDelta_->setFont(font);
    pLabelGlassDelta_->setText(strDMeans);
    pLabelGlassDelta_->setAlignment(Qt::AlignHCenter);

    QColor clrTextGlassDelta = Qt::white;
    if(!pDistributionViewU_->distributions().isEmpty())
    {
        CDistributionsList lstDstribution = pDistributionViewU_->distributions();
        CGroup group = lstDstribution.first().group();
        clrTextGlassDelta = group.colorPrimary();
    }

    /* Set background color */
//    qPValue_ = 0.001;
    if(qPValue_ < 0.05)
    {
        QColor colorBackground = colorAndFontMap_.getColor("generic/alert");
        QColor clrText = Qt::white;
        pLabelGlassDelta_->setStyleSheet("color: " + clrText.name() + ";" +
                                         "background-color: " + colorBackground.name());
        font.setBold(true);
        font.setItalic(false);
        pLabelGlassDelta_->setFont(font);
    }
    else
    {
        QColor colorBackground = clrBackGroundLabel;
        QColor clrText = clrTextGlassDelta;
//        pLabelGlassDelta_->setStyleSheet("color: " + clrText.name() + ";" +
//                                         "background-color: " + colorBackground.name());
        pLabelGlassDelta_->setStyleSheet("color: " + clrText.name() + ";" +
                                         "background-color: white");
        font.setBold(false);
        font.setItalic(false);
        pLabelGlassDelta_->setFont(font);
    }
}

qreal CEffectsTableGraph::rEffectSize() const
{
    return rEffectSize_;
}

void CEffectsTableGraph::setREffectSize(const qreal &rEffectSize)
{
    rEffectSize_ = rEffectSize;
    updateGraph();
}

qreal CEffectsTableGraph::rVarianceU() const
{
    return rVarianceU_;
}

void CEffectsTableGraph::setVarianceU(const qreal &rVarianceU)
{
    rVarianceU_ = rVarianceU;
}

void CEffectsTableGraph::setGlobalDomainViolinPlot(QPair<qreal, qreal> pairDomain) const
{
    pDistributionViewU_->setGlobalDomainViolinPlot(pairDomain);
    pDistributionViewT_->setGlobalDomainViolinPlot(pairDomain);
}

void CEffectsTableGraph::setUseGlobalDomainViolinPlot(bool bUseGlobalDomain) const
{
    pDistributionViewU_->setUseGlobalDomainForViolinPlot(bUseGlobalDomain);
    pDistributionViewT_->setUseGlobalDomainForViolinPlot(bUseGlobalDomain);
}

qreal CEffectsTableGraph::qEstimateSE() const
{
    return qEstimateSE_;
}

void CEffectsTableGraph::setQEstimateSE(const qreal &qEstimateSE)
{
    qEstimateSE_ = qEstimateSE;
    updateGraph();
}

qreal CEffectsTableGraph::deltaMeans() const
{
    return dMeans_;
}

void CEffectsTableGraph::setDeltaMeans(const qreal &value)
{
    dMeans_ = value;
    updateGraph();
}

qreal CEffectsTableGraph::PValue() const
{
    return qPValue_;
}

void CEffectsTableGraph::setPValue(const qreal &qPValue)
{
    qPValue_ = qPValue;
    updateGraph();
}

qreal CEffectsTableGraph::estimate() const
{
    return qEstimate_;
}

void CEffectsTableGraph::setEstimate(const qreal &qEstimate)
{
    qEstimate_ = qEstimate;
    updateGraph();
}
