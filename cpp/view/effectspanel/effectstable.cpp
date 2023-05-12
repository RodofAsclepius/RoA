#include "effectstable.h"
#include <qheaderview.h>
#include "utils.h"
#include "effectsmodelslist.h"
#include "distributionview.h"
#include "effectstablegraph.h"
#include "colorandfontmap.h"
#include <QtMath>

#define COLUMN_COUNT 0

CEffectsTable::CEffectsTable(QWidget *parent)
    : QTableWidget(parent),
      mapColorsFonts_(CColorAndFontMap::instance())
{
    setContentsMargins(0, 0, 0, 0);
    setColumnCount(COLUMN_COUNT);

    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);

    QString strStyleSheet = styleSheet();
//    strStyleSheet += "QTableWidget::item"
//                        "{"
//                            "border-right: 1px solid #555555;"
//                        "}";
//     strStyleSheet += "QTableWidget::item:hover {"
//                            "background-color: transparent;"
//                        "}";
    setStyleSheet(strStyleSheet);

    setSelectionMode(QAbstractItemView::NoSelection);
    setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    setFocusPolicy(Qt::NoFocus);

    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
}

int CEffectsTable::columnVariable() const
{
    return iColumnVariable_;
}

void CEffectsTable::setColumnVariable(int iColumnVariable)
{
    iColumnVariable_ = iColumnVariable;
}

void CEffectsTable::updateTable()
{
//    DEBUG_OBJ_INFO

    clear();
    setColumnCount(lstEffectModels_.count() + 1);
    horizontalHeader()->setStretchLastSection(true);
    setRowCount(1);
    setRowHeight(0, height());

    /* Compute global extent for all graphs */
    QPair<qreal, qreal> pairGlobalDomain = commputeGlobalDomain();

    QStringList lstHeaderLabels;
    for(int iModelIndex = 0; iModelIndex < lstEffectModels_.count(); ++iModelIndex)
    {
        CEffectsModel effectsModel = lstEffectModels_.at(iModelIndex);
        lstHeaderLabels << effectsModel.group().name();

        QTableWidgetItem *pItemEffect = new QTableWidgetItem();
        setItem(0, iModelIndex, pItemEffect);

        CEffectsTableGraph* pGraph = new CEffectsTableGraph();
        pGraph->setGlobalDomainViolinPlot(pairGlobalDomain);
        pGraph->setUseGlobalDomainViolinPlot(true);

        pGraph->setREffectSize(effectsModel.getProperty<double>("effectSize"));
        pGraph->setQEstimateSE(effectsModel.getProperty<double>("SE"));
        pGraph->setPValue(effectsModel.getProperty<double>("P"));
        pGraph->setDeltaMeans(effectsModel.getProperty<double>("D"));
        pGraph->setVarianceU(effectsModel.getProperty<double>("varianceU"));
        setCellWidget(0, iModelIndex, pGraph);

        CDistributionsList lstDistributionsU;
        lstDistributionsU.append(effectsModel.distributionU());
        pGraph->setDistributionU(lstDistributionsU);

        CDistributionsList lstDistributionsT;
        lstDistributionsT.append(effectsModel.distributionT());
        pGraph->setDistributionT(lstDistributionsT);

        pGraph->updateGraph();

        horizontalHeader()->resizeSection(iModelIndex, COLUMN_DISTRIBUTION_U_WIDTH * 2);
    }

    lstHeaderLabels.append("");

    setHorizontalHeaderLabels(lstHeaderLabels);
    resize(horizontalHeader()->width(), height());
}

QPair<qreal, qreal> CEffectsTable::commputeGlobalDomain() const
{
    QPair<qreal, qreal> pairDomain = qMakePair<qreal, qreal>(99999, -99999);

    foreach(CEffectsModel effectsModel, lstEffectModels_)
    {
        CDistribution distU = effectsModel.distributionU();
        CDistribution distT = effectsModel.distributionT();

        QPair<qreal, qreal> violinDomainU = distU.violinDomain();

        pairDomain = QPair<qreal, qreal>(qMin(pairDomain.first, violinDomainU.first),
                                         qMax(pairDomain.second, violinDomainU.second));

        QPair<qreal, qreal> violinDomainT = distT.violinDomain();
        pairDomain = QPair<qreal, qreal>(qMin(pairDomain.first, violinDomainT.first),
                                         qMax(pairDomain.second, violinDomainT.second));
    }

    pairDomain.first = qFloor(pairDomain.first);
    pairDomain.second = qCeil(pairDomain.second + 2);
    return pairDomain;
}

CEffectsModelsList CEffectsTable::effectModels() const
{
    return lstEffectModels_;
}

void CEffectsTable::setEffectModels(const CEffectsModelsList &lstEffectModels)
{
    lstEffectModels_ = lstEffectModels;

    updateTable();
}
