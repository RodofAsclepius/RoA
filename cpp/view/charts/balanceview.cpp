#include "balanceview.h"
#include "colorandfontmap.h"
#include "utils.h"
#include "math.h"
#include "extendedgraphicsview.h"
#include "balanceviewheader.h"

#include <QPainter>
#include <QGridLayout>
#include <QGraphicsTextItem>
#include <QScrollBar>
#include <QRandomGenerator>

#define CHART_MARGIN_LEFT 6
#define CHART_MARGIN_TOP 6
#define CHART_MARGIN_RIGHT 6
#define CHART_MARGIN_BOTTOM 6

#define LABEL_MARGIN_LEFT 24
#define LABEL_MARGIN_TOP 12
#define LABEL_MARGIN_RIGHT 24
#define LABEL_MARGIN_BOTTOM 12

using namespace utils;

CBalanceView::CBalanceView(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f),
      pHeader_(new CBalanceViewHeader()),
        mapsColorAndFonts_(CColorAndFontMap::instance()),
        pView_(new CExtendedGraphicsView())
{
    setContentsMargins(0, 0, 0 ,0);
    marPlot_ = QMargins(CHART_MARGIN_LEFT, CHART_MARGIN_TOP, CHART_MARGIN_RIGHT, CHART_MARGIN_BOTTOM);
    marLabels_ = QMargins(LABEL_MARGIN_LEFT, LABEL_MARGIN_TOP, LABEL_MARGIN_RIGHT, LABEL_MARGIN_BOTTOM);

    connect(&mapsColorAndFonts_, &CColorAndFontMap::updated, this, &CBalanceView::onColorAndFontMapUpdated);
    applyStyleSheet();

    rCellOffset_ = ceil(iRowHeight_ / 2);
    iLevelLabelOffsetHorizontal_ = LABEL_MARGIN_LEFT;

    QGridLayout *pGridLayout = new QGridLayout();
    pGridLayout->setContentsMargins(0, 0, 0, 0);

    pView_->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    pView_->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    pGridLayout->addWidget(pView_, 0, 0, 1, 1);
    setLayout(pGridLayout);

    setDrawBorder(false);
    setMinimumWidth(120);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

//    connect(pView_, &QGraphicsView::mousePressEvent, this, &CDistributionView::onViewMousePressEvent);
}

CBalanceView::~CBalanceView()
{

}

void CBalanceView::render(bool bUpdateGeometry)
{
//    DEBUG_OBJ_INFO

    QRect rectViewPort = computeViewPortRect();
    pView_->scene()->clear();
    pView_->setTransformationAnchor(QGraphicsView::NoAnchor);
    pView_->setRenderHint(QPainter::Antialiasing, true);
    rectViewPort.setHeight(iTotalSceneHeight_);
    pView_->scene()->setSceneRect(rectViewPort);

    renderLovePlotCurves(bUpdateGeometry);
}

