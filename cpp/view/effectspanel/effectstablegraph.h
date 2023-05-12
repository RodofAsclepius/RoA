#ifndef CEFFECTSTABLEGRAPH_H
#define CEFFECTSTABLEGRAPH_H

#include <QWidget>
#include "distributionlist.h"

#define COLUMN_DISTRIBUTION_U_WIDTH 175
#define COLUMN_DISTRIBUTION_T_WIDTH 175
//#define COLUMN_DISTRIBUTION_U_HEIGHT 140
//#define COLUMN_DISTRIBUTION_T_HEIGHT 140

class CColorAndFontMap;
class CDistributionView;
class QLabel;

class CEffectsTableGraph : public QWidget
{
    Q_OBJECT
public:
    explicit CEffectsTableGraph(QWidget *parent = nullptr);

    void setDistributionU(const CDistributionsList &lstDistributions);
    void setDistributionT(const CDistributionsList &lstDistributions);

    qreal estimate() const;
    void setEstimate(const qreal &qEstimate);
    qreal PValue() const;
    void setPValue(const qreal &qPValue);
    qreal deltaMeans() const;
    void setDeltaMeans(const qreal &value);
    qreal qEstimateSE() const;
    void setQEstimateSE(const qreal &qEstimateSE);
    void updateGraph();
    qreal rEffectSize() const;
    void setREffectSize(const qreal &rEffectSize);
    qreal rVarianceU() const;
    void setVarianceU(const qreal &rVarianceU);

    void setGlobalDomainViolinPlot(QPair<qreal, qreal> pairDomain) const;
    void setUseGlobalDomainViolinPlot(bool bUseGlobalDomain) const;

protected:
    CColorAndFontMap &colorAndFontMap_;
    CDistributionView *pDistributionViewU_;
    CDistributionView *pDistributionViewT_;
    QLabel *pLabelGlassDelta_;
    qreal rEffectSize_ = 0;
    qreal dMeans_ = 0;
    qreal qEstimate_ = 0;
    qreal qEstimateSE_ = 0;
    qreal qPValue_= 1;
    qreal rVarianceU_= 1;

    QColor clrBackGroundLabel = QColor("#313131");
};

#endif // CEFFECTSTABLEGRAPH_H