void CBalanceView::renderLovePlotCurves(bool bUpdateGeometry)
{
//    DEBUG_OBJ_INFO

    if(!geometryCacheReadyLovePlot() || bUpdateGeometry)
    {
        updateGeometryCacheLovePlot();
    }

    QRect rectView = computeViewPortRect();
    QGraphicsScene *pScene = pView_->scene();

    /* Fill background */
    pScene->setBackgroundBrush(clrBackground_);

    if(bDrawBorder)
    {
        QRect rectBorder = rectView;
        pScene->addRect(rectBorder);
    }

    /* Plot background */
    QRect rectPlot = computePlotRectLovePlot();
    QPen penPlotBackground(Qt::transparent);
    if(bDrawBorderPlot)
    {
        penPlotBackground.setColor(Qt::black);
    }

    /* Outer */
    QColor clrOuter = Qt::lightGray;
    QPen penTresholdOuter(clrOuter, 1, Qt::PenStyle::SolidLine);
    pScene->addLine(scaleXLovePlot(-rOuterTreshold_), 0, scaleXLovePlot(-rOuterTreshold_), pScene->height(), penTresholdOuter);
    pScene->addLine(scaleXLovePlot(rOuterTreshold_), 0, scaleXLovePlot(rOuterTreshold_), pScene->height(), penTresholdOuter);

    /* Innter treshold lines */
    QColor clrInner = Qt::white;
    QPen penTresholdInner(clrInner, 1);
    QBrush brushRectInner(clrInner, Qt::BrushStyle::SolidPattern);
    pScene->addRect(scaleXLovePlot(-rInnerTreshold_), 0,
                    scaleXLovePlot(rInnerTreshold_)-scaleXLovePlot(-rInnerTreshold_), pScene->height(), penTresholdInner, brushRectInner);

    /* Zero */
    QPen penTresholZero(Qt::black, 1, Qt::PenStyle::SolidLine);
    pScene->addLine(scaleXLovePlot(0), 0, scaleXLovePlot(0), pScene->height(), penTresholZero);

    /* Horizontal lines */
    QColor clrGridLine = QColor("#555555");
    QPen penHorizontalLines(clrGridLine, 1, Qt::PenStyle::SolidLine);

    int iIndex=0;

    for(int iHeight=0; iHeight<pScene->height(); iHeight +=iRowHeight_, iIndex++)
    {
        int iTotalHeight = iHeight;
        pScene->addLine(0, iTotalHeight, width(), iTotalHeight, penHorizontalLines);
    }

    QMap<QString, qreal> mapTickLabels;
    mapTickLabels[QString::number(-rInnerTreshold_, 'f', 2)] = scaleXLovePlot(-rInnerTreshold_);
    mapTickLabels[QString::number(rInnerTreshold_, 'f', 2)] = scaleXLovePlot(rInnerTreshold_);
    mapTickLabels[QString::number(-rOuterTreshold_, 'f', 2)] = scaleXLovePlot(-rOuterTreshold_);
    mapTickLabels[QString::number(rOuterTreshold_, 'f', 2)] = scaleXLovePlot(rOuterTreshold_);
    mapTickLabels[QString::number(4.0*-rOuterTreshold_, 'f', 2)] = scaleXLovePlot(4*-rOuterTreshold_);
    mapTickLabels[QString::number(4.0*rOuterTreshold_, 'f', 2)] = scaleXLovePlot(4*rOuterTreshold_);
    pHeader_->setTicksLabels(mapTickLabels);

    bool bContentFound = false;
    QPen penPath(clrPenDefaultLovePlot_, iDensityPlotWidth);
    QBrush brushPath;

    if(bShowUnadjustedValues_)
    {
        /* Unadjusted curves */
        QMapIterator<CGroup, QPainterPath> mapGroupPathUn(mapLoveLinesUn_);
        while(mapGroupPathUn.hasNext())
        {
            mapGroupPathUn.next();

            CGroup group = mapGroupPathUn.key();
            QPainterPath path = mapGroupPathUn.value();
            penPath.setColor(group.colorSecondary());
            penPath.setStyle(Qt::PenStyle::DashLine);
            brushPath.setColor(group.colorSecondary());
            QGraphicsPathItem* pItem = pScene->addPath(path, penPath, brushPath);
            bContentFound = true;
        }

        /* Unadjusted points */
        QMapIterator<CGroup, QVector<QPointF>> mapItAdj(mapGroupCoordinatesUn_);
        while(mapItAdj.hasNext())
        {
            mapItAdj.next();

            CGroup group = mapItAdj.key();
            QVector<QPointF> vecPoints = mapItAdj.value();

            QVector<QVariantMap> vecVariantData = mapGroupCoordinatesDataAdj_.value(group);
            for(int iVecIndex=0; iVecIndex < vecPoints.count(); ++iVecIndex)
            {
                QPointF point = vecPoints.at(iVecIndex);
                qreal rDistance = abs(point.x() - computeCenterLovePlot().x());
                qreal rPointScale = log(rDistance);
                qreal rPointSize = rPointScale * 5;
                rPointSize = qMax(1.0, qMin(rPointSize, 25.0));

                QPen penPoint = QPen(group.colorPrimary(), 1.5);
                penPoint.setStyle(Qt::PenStyle::DashLine);
                QColor clrPoint = group.colorPrimary();
                clrPoint.setAlpha(200);
                QBrush brushPoint = QBrush(Qt::transparent);
                QGraphicsEllipseItem *pEllipseItem = pScene->addEllipse(point.rx() - rPointSize/2.0,
                                                                        point.ry() - rPointSize/2.0,
                                                                        rPointSize, rPointSize,
                                                                        penPoint, brushPoint);
                pEllipseItem->setFlag(QGraphicsItem::ItemIsFocusable, true);
                pEllipseItem->setAcceptHoverEvents(true);
                pEllipseItem->setZValue(300);

                QVariantMap varData = vecVariantData.at(iVecIndex);
                QString strTooltip = "Value: " + varData.value("value").toString();
//                        + "\n" +
//                                     "M.0.Un" + varData.value("M.0.Un").toString() + "\n" +
//                                     "M.1.Un" + varData.value("M.1.Un").toString() + "\n" +
//                                     "V.Ratio.Un" + varData.value("V.Ratio.Un").toString();
                pEllipseItem->setToolTip(strTooltip);
            }
        }
    }

    /* Adjusted curves */
    QMapIterator<CGroup, QPainterPath> mapGroupPathAdj(mapLoveLinesAdj_);
    while(mapGroupPathAdj.hasNext())
    {
        mapGroupPathAdj.next();

        CGroup group = mapGroupPathAdj.key();
        QColor c = group.colorSecondary();
        QPainterPath path = mapGroupPathAdj.value();
        penPath.setColor(c);
        penPath.setStyle(Qt::PenStyle::SolidLine);
        brushPath.setColor(c);
        QGraphicsPathItem* pItem = pScene->addPath(path, penPath, brushPath);
        bContentFound = true;
    }

    /* Adjusted points */
    QMapIterator<CGroup, QVector<QPointF>> mapItAdj(mapGroupCoordinatesAdj_);
    while(mapItAdj.hasNext())
    {
        mapItAdj.next();

        CGroup group = mapItAdj.key();
        QVector<QPointF> vecPoints = mapItAdj.value();

        QVector<QVariantMap> vecVariantData = mapGroupCoordinatesDataAdj_.value(group);
        for(int iVecIndex=0; iVecIndex < vecPoints.count(); ++iVecIndex)
        {
            QPointF point = vecPoints.at(iVecIndex);
            qreal rDistance = abs(point.x() - computeCenterLovePlot().x());
            qreal rPointScale = log(rDistance);
            qreal rPointSize = rPointScale * 5;
            rPointSize = qMax(1.0, qMin(rPointSize, 25.0));

            QPen penPoint = QPen(Qt::black, 1.5);
//            QPen penPoint = QPen(Qt::darkg, 1.5);
            QColor clrPoint = group.colorPrimary();
            clrPoint.setAlpha(200);
            QBrush brushPoint = QBrush(clrPoint);
            QGraphicsEllipseItem *pEllipseItem = pScene->addEllipse(point.rx() - rPointSize/2.0,
                                                                    point.ry() - rPointSize/2.0,
                                                                    rPointSize, rPointSize,
                                                                    penPoint, brushPoint);
            pEllipseItem->setFlag(QGraphicsItem::ItemIsFocusable, true);
            pEllipseItem->setAcceptHoverEvents(true);
            pEllipseItem->setZValue(300);

            QVariantMap varData = vecVariantData.at(iVecIndex);
            QString strTooltip = "Value: " + varData.value("value").toString();
//                                 + "\n" +
//                                 "M.0.Adj" + varData.value("M.0.Adj").toString() + "\n" +
//                                 "M.1.Adj" + varData.value("M.1.Adj").toString() + "\n" +
//                                 "V.Ratio.Adj" + varData.value("V.Ratio.Adj").toString();
            pEllipseItem->setToolTip(strTooltip);
        }
    }
}

qreal CBalanceView::scaleXLovePlot(qreal rX) const
{
//    DEBUG_OBJ_INFO

    qreal rScaledX = 0;
    qreal rCenterX = computeCenterLovePlot().x();
    qreal rHorizontalCapValue = .68 * (static_cast<qreal>(width()) / 2.0);
    qreal frac = rX / rHorizontalAxisMax_;
    qreal plotX = frac * rHorizontalCapValue;
    rScaledX = rCenterX + plotX;

    /* Cap */
    rScaledX = qMin(rScaledX, rCenterX + rHorizontalCapValue);
    rScaledX = qMax(rScaledX, rCenterX - rHorizontalCapValue);

    return rScaledX;
}

void CBalanceView::clear()
{
    lstBalanceModels_.clear();
    updateView(true);
}

void CBalanceView::updateView(bool bUpdateGeometry)
{
//    DEBUG_OBJ_INFO

    render(bUpdateGeometry);
}

CBalanceView::ScalingMode CBalanceView::scalingModeHorizontal() const
{
    return scalingModeHorizontal_;
}

void CBalanceView::setScalingModeHorizontal(const ScalingMode &value)
{
    scalingModeHorizontal_ = value;
}

CBalanceView::ScalingMode CBalanceView::scalingModeVertical() const
{
    return scalingModeVertical_;
}

void CBalanceView::setScalingModeVertical(const ScalingMode &scalingModeVertical)
{
    scalingModeVertical_ = scalingModeVertical;
}

void CBalanceView::applyStyleSheet()
{
    /* Love plot*/
    clrAxisLovePlot_ = Qt::black;
    clrPenDefaultLovePlot_ = Qt::black;

    QString strStyleSheet = styleSheet();
    strStyleSheet += "QWidget{ border: none; }";
    setStyleSheet(strStyleSheet);
}

bool CBalanceView::drawBorder() const
{
    return bDrawBorder;
}

void CBalanceView::setDrawBorder(bool value)
{
    bDrawBorder = value;
}

bool CBalanceView::drawBorderPlot() const
{
    return bDrawBorderPlot;
}

void CBalanceView::setDrawBorderPlot(bool value)
{
    bDrawBorderPlot = value;
}

bool CBalanceView::drawAxes() const
{
    return bDrawAxes;
}

void CBalanceView::setDrawAxes(bool value)
{
    bDrawAxes = value;
}

QRect CBalanceView::computeViewPortRect() const
{
    QMargins margins = contentsMargins();
    QRect rectView = rect();
    rectView.adjust(margins.left(), margins.top(), -margins.right(), -margins.bottom());

    return rectView;
}

QSize CBalanceView::computeAxesLovePlot() const
{
    QSize size;

    switch(orientationModeLovePlot_)
    {
    case North:
    {
        size = QSize(computePlotRectLovePlot().width(), computePlotRectLovePlot().height());
    }
        break;
        case East:
        {
            size = QSize(computePlotRectLovePlot().height(), computePlotRectLovePlot().width());
        }
        break;
        case South:
        {
            size = QSize(computePlotRectLovePlot().width(), computePlotRectLovePlot().height());
        }
        break;
        case West:
        {
            size = QSize(computePlotRectLovePlot().height(), computePlotRectLovePlot().width());
        }
        break;
    }

    return size;
}

QPoint CBalanceView::computeCenterLovePlot() const
{
    QPoint pntCenter = computeOriginLovePlot();

    switch(orientationModeLovePlot_)
    {
        case North:
        {
            pntCenter.setX(pntCenter.x() + computeAxesLovePlot().width() / 2);
            pntCenter.setY(pntCenter.y() - computeAxesLovePlot().height() / 2);
        }
        break;
        case East:
        {
            pntCenter.setX(pntCenter.x() + computeAxesLovePlot().height() / 2);
            pntCenter.setY(pntCenter.y() - computeAxesLovePlot().width() / 2);
        }
        break;
        case South:
        {
            pntCenter.setX(pntCenter.x() + computeAxesLovePlot().width() / 2);
            pntCenter.setY(pntCenter.y() - computeAxesLovePlot().height() / 2);
        }
        break;
        case West:
        {
            pntCenter.setX(pntCenter.x() + computeAxesLovePlot().height() / 2);
            pntCenter.setY(pntCenter.y() - computeAxesLovePlot().width() / 2);
        }
        break;
    }

    return pntCenter;
}

QRect CBalanceView::computePlotRectLovePlot() const
{
    QRect rectView = computeViewPortRect();
    rectView.adjust(marPlot_.left(), marPlot_.top(), -marPlot_.right(), -marPlot_.bottom());

    switch(orientationModeLovePlot_)
    {
        case North:
            rectView.adjust(0, 0, 0, -marLabels_.bottom());
        break;
        case East:
            rectView.adjust(marLabels_.left(), 0, 0, 0);
        break;
        case South:
            rectView.adjust(0, marLabels_.top(), 0, 0);
        break;
        case West:
            rectView.adjust(0, 0, -marLabels_.right(), 0);
        break;
    }

    return rectView;
}

QPoint CBalanceView::computeOriginLovePlot() const
{
    QPoint pnt;

    switch(orientationModeLovePlot_)
    {
        case North:
        {
            pnt = computePlotRectLovePlot().bottomLeft();
        }
        break;
        case East:
        {
            pnt = computePlotRectLovePlot().topLeft();
        }
        break;
        case South:
        {
            pnt = computePlotRectLovePlot().topLeft();
        }
        break;
        case West:
        {
            pnt = computePlotRectLovePlot().topRight();
        }
        break;
    }

    return pnt;
}

void CBalanceView::resizeEvent(QResizeEvent *event)
{
    render(true);
}

void CBalanceView::mouseMoveEvent(QMouseEvent *event)
{
    DEBUG_OBJ_INFO
    setFocus();
    QWidget::mouseMoveEvent(event);
}

bool CBalanceView::geometryCacheReadyLovePlot() const
{
    return bGeometryCacheReadyLovePlot_;
}

void CBalanceView::setGeometryCacheReadyLovePlot(bool bGeometryCacheReadyLovePlot)
{
    bGeometryCacheReadyLovePlot_ = bGeometryCacheReadyLovePlot;
}

CBalanceView::OrientationMode CBalanceView::orientationModeLovePlot() const
{
    return orientationModeLovePlot_;
}

void CBalanceView::setOrientationModeLovePlot(const OrientationMode &orientationModeLovePlot)
{
    orientationModeLovePlot_ = orientationModeLovePlot;
}

QPair<CVariablesList, QVector<QStringList> > CBalanceView::variablesAndLevels() const
{
    return vecVariablesAndLevels_;
}

void CBalanceView::setVariablesAndLevels(const QPair<CVariablesList, QVector<QStringList> > &vecVariablesAndLevels)
{
    vecVariablesAndLevels_ = vecVariablesAndLevels;
}

bool CBalanceView::showUnadjustedValues() const
{
    return bShowUnadjustedValues_;
}

void CBalanceView::setShowUnadjustedValues(bool bShowUnadjustedValues)
{
    if(bShowUnadjustedValues_ != bShowUnadjustedValues)
    {
        bShowUnadjustedValues_ = bShowUnadjustedValues;
        render();
    }
}

int CBalanceView::rowHeight() const
{
    return iRowHeight_;
}

void CBalanceView::setRowHeight(int iRowHeight)
{
    iRowHeight_ = iRowHeight;
    rCellOffset_ = iRowHeight / 2;
}

void CBalanceView::generateLoveCurves()
{
//    DEBUG_OBJ_INFO

    int iLineYShift = 0;
    iLineYShift = 0;
    //    mapSplinesUn_.clear();
    //    mapSplinesAdj_.clear();
    mapGroupCoordinatesDataUn_.clear();
    mapGroupCoordinatesDataAdj_.clear();
    mapLoveLinesUn_.clear();
    mapLoveLinesAdj_.clear();

    if(vecVariablesAndLevels_.first.isEmpty())
    {
        return;
    }

    rHorizontalAxisMax_ = 0;
    qreal rTotalHeight = 0;
    foreach(CBalanceModel balanceModel, lstBalanceModels_)
    {
        CVariableBalanceList lstVariablesBalanced = balanceModel.variablesBalanced();
        qreal x = 0, y = 0;

        /* Loop sorted variables */
        int iIndex = 0;
        QVector<QPointF> vecPointsUn;
        QVector<QPointF> vecPointsAdj;
        QVector<QMap<QString, QVariant>> vecDataUn;
        QVector<QMap<QString, QVariant>> vecDataAdj;

        for(int iVariables=0; iVariables<vecVariablesAndLevels_.first.count(); ++iVariables)
        {
            CVariable variable = vecVariablesAndLevels_.first.at(iVariables);
            QStringList lstStringsLevelsVisible = vecVariablesAndLevels_.second.at(iVariables);
            QString strVariableName = variable.name();
            CVariableBalance variableBalance = lstVariablesBalanced.byName(strVariableName);

            /* Todo fix empty variable found */
            if(variable.levels().count() < 2)
            {
                /* We have a numeric / integer variable */
                CVariableBalanceInfo balanceInfo = variableBalance.balanceInfo();
                QMap<QString, QMap<QString, qreal>> mapLevelValues = balanceInfo.levelValues();

                if(!mapLevelValues.isEmpty())
                {
                    QMap<QString, qreal> balanceValues = mapLevelValues.first();

                    qreal rDiffUn = balanceValues.value("Diff.Un");
                    qreal rDiffAdj = balanceValues.value("Diff.Adj");

                    /* Get most extreme value */
                    foreach(QString strLevel, variable.levels())
                    {
                        QMap<QString, qreal> balanceValuesCurrentLevel = mapLevelValues[strLevel];

                        qreal rDiffUnCurrentLevel = balanceValuesCurrentLevel.value("Diff.Un");
                        qreal rDiffAdjCurrentLevel = balanceValuesCurrentLevel.value("Diff.Adj");

                        if(abs(rDiffUnCurrentLevel) > abs((rDiffUn)))
                        {
                            rDiffUn = rDiffUnCurrentLevel;
                        }

                        if(abs(rDiffAdjCurrentLevel) > abs((rDiffAdj)))
                        {
                            rDiffAdj = rDiffAdjCurrentLevel;
                        }
                    }

                    /* abs ok here? */
//                    rHorizontalAxisMax_ = qMax(abs(rDiffAdj), qMax(abs(rDiffUn), qAbs(rHorizontalAxisMax_)));
                    rHorizontalAxisMax_ = 0.5;

                    /* Unadjusted */
                    rDiffUn = balanceValues.value("Diff.Un");
                    x = scaleXLovePlot(rDiffUn);
                    y = rCellOffset_ + iLineYShift + (iIndex * (iRowHeight_));
                    vecPointsUn << QPointF(x, y);
                    /* Data */
                    QMap<QString, QVariant> mapDataUn;
                    mapDataUn["value"] = rDiffUn;
                    mapDataUn["M.0.Un"] = balanceValues.value("M.0.Un");
                    mapDataUn["M.1.Un"] = balanceValues.value("M.1.Un");
                    mapDataUn["V.Ratio.Un"] = balanceValues.value("V.Ratio.Un");
                    vecDataUn.append(mapDataUn);

                    /* Adjusted */
                    rDiffAdj = balanceValues.value("Diff.Adj");
                    x = scaleXLovePlot(rDiffAdj);
                    y = rCellOffset_ + iLineYShift + (iIndex * (iRowHeight_));
                    vecPointsAdj << QPointF(x, y);
                    /* Data */
                    QMap<QString, QVariant> mapDataAdj;
                    mapDataAdj["value"] = rDiffAdj;
                    mapDataAdj["M.0.Adj"] = balanceValues.value("M.0.Adj");
                    mapDataAdj["M.1.Adj"] = balanceValues.value("M.1.Adj");
                    mapDataAdj["V.Ratio.Adj"] = balanceValues.value("V.Ratio.Adj");
                    vecDataAdj.append(mapDataAdj);
                }

                ++iIndex;
            }
            else
            {
                /* We have >=2 variable levels */
                CVariableBalanceInfo balanceInfo = variableBalance.balanceInfo();
                QMap<QString, QMap<QString, qreal>> mapLevelValues = balanceInfo.levelValues();
//                lstStringsLevelsVisible = mapLevelValues.keys(); /* override */

                if(!mapLevelValues.isEmpty())
                {
                    /* Add toplevel */
                    QMap<QString, qreal> balanceValues = mapLevelValues.first();

                    qreal rDiffUn = balanceValues.value("Diff.Un");
                    qreal rDiffAdj = balanceValues.value("Diff.Adj");

                    /* Get most extreme value */
                    foreach(QString strLevel, variable.levels())
                    {
                        QMap<QString, qreal> balanceValuesCurrentLevel = mapLevelValues[strLevel];

                        qreal rDiffUnCurrentLevel = balanceValuesCurrentLevel.value("Diff.Un");
                        qreal rDiffAdjCurrentLevel = balanceValuesCurrentLevel.value("Diff.Adj");

                        if(abs(rDiffUnCurrentLevel) > abs((rDiffUn)))
                        {
                            rDiffUn = rDiffUnCurrentLevel;
                        }

                        if(abs(rDiffAdjCurrentLevel) > abs((rDiffAdj)))
                        {
                            rDiffAdj = rDiffAdjCurrentLevel;
                        }
                    }

                    /* Unadjusted */
                    x = scaleXLovePlot(rDiffUn);
                    y = rCellOffset_ + iLineYShift + (iIndex * (iRowHeight_));
                    vecPointsUn << QPointF(x, y);
                    /* Data */
                    QMap<QString, QVariant> mapDataUn;
                    mapDataUn["value"] = rDiffUn;
                    mapDataUn["M.0.Un"] = balanceValues.value("M.0.Un");
                    mapDataUn["M.1.Un"] = balanceValues.value("M.1.Un");
                    mapDataUn["V.Ratio.Un"] = balanceValues.value("V.Ratio.Un");
                    vecDataUn.append(mapDataUn);

                    /* Adjusted */
                    x = scaleXLovePlot(rDiffAdj);
                    y = rCellOffset_ + iLineYShift + (iIndex * (iRowHeight_));
                    vecPointsAdj << QPointF(x, y);
                    /* Data */
                    QMap<QString, QVariant> mapDataAdj;
                    mapDataAdj["value"] = rDiffAdj;
                    mapDataAdj["M.0.Adj"] = balanceValues.value("M.0.Adj");
                    mapDataAdj["M.1.Adj"] = balanceValues.value("M.1.Adj");
                    mapDataAdj["V.Ratio.Adj"] = balanceValues.value("V.Ratio.Adj");
                    vecDataAdj.append(mapDataAdj);

                    ++iIndex;

                    if(lstStringsLevelsVisible.count() > 1)
                    {
                        foreach(QString strLevel, lstStringsLevelsVisible)
                        {
                            /* Add levels */
                            if(balanceInfo.levelValues().keys().contains(strLevel))
                            {
                                QMap<QString, qreal> balanceValues = mapLevelValues[strLevel];
                                if(!balanceValues.isEmpty())
                                {
                                    /* Unadjusted */
                                    qreal rDiffUn = balanceValues.value("Diff.Un");
                                    x = scaleXLovePlot(rDiffUn);
                                    y = rCellOffset_ + iLineYShift + (iIndex * (iRowHeight_));
                                    vecPointsUn << QPointF(x, y);
                                    /* Data */
                                    QMap<QString, QVariant> mapDataUn;
                                    mapDataUn["M.0.Un"] = balanceValues.value("M.0.Un");
                                    mapDataUn["M.1.Un"] = balanceValues.value("M.1.Un");
                                    mapDataUn["V.Ratio.Un"] = balanceValues.value("V.Ratio.Un");
                                    vecDataUn.append(mapDataUn);

                                    /* Adjusted */
                                    qreal rDiffAdj = balanceValues.value("Diff.Adj");
                                    x = scaleXLovePlot(rDiffAdj);
                                    y = rCellOffset_ + iLineYShift + (iIndex * (iRowHeight_));
                                    vecPointsAdj << QPointF(x, y);
                                    QMap<QString, QVariant> mapDataAdj;
                                    mapDataAdj["M.0.Adj"] = balanceValues.value("M.0.Adj");
                                    mapDataAdj["M.1.Adj"] = balanceValues.value("M.1.Adj");
                                    mapDataAdj["V.Ratio.Adj"] = balanceValues.value("V.Ratio.Adj");
                                    vecDataAdj.append(mapDataAdj);
                                }
                                else
                                {
                                    /* No balance values for this level */
                                }
                            }
                            else
                            {
                                /* No information for this level */
                            }
                            ++iIndex;
                        } // foreach level
                    }
                } // Balance info empty
                else
                {
                    /* We have empty balance values */
                }
            } // factor or not

//            ++iIndex;
        } // foreach variable

        /* Store values */
        mapGroupCoordinatesUn_[balanceModel.group()] = vecPointsUn;
        mapGroupCoordinatesAdj_[balanceModel.group()] = vecPointsAdj;
        mapGroupCoordinatesDataUn_[balanceModel.group()] = vecDataUn;
        mapGroupCoordinatesDataAdj_[balanceModel.group()] = vecDataAdj;

        /* Generate painter paths un */
        QMapIterator<CGroup, QVector<QPointF>> mapIt(mapGroupCoordinatesUn_);
        while(mapIt.hasNext())
        {
            mapIt.next();

            CGroup group = mapIt.key();
            QVector<QPointF> vecPoints = mapIt.value();
            QPainterPath pathUn;
            QPainterPath pahtDotUn;
            foreach(QPointF point, vecPoints)
            {
                if(pathUn.isEmpty())
                {
                    pathUn.moveTo(point);
                }
                else
                {
                    QPointF pntCur = pathUn.currentPosition();
                    pathUn.cubicTo(pntCur + QPointF(0, rowHeight()/2), point + QPointF(0, -rowHeight()/2), point);
                }

                pathUn.addEllipse(point, 1.0, 1.0);
//                qreal rDistance = abs(point.x() - computeCenterLovePlot().x());
//                qreal rPointScale = 2 * .5 + (rDistance / (0.68 * width()/2.0));
//                pahtDotUn.addEllipse(point, rPointScale * 6, rPointScale * 6);

                rTotalHeight = qMax(rTotalHeight, point.y());
            }
            mapLoveLinesUn_.insert(group, pathUn);
            mapLoveDotsUn_.insert(group, pahtDotUn);
        }

        /* Generate painter paths adj */
        QMapIterator<CGroup, QVector<QPointF>> mapItAdj(mapGroupCoordinatesAdj_);
        while(mapItAdj.hasNext())
        {
            mapItAdj.next();

            CGroup group = mapItAdj.key();
            QVector<QPointF> vecPoints = mapItAdj.value();
            QPainterPath pathAdj;
            QPainterPath pahtDotAdj;
            foreach(QPointF point, vecPoints)
            {
                if(pathAdj.isEmpty())
                {
                    pathAdj.moveTo(point);

                }
                else
                {
                    QPointF pntCur = pathAdj.currentPosition();
                    pathAdj.cubicTo(pntCur + QPointF(0, rowHeight()/2), point + QPointF(0, -rowHeight()/2), point);
                }

//                qreal rDistance = abs(point.x() - computeCenterLovePlot().x());
//                qreal rPointScale = 2* .5 + (rDistance / (0.68 * width()/2.0));
//                pahtDotAdj.addEllipse(point, rPointScale * 6, rPointScale * 6);
                pathAdj.addEllipse(point, 1.0, 1.0);

                rTotalHeight = qMax(rTotalHeight, point.y());
            }
            mapLoveLinesAdj_.insert(group, pathAdj);
            mapLoveDotsAdj_.insert(group, pahtDotAdj);
        }

    } // foreach model

    iTotalSceneHeight_ = rTotalHeight + rowHeight()/2;

    /* Adjust height */
    QRect rectViewPort = computeViewPortRect();
    rectViewPort.setHeight(iTotalSceneHeight_);
    pView_->scene()->setSceneRect(rectViewPort);
}

CExtendedGraphicsView *CBalanceView::pView() const
{
    return pView_;
}

void CBalanceView::setPView(CExtendedGraphicsView *pView)
{
    pView_ = pView;
}

bool CBalanceView::getLogarithmicScaling() const
{
    return bLogarithmicScaling;
}

void CBalanceView::setLogarithmicScaling(bool value)
{
    bLogarithmicScaling = value;
}

CBalanceViewHeader *CBalanceView::header() const
{
    return pHeader_;
}

void CBalanceView::translateY(qreal ry)
{
    //    iTranslationY_ = ry;
    pView_->verticalScrollBar()->setValue(ry);

    //    pView_->scroll(0, ry);
    //    pView_->translate()
}

CBalanceModelsList CBalanceView::balanceModels() const
{
    return lstBalanceModels_;
}

void CBalanceView::setBalanceModels(const CBalanceModelsList &lstBalanceModels)
{
//    DEBUG_OBJ_INFO

    lstBalanceModels_ = lstBalanceModels;
}

void CBalanceView::clearGeometryCache()
{
    clearGeometryCacheLovePlot();
}

void CBalanceView::updateGeometryCacheLovePlot()
{
    clearGeometryCacheLovePlot();

    generateLoveCurves();
    setGeometryCacheReadyLovePlot(true);
}


void CBalanceView::clearGeometryCacheLovePlot()
{
    QMutexLocker mutLocker(&mutex_);

    mapGroupCoordinatesUn_.clear();
    mapGroupCoordinatesAdj_.clear();
    mapGroupCoordinatesDataUn_.clear();
    mapGroupCoordinatesDataAdj_.clear();
    mapLoveDotsUn_.clear();
    mapLoveDotsAdj_.clear();
    mapLoveGridLines_.clear();

//    emit chartChangedEvent();
}

bool CBalanceView::drawValueLabels() const
{
    return bDrawValueLabels;
}

void CBalanceView::setDrawValueLabels(bool value)
{
    bDrawValueLabels = value;
}

void CBalanceView::onColorAndFontMapUpdated()
{
    applyStyleSheet();
    repaint();
}

void CBalanceView::onViewMousePressEvent(QMouseEvent *event)
{

}


