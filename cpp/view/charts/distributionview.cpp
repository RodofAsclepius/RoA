#include "distributionview.h"
#include "colorandfontmap.h"
#include "utils.h"
#include "math.h"
#include "extendedgraphicsview.h"
#include <QTreeWidgetItem>

#include <QPainter>
#include <QGridLayout>
#include <QGraphicsTextItem>

#define CHART_MARGIN_LEFT 6
#define CHART_MARGIN_TOP 6
#define CHART_MARGIN_RIGHT 6
#define CHART_MARGIN_BOTTOM 36

#define LABEL_MARGIN_LEFT 12
#define LABEL_MARGIN_TOP 12
#define LABEL_MARGIN_RIGHT 24
#define LABEL_MARGIN_BOTTOM 12

using namespace utils;

CDistributionView::CDistributionView(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f),
        mapsColorAndFonts_(CColorAndFontMap::instance()),
        pView_(new CExtendedGraphicsView())
{
    setContentsMargins(0, 0, 0 ,0);
    marPlot_ = QMargins(CHART_MARGIN_LEFT, CHART_MARGIN_TOP, CHART_MARGIN_RIGHT, CHART_MARGIN_BOTTOM);
    marLabels_ = QMargins(LABEL_MARGIN_LEFT, LABEL_MARGIN_TOP, LABEL_MARGIN_RIGHT, LABEL_MARGIN_BOTTOM);

    connect(&mapsColorAndFonts_, &CColorAndFontMap::updated, this, &CDistributionView::onColorAndFontMapUpdated);
    applyStyleSheet();

    iLevelLabelOffsetHorizontal_ = LABEL_MARGIN_LEFT;
    iLevelLabelOffsetVerticalViolinPlot_ = LABEL_MARGIN_BOTTOM;

    QGridLayout *pGridLayout = new QGridLayout();
    pGridLayout->setContentsMargins(0, 0, 0, 0);
    pView_->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    pView_->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    pGridLayout->addWidget(pView_);
    setLayout(pGridLayout);

//    connect(pView_, &QGraphicsView::mousePressEvent, this, &CDistributionView::onViewMousePressEvent);
}

CDistributionView::~CDistributionView()
{

}

void CDistributionView::render(bool bUpdateGeometry)
{
    QRect rectViewPort = computeViewPortRect();
    pView_->scene()->setSceneRect(rectViewPort);
    pView_->setRenderHint(QPainter::Antialiasing, true);
    pView_->scene()->clear();

    switch(viewMode_)
    {
    case Histogram:
        renderHistogram(bUpdateGeometry);
        break;
    case Densityplot:
        renderDensityPlot(bUpdateGeometry);
        break;
    case ViolinPlot:
        renderViolinPlot(bUpdateGeometry);
        break;
    }
}

void CDistributionView::renderHistogram(bool bUpdateGeometry)
{
//    DEBUG_OBJ_INFO

    if(!geometryCacheReadyHistogram() || bUpdateGeometry)
    {
        updateGeometryCacheHistogram();
    }

    QRect rectView = computeViewPortRect();
    QGraphicsScene *pScene = pView_->scene();

    /* Fill background */

//    QTreeWidgetItem* pParent = dynamic_cast<QTreeWidgetItem*>(parent());

//    qDebug() << parent();
//    qDebug() << pParent->backgroundColor(1);

//    pScene->setBackgroundBrush(Qt::transparent);

    if(bDrawBorder)
    {
        QRect rectBorder = rectView;
//        rectBorder.adjust(1, 1, -40, -40);
        pScene->addRect(rectBorder);
    }

    /* Plot background */
    QRect rectPlot = computePlotRectHistogram();
    QPen penPlotBackground(Qt::transparent);
    if(bDrawBorderPlot)
    {
        penPlotBackground.setColor(Qt::black);
    }
    pScene->addRect(rectPlot, penPlotBackground, QBrush(clrBackgroundHistogram));

    /* Draw bars */
    clrBarPenDefaultHistogram_ = Qt::black;
    QPen penBars(QBrush(clrBarPenDefaultHistogram_), rHistogramPlotLineWidth_*2);

    for(int iIndex=0; iIndex < vecBarsHistogram_.count(); ++iIndex)
    {
        QRect rectBar = vecBarsHistogram_.at(iIndex);

         pScene->addRect(rectBar, penBars, QBrush(vecBarColorsHistogram_.at(iIndex)));

        if(bDrawValueLabels)
        {
            QFont fontValuelLabels = pScene->font();
            fontValuelLabels.setPointSize(iValueLabelFontSize);

            /* Draw numbers */
            QString strValueLabel =  vecValueLabelsHistogram_.at(iIndex);
            QRect rectValueLabel = vecValueLabelPositionsHistogram_.at(iIndex);

            QFontMetrics fm(fontValuelLabels);

            switch(orientationModeHistogram_)
            {
                case North:
                {
                    QGraphicsTextItem *pTextItem = pScene->addText(strValueLabel, fontValuelLabels);
                    pTextItem->setPos(rectValueLabel.left(), rectValueLabel.y() -10);
                    pTextItem->setDefaultTextColor(Qt::black);
                }
                break;
                case East:
                {
                    QGraphicsTextItem *pTextItem = pScene->addText(strValueLabel, fontValuelLabels);
                    pTextItem->setPos(rectValueLabel.left() - 10, rectValueLabel.y() -30);
                    pTextItem->setDefaultTextColor(Qt::black);
                }
                break;
                case South:
                {
                    QGraphicsTextItem *pTextItem = pScene->addText(strValueLabel, fontValuelLabels);
                    pTextItem->setPos(rectValueLabel.left(), rectValueLabel.y() - 30);
                    pTextItem->setDefaultTextColor(Qt::black);
                }
                break;
                case West:
                    QGraphicsTextItem *pTextItem = pScene->addText(strValueLabel, fontValuelLabels);
                    pTextItem->setPos(rectValueLabel.left() - 10, rectValueLabel.y() -30);
                    pTextItem->setDefaultTextColor(Qt::black);
                break;
            }
        }

        /* Attach labels */
    }

    renderAxesHistogram();

    /* Draw level labels */
    if(bDrawLevelLabelsHistogram)
    {
        QMapIterator<QString, QRect> mapIt(mapLevelLabelsPositionHistogram_);
        QFont fontLevelLabels = pScene->font();
        fontLevelLabels.setPointSize(iLevelLabelFontSize);
        while(mapIt.hasNext())
        {
            mapIt.next();
            QString strLabel = mapIt.key();
            QRect rect = mapIt.value();


            if(strLabel == strTreatmentLevel_)
            {
                fontLevelLabels.setBold(true);
            }

            QColor clrLevelLables = Qt::black;
            if(bUseGroupColorsForLables_)
            {
                if(!lstDistributions_.isEmpty())
                {
                    if(strLabel == strTreatmentLevel_)
                    {
                        clrLevelLables = lstDistributions_.first().group().colorPrimary();
                    }
                    else
                    {
                        clrLevelLables = lstDistributions_.first().group().colorSecondary();
                    }
                }
            }

            switch(orientationModeHistogram_)
            {
                case North:
                {
                    QGraphicsTextItem *pTextItem = pScene->addText(strLabel, fontLevelLabels);
                    pTextItem->setPos(rect.center().x() - 10, rect.y() -30);
                    pTextItem->setDefaultTextColor(clrLevelLables);
                }
                break;
                case East:
                {
                    QGraphicsTextItem *pTextItem = pScene->addText(strLabel, fontLevelLabels);
                    pTextItem->setPos(rect.center().x() - 20, rect.y());
                    pTextItem->setDefaultTextColor(clrLevelLables);
                }
                break;
                case South:
                {
                    QGraphicsTextItem *pTextItem = pScene->addText(strLabel, fontLevelLabels);
                    pTextItem->setPos(rect.center().x(), rect.y());
                    pTextItem->setDefaultTextColor(clrLevelLables);
                }
                break;
                case West:
                {
                    QGraphicsTextItem *pTextItem = pScene->addText(strLabel, fontLevelLabels);
                    pTextItem->setPos(rect.center().x() + 20, rect.y());
                    pTextItem->setDefaultTextColor(clrLevelLables);
                }
                break;
            }
        }
    }

    /* Print message */
    if(bShowMessage_)
    {
        QColor clrMessage = Qt::black;
        if(bUseGroupColorsForMessage_)
        {
            if(!lstDistributions_.isEmpty())
            {
                clrMessage = lstDistributions_.first().group().colorPrimary();
                if(bUseSecondaryGroupColors_)
                {
                    clrMessage = lstDistributions_.first().group().colorSecondary();
                }
            }
        }
        QFont fontMessage = pScene->font();
        fontMessage.setPointSize(8);
        QGraphicsTextItem *pTextItem = pScene->addText(strMessage_, fontMessage);
        pTextItem->setDefaultTextColor(clrMessage);
        QFontMetrics fm(fontMessage);
        if(orientationMode() == OrientationMode::North)
        {
            pTextItem->setPos(rectView.right()-fm.horizontalAdvance(strMessage_)-8, rectView.top());
        }
        if(orientationMode() == OrientationMode::South)
        {
            pTextItem->setPos(rectView.right()-fm.horizontalAdvance(strMessage_)-8, rectView.bottom() - fm.height()-8);
        }
    }
}

void CDistributionView::renderDensityPlot(bool bUpdateGeometry)
{
//    DEBUG_OBJ_INFO

    if(!geometryCacheReadyDensityPlot() || bUpdateGeometry)
    {
        updateGeometryCacheDensityPlot();
    }

    QRect rectView = computeViewPortRect();
    QGraphicsScene *pScene = pView_->scene();

    /* Fill background */
    pScene->setBackgroundBrush(clrBackground_);

    if(bDrawBorder)
    {
        QRect rectBorder = rectView;
//        rectBorder.adjust(1, 1, -40, -40);
        pScene->addRect(rectBorder);
    }

    /* Plot background */
    QRect rectPlot = computePlotRectHistogram();
    QPen penPlotBackground(Qt::transparent);
    if(bDrawBorderPlot)
    {
        penPlotBackground.setColor(Qt::black);
    }
    pScene->addRect(rectPlot, penPlotBackground, QBrush(clrBackgroundHistogram));

    /* Plot curves */
    clrPenDefaultDensityPlot_ = Qt::black;
    QPen penPath(clrPenDefaultDensityPlot_, iDensityPlotLineWidth_*2);

    int iIndexCurve = 0;
    QMapIterator<CGroup, QPainterPath> mapIt(mapGroupCurves_);
    while(mapIt.hasNext())
    {
        mapIt.next();

        CGroup group = mapIt.key();
        QPainterPath path = mapIt.value();

        QColor lineColor = clrPenDefaultDensityPlot_;
        QColor fillColor = group.colorPrimary();
        if(bUseSecondaryGroupColors_)
        {
            fillColor = group.colorSecondary();
        }
//        pItemFiilled->b
        if(iIndexCurve++ > 0)
        {
            fillColor.setAlphaF(0.5);
        }

        QBrush brushFill;
        penPath.setColor(lineColor);
        brushFill.setColor(fillColor);
        brushFill.setStyle(Qt::BrushStyle::SolidPattern);
        QGraphicsPathItem* pItemFilled = pScene->addPath(path, penPath, brushFill);

        if(bDrawDensityPlotBorder_)
        {
//            QBrush brushFill;
//            penPath.setColor(lineColor);
//            brushFill.setColor(fillColor);
//            brushFill.setStyle(Qt::BrushStyle::SolidPattern);
//            QGraphicsPathItem* pItemFilled = pScene->addPath(path, penPath, brushFill);
        }
    }

    /* Render axes */
    renderAxesDensityPlot();

    /* Print message */
    if(bShowMessage_)
    {
        QColor clrMessage = Qt::black;
//        if(bUseGroupColorsForMessage_)
//        {
//            if(!lstDistributions_.isEmpty())
//            {
//                clrMessage = lstDistributions_.first().group().colorPrimary();
//                if(bUseSecondaryGroupColors_)
//                {
//                    clrMessage = lstDistributions_.first().group().colorSecondary();
//                }
//            }
//        }
        QFont fontMessage = pScene->font();
        fontMessage.setPointSize(8);
        QGraphicsTextItem *pTextItem = pScene->addText(strMessage_, fontMessage);
        pTextItem->setDefaultTextColor(clrMessage);
        QFontMetrics fm(fontMessage);
        if(orientationMode() == OrientationMode::North)
        {
            pTextItem->setPos(rectView.right()-fm.horizontalAdvance(strMessage_)-8, rectView.top());
        }
        if(orientationMode() == OrientationMode::South)
        {
            pTextItem->setPos(rectView.right()-fm.horizontalAdvance(strMessage_)-8, rectView.bottom() - fm.height()-8);
        }
    }
}

void CDistributionView::renderAxesDensityPlot()
{
    QGraphicsScene *pScene = pView_->scene();

    switch(orientationModeHistogram_)
    {
        case North:
        {
            QPen penAxes(clrAxisDensityPlot_, rAxesWidth_);
            if(bDrawHorizontalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() + QPoint(computeAxesHistogram().width(), 0)), penAxes);
            }
            if(bDrawVerticalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() - QPoint(0, computeAxesHistogram().height())), penAxes);
            }
        }
        break;
        case East:
        {
            QPen penAxes(clrAxisDensityPlot_, rAxesWidth_);
            if(bDrawHorizontalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() + QPoint(0, computeAxesHistogram().width())), penAxes);
            }
            if(bDrawVerticalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() + QPoint(computeAxesHistogram().height(), 0)), penAxes);
            }
        }
        break;
        case South:
        {
            QPen penAxes(clrAxisDensityPlot_, rAxesWidth_);
            if(bDrawHorizontalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() + QPoint(0, computeAxesHistogram().width())), penAxes);
            }
            if(bDrawVerticalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() + QPoint(computeAxesHistogram().height(), 0)), penAxes);
            }
        }
        break;
        case West:
        {
            QPen penAxes(clrAxisDensityPlot_, rAxesWidth_);
            if(bDrawHorizontalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() + QPoint(0, computeAxesHistogram().width())), penAxes);
            }
            if(bDrawVerticalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() - QPoint(computeAxesHistogram().height(), 0)), penAxes);
            }
        }
        break;
    }
}

void CDistributionView::renderViolinPlot(bool bUpdateGeometry)
{
    //    DEBUG_OBJ_INFO

    if(!geometryCacheReadyViolinPlot() || bUpdateGeometry)
    {
        updateGeometryCacheViolinPlot();
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
    QRect rectPlot = computePlotRectViolinPlot();
    QPen penPlotBackground(Qt::transparent);
    if(bDrawBorderPlot)
    {
        penPlotBackground.setColor(Qt::red);
    }
    pScene->addRect(rectPlot, penPlotBackground, QBrush(clrBackgroundViolinPlot));

    /* Render axes */
    renderAxesViolinPlot();

    /* Plot curves */
    clrViolinPlotEdge_ = Qt::black;
    QPen penPathBorder(clrViolinPlotEdge_, 1);
    QBrush brushPath;

    QMapIterator<CGroup, QPainterPath> mapIt(mapViolinData_);
    while(mapIt.hasNext())
    {
        mapIt.next();

        CGroup group = mapIt.key();
        QPainterPath path = mapIt.value();

        QColor color = group.colorPrimary();
        if(bUseSecondaryGroupColors_)
        {
            color = group.colorSecondary();
        }
        penPathBorder.setColor(color);
        brushPath.setColor(color);
        brushPath.setStyle(Qt::BrushStyle::SolidPattern);

        brushPath.setColor(color);
        QGraphicsPathItem* pItemFill = pScene->addPath(path, penPathBorder, brushPath);

        if(bDrawViolinPlotEdge_)
        {
            penPathBorder.setColor(clrViolinPlotEdge_);
            brushPath.setStyle(Qt::BrushStyle::NoBrush);
            QGraphicsPathItem* pItemBorder = pScene->addPath(path, penPathBorder, brushPath);
        }
    }

    /* Mirror data */
    QMapIterator<CGroup, QPainterPath> mapItMirror(mapViolinMirrorData_);
    while(mapItMirror.hasNext())
    {
        mapItMirror.next();

        CGroup group = mapItMirror.key();
        QPainterPath path = mapItMirror.value();

        QColor color = group.colorPrimary();
        if(bUseSecondaryGroupColors_)
        {
            color = group.colorSecondary();
        }
        penPathBorder.setColor(color);
        brushPath.setColor(color);
        brushPath.setStyle(Qt::BrushStyle::SolidPattern);
        brushPath.setColor(color);
        QGraphicsPathItem* pItem = pScene->addPath(path, penPathBorder, brushPath);

        if(bDrawViolinPlotEdge_)
        {
            penPathBorder.setColor(clrViolinPlotEdge_);
            brushPath.setStyle(Qt::BrushStyle::NoBrush);
            QGraphicsPathItem* pItemBorder = pScene->addPath(path, penPathBorder, brushPath);
        }
    }

    /* Box plot */
    clrPenDefaultDensityPlot_ = Qt::black;
    QPen penPathBox(clrPenDefaultDensityPlot_, rViolinPlotLineWidth_);
    QMapIterator<CGroup, QPainterPath> mapItBoxPlot(mapViolinBoxPlotData_);
    while(mapItBoxPlot.hasNext())
    {
        mapItBoxPlot.next();

        CGroup group = mapItBoxPlot.key();
        QPainterPath path = mapItBoxPlot.value();

//        penPathBox.setColor(Qt::black);
        penPathBox.setWidthF(rViolinPlotBoxPlotLineWidth_);
        brushPath.setColor(Qt::black);
        brushPath.setStyle(Qt::BrushStyle::SolidPattern);
//        brushPath.setColor(clrBackgroundViolinPlot);
        QGraphicsPathItem* pItem = pScene->addPath(path, penPathBox, brushPath);

        penPathBox.setWidthF(rViolinPlotBoxPlotLineWidth_);
//        penPathBox.setColor(clrBoxPlotViolinPlot_);
        penPathBox.setColor(Qt::black);
        brushPath.setStyle(Qt::BrushStyle::NoBrush);
//        QGraphicsPathItem* pItemBorder = pScene->addPath(path, penPathBox, brushPath);
    }


    QColor clrLabelText = Qt::black;
    if(bUseGroupColorsForLables_)
    {
        if(!lstDistributions_.isEmpty())
        {
            clrLabelText = lstDistributions_.first().group().colorPrimary();
        }
    }

    /* Labels */
    QMapIterator<QString, QPointF> mapIteratorLabels(mapValueLabelPositionsViolinPlot_);
    while(mapIteratorLabels.hasNext())
    {
        mapIteratorLabels.next();

        QString strLabelName = mapIteratorLabels.key();
        QPointF pos = mapIteratorLabels.value();

        /* For now */
        if(strLabelName != "middle")
        {
            continue;
        }

        QFont font = pScene->font();
        font.setPointSize(12);
        font.setBold(false);
        if(strLabelName == "middle")
        {
            font.setBold(true);
            font.setPointSize(12);
        }
        QString strValue = mapLabelsViolinPlot_.value(strLabelName);
        QGraphicsTextItem *pTextItem = pScene->addText(strValue, font);
        pScene->setBackgroundBrush(clrBackgroundViolinPlot);
        pTextItem->setPos(pos);
        pTextItem->setDefaultTextColor(clrLabelText);
    }

    /* Print message */
    if(bShowMessage_)
    {
        QColor clrMessage = Qt::black;
        if(bUseGroupColorsForMessage_)
        {
            if(!lstDistributions_.isEmpty())
            {
                clrMessage = lstDistributions_.first().group().colorPrimary();
                if(bUseSecondaryGroupColors_)
                {
                    clrMessage = lstDistributions_.first().group().colorSecondary();
                }
            }
        }
        QFont fontMessage = pScene->font();
        fontMessage.setPointSize(8);
        QGraphicsTextItem *pTextItem = pScene->addText(strMessage_, fontMessage);
        pTextItem->setDefaultTextColor(clrMessage);
        QFontMetrics fm(fontMessage);
        if(orientationMode() == OrientationMode::North)
        {
            pTextItem->setPos(rectView.right()-fm.horizontalAdvance(strMessage_)-8, rectView.top());
        }
        if(orientationMode() == OrientationMode::South)
        {
            pTextItem->setPos(rectView.right()-fm.horizontalAdvance(strMessage_)-8, rectView.bottom() - fm.height()-8);
        }
    }
}

void CDistributionView::renderAxesViolinPlot()
{
    QGraphicsScene *pScene = pView_->scene();

    QPen penAxes(clrBarPenDefaultHistogram_, rAxesWidth_);

    QPoint pntOrigin = computeOriginDensityPlot();
    QSize sizeAxes = computeAxesDensityplot();

    QPoint pntOriginCentered = pntOrigin;
    pntOriginCentered.setX(pntOrigin.x() + sizeAxes.width() / 2);

    if(bDrawHorizontalAxis_)
    {
        clrAxisViolinPlot_ = Qt::darkGray;
        QPen penAxesViolinPlot(clrAxisViolinPlot_, 1);
        pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() + QPoint(computeAxesHistogram().width(), 0)), penAxesViolinPlot);
    }
    if(bDrawVerticalAxis_)
    {
        clrAxisViolinPlot_ = Qt::darkGray;
        QPen penAxesViolinPlot(clrAxisViolinPlot_, 1);
        pScene->addLine(QLineF(QPoint(pntOriginCentered.x(), computeAxesHistogram().height()),
                               QPoint(pntOriginCentered.x(), -computeAxesHistogram().height())), penAxesViolinPlot);
    }

    /* Draw labels */
    qreal rOffsetX = 3;
    QFont fontViolinPlotLabels;
    if(bBoldLevelLabel)
    {
        fontViolinPlotLabels.setBold(true);
    }

//    mapIt.next();
//    QString strLabel = mapIt.key();
//    strLabel = "Tr: " + strLabel;
//    QRect rect = mapIt.value();

//    fontLevelLabels.setBold(strLabel == strTreatmentLevel_);

    QColor clrLevelLables = clrPenDefaultViolinPlot_;
//    if(bUseGroupColorsForLables_)
    {
        if(!lstDistributions_.isEmpty())
        {
            /* Use primary color */
            if(strLabelXAxis_ == strTreatmentLevel_ || bBoldLevelLabel)
            {
                clrLevelLables = lstDistributions_.first().group().colorPrimary();
            }
            else /* Use secondary color */
            {
                clrLevelLables =  lstDistributions_.first().group().colorSecondary();
            }
        }
    }

    QString strLabelFinal = "Tr: " + strLabelXAxis_;
    QGraphicsTextItem *pTextItem = pScene->addText(strLabelFinal, fontViolinPlotLabels);
    QFontMetrics fm(fontViolinPlotLabels);
    pTextItem->setPos(pntOriginCentered.x() - fm.horizontalAdvance(strLabelFinal)/2 - rOffsetX,
                      computeAxesHistogram().height() + iLevelLabelOffsetVerticalViolinPlot_ / 2);
    pTextItem->setDefaultTextColor(clrLevelLables);
}

void CDistributionView::renderAxesHistogram()
{
    QGraphicsScene *pScene = pView_->scene();

    switch(orientationModeHistogram_)
    {
        case North:
        {
            QPen penAxes(clrAxisHistogram_, rAxesWidth_);

            if(bDrawHorizontalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() + QPoint(computeAxesHistogram().width(), 0)), penAxes);
            }
            if(bDrawVerticalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() - QPoint(0, computeAxesHistogram().height())), penAxes);
            }
        }
        break;
        case East:
        {
            QPen penAxes(clrAxisHistogram_, rAxesWidth_);
            if(bDrawHorizontalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() + QPoint(0, computeAxesHistogram().width())), penAxes);
            }
            if(bDrawVerticalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() + QPoint(computeAxesHistogram().height(), 0)), penAxes);
            }
        }
        break;
        case South:
        {
            QPen penAxes(clrAxisHistogram_, rAxesWidth_);
            if(bDrawHorizontalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() + QPoint(0, computeAxesHistogram().width())), penAxes);
            }
            if(bDrawVerticalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() + QPoint(computeAxesHistogram().height(), 0)), penAxes);
            }
        }
        break;
        case West:
        {
            QPen penAxes(clrAxisHistogram_, rAxesWidth_);
            if(bDrawHorizontalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() + QPoint(0, computeAxesHistogram().width())), penAxes);
            }
            if(bDrawVerticalAxis_)
            {
                pScene->addLine(QLineF(computeOriginHistogram(), computeOriginHistogram() - QPoint(computeAxesHistogram().height(), 0)), penAxes);
            }
        }
        break;
    }
}

void CDistributionView::clear()
{
    lstDistributions_.clear();
    updateView(true);
}

void CDistributionView::updateView(bool bUpdateGeometry)
{
    render(bUpdateGeometry);
}

CDistributionView::ViewMode CDistributionView::viewMode() const
{
    return viewMode_;
}

void CDistributionView::setViewMode(const CDistributionView::ViewMode &value)
{
    viewMode_ = value;
}

CDistributionView::ScalingMode CDistributionView::scalingModeHorizontal() const
{
    return scalingModeHorizontal_;
}

void CDistributionView::setScalingModeHorizontal(const ScalingMode &value)
{
    scalingModeHorizontal_ = value;
}

CDistributionView::ScalingMode CDistributionView::scalingModeVertical() const
{
    return scalingModeVertical_;
}

void CDistributionView::setScalingModeVertical(const ScalingMode &scalingModeVertical)
{
    scalingModeVertical_ = scalingModeVertical;
}

void CDistributionView::applyStyleSheet()
{
    /* Histogram */
//    clrAxisHistogram_ = Qt::white;
//    clrBarPenDefaultHistogram_ = Qt::black;
//    clrBarFillDefaultHistogram_ = Qt::white;
//    clrBarPenSelectedHistogram_ = Qt::red;
//    clrBarFillSelectedHistogram_ = Qt::white;
//    clrBarPenHighlightedHistogram_ = Qt::green;
//    clrBarFillHighlightedHistogram_ = Qt::white;

//    /* Density plot*/
//    clrBackgroundDensityPlot = Qt::white;
//    clrAxisDensityPlot_ = Qt::white;
//    clrPenDefaultDensityPlot_ = Qt::black;

    QString strStyleSheet = styleSheet();
    strStyleSheet += QString("QWidget {border-bottom: none}");
    setStyleSheet(strStyleSheet);
}

bool CDistributionView::drawBorder() const
{
    return bDrawBorder;
}

void CDistributionView::setDrawBorder(bool value)
{
    bDrawBorder = value;
}

bool CDistributionView::drawBorderPlot() const
{
    return bDrawBorderPlot;
}

void CDistributionView::setDrawBorderPlot(bool value)
{
    bDrawBorderPlot = value;
}

void CDistributionView::setDrawAxes(bool bDrawHorizontal, bool bDrawVertical)
{
    bDrawHorizontalAxis_ = bDrawHorizontal;
    bDrawVerticalAxis_ = bDrawVertical;
}

QRect CDistributionView::computePlotRectHistogram() const
{
    QRect rectView = computeViewPortRect();
    rectView.adjust(marPlot_.left(), marPlot_.top(), -marPlot_.right(), -marPlot_.bottom());

    switch(orientationMode())
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

QRect CDistributionView::computeViewPortRect() const
{
    QMargins margins = contentsMargins();
    QRect rectView = rect();
    rectView.adjust(margins.left(), margins.top(), -margins.right(), -margins.bottom());

    return rectView;
}

QPoint CDistributionView::computeOriginHistogram() const
{
    QPoint pnt;

    switch(orientationModeHistogram_)
    {
        case North:
        {
            pnt = computePlotRectHistogram().bottomLeft();
        }
        break;
        case East:
        {
            pnt = computePlotRectHistogram().topLeft();
        }
        break;
        case South:
        {
            pnt = computePlotRectHistogram().topLeft();
        }
        break;
        case West:
        {
            pnt = computePlotRectHistogram().topRight();
        }
        break;
    }

    return pnt;
}

QSize CDistributionView::computeAxesDensityplot() const
{
    QSize size;

    size = QSize(computePlotRectDensityPlot().width(), computePlotRectDensityPlot().height());

    return size;
}

QPoint CDistributionView::computeCenterDensityPlot() const
{
    QPoint pntCenter = computeOriginDensityPlot();

    pntCenter.setX(pntCenter.x() + computeAxesDensityplot().width() / 2);
    pntCenter.setY(pntCenter.y() - computeAxesDensityplot().height() / 2);

    return pntCenter;
}

QRect CDistributionView::computePlotRectDensityPlot() const
{
    QRect rectView = computeViewPortRect();
    rectView.adjust(marPlot_.left(), marPlot_.top(), -marPlot_.right(), -marPlot_.bottom());
    rectView.adjust(marLabels_.left(), 0, 0, -marLabels_.bottom());

    return rectView;
}

QPoint CDistributionView::computeOriginDensityPlot() const
{
    QPoint pnt;

    switch(orientationModeHistogram_)
    {
        case North:
        {
            pnt = computePlotRectDensityPlot().bottomLeft();
        }
        break;
        case East:
        {
            pnt = computePlotRectDensityPlot().topLeft();
        }
        break;
        case South:
        {
            pnt = computePlotRectDensityPlot().topLeft();
        }
        break;
        case West:
        {
            pnt = computePlotRectDensityPlot().topRight();
        }
        break;
    }

    return pnt;
}

QString CDistributionView::getStrLabelYAxis() const
{
    return strLabelYAxis_;
}

void CDistributionView::setStrLabelYAxis(const QString &value)
{
    strLabelYAxis_ = value;
}

QMargins CDistributionView::marginsPlot() const
{
    return marPlot_;
}

void CDistributionView::setMarginsPlot(const QMargins &marPlot)
{
    marPlot_ = marPlot;
}

QMargins CDistributionView::margingsLabels() const
{
    return marLabels_;
}

void CDistributionView::setMarginsLabels(const QMargins &marLabels)
{
    marLabels_ = marLabels;
}

bool CDistributionView::useSecondaryGroupColors() const
{
    return bUseSecondaryGroupColors_;
}

void CDistributionView::setUseSecondaryGroupColors(bool bUseSecondaryGroupColors)
{
    bUseSecondaryGroupColors_ = bUseSecondaryGroupColors;
}

bool CDistributionView::alternatePrimarySecondaryGroupColorsOverLevels() const
{
    return bAlternatePrimarySecondaryGroupColorsOverLevels_;
}

void CDistributionView::setAlternatePrimarySecondaryGroupColorsOverLevels(bool bAlternatePrimarySecondaryGroupColorsOverLevels)
{
    bAlternatePrimarySecondaryGroupColorsOverLevels_ = bAlternatePrimarySecondaryGroupColorsOverLevels;
}

QString CDistributionView::treatmentLevel() const
{
    return strTreatmentLevel_;
}

void CDistributionView::setTreatmentLevel(const QString &strTreatmentLevel)
{
    strTreatmentLevel_ = strTreatmentLevel;
}

qreal CDistributionView::axesWidth() const
{
    return rAxesWidth_;
}

void CDistributionView::setAxesWidth(qreal rAxesWidth)
{
    rAxesWidth_ = rAxesWidth;
}

bool CDistributionView::boldLevelLabel() const
{
    return bBoldLevelLabel;
}

void CDistributionView::setBoldLevelLabel(bool value)
{
    bBoldLevelLabel = value;
}

bool CDistributionView::useGroupColorsForLables() const
{
    return bUseGroupColorsForLables_;
}

void CDistributionView::setUseGroupColorsForLables(bool bUserGroupColorsForLables)
{
    bUseGroupColorsForLables_ = bUserGroupColorsForLables;
}

QPair<qreal, qreal> CDistributionView::globalDomainViolinPlot() const
{
    return pairGlobalDomainViolinPlot_;
}

void CDistributionView::setGlobalDomainViolinPlot(const QPair<qreal, qreal> &pairGlobalDomainViolinPlot)
{
    pairGlobalDomainViolinPlot_ = pairGlobalDomainViolinPlot;
}

bool CDistributionView::getUseGlobalDomainForViolinPlot() const
{
    return bUseGlobalDomainForViolinPlot_;
}

void CDistributionView::setUseGlobalDomainForViolinPlot(bool bUseGlobalDomainForViolinPlot)
{
    bUseGlobalDomainForViolinPlot_ = bUseGlobalDomainForViolinPlot;
}

QPair<qreal, qreal> CDistributionView::violinPlotDomain() const
{
    return pairViolinPlotDomain_;
}

void CDistributionView::setViolinPlotDomain(const QPair<qreal, qreal> &pairViolinPlotDomain)
{
    pairViolinPlotDomain_ = pairViolinPlotDomain;
}

QPair<qreal, qreal> CDistributionView::getViolinPlotRange() const
{
    return pairViolinPlotRange_;
}

void CDistributionView::setViolinPlotRange(const QPair<qreal, qreal> &pairViolinPlotRange)
{
    pairViolinPlotRange_ = pairViolinPlotRange;
}

QString CDistributionView::strMessage() const
{
    return strMessage_;
}

void CDistributionView::setMessage(const QString &strMessage)
{
    strMessage_ = strMessage;
    if(bShowMessage_)
    {
        render();
    }
}

bool CDistributionView::showMessage() const
{
    return bShowMessage_;
}

void CDistributionView::setShowMessage(bool bShowMessage)
{
    bShowMessage_ = bShowMessage;
    if(bShowMessage_)
    {
        render();
    }
}

bool CDistributionView::useGroupColorsForMessage() const
{
    return bUseGroupColorsForMessage_;
}

void CDistributionView::setUseGroupColorsForMessage(bool bUseGroupColorsForMessage)
{
    bUseGroupColorsForMessage_ = bUseGroupColorsForMessage;
}

int CDistributionView::violinPlotDomainToPixel(qreal rValue) const
{
    //    pairViolinPlotDomain_
    QRect rectPlot  = computePlotRectViolinPlot();

    /* Compute percentage */
//    qreal rPercentage = (rValue - pairGlobalDomainViolinPlot_.first) / pairGlobalDomainViolinPlot_.second;
    qreal rPercentage = rValue / pairGlobalDomainViolinPlot_.second;
    rPercentage = qMax(qMin(1.0, rPercentage), 0.0);

    /* We need to scale to full rect this time */
    return qRound(rectPlot.height() * rPercentage);
}

int CDistributionView::violinPlotRangeToPixel(qreal rValue) const
{

}

QString CDistributionView::getStrLabelXAxis() const
{
    return strLabelXAxis_;
}

void CDistributionView::setStrLabelXAxis(const QString &value)
{
    strLabelXAxis_ = value;
}

QSize CDistributionView::computeAxesViolinplot() const
{
    QSize size;

    switch(orientationModeViolinPlot_)
    {
    case North:
    {
        size = QSize(computePlotRectHistogram().width(), computePlotRectHistogram().height());
        }
        break;
        case East:
        {
            size = QSize(computePlotRectHistogram().height(), computePlotRectHistogram().width());
        }
        break;
        case South:
        {
            size = QSize(computePlotRectHistogram().width(), computePlotRectHistogram().height());
        }
        break;
        case West:
        {
            size = QSize(computePlotRectHistogram().height(), computePlotRectHistogram().width());
        }
        break;
    }

    return size;
}

QPoint CDistributionView::computeCenterViolinPlot() const
{
    QPoint pntCenter = computeOriginViolinPlot();

    switch(orientationModeHistogram_)
    {
        case North:
        {
            pntCenter.setX(pntCenter.x() + computeAxesViolinplot().width() / 2);
            pntCenter.setY(pntCenter.y() - computeAxesViolinplot().height() / 2);
        }
        break;
        case East:
        {
            pntCenter.setX(pntCenter.x() + computeAxesViolinplot().height() / 2);
            pntCenter.setY(pntCenter.y() - computeAxesViolinplot().width() / 2);
        }
        break;
        case South:
        {
            pntCenter.setX(pntCenter.x() + computeAxesViolinplot().width() / 2);
            pntCenter.setY(pntCenter.y() - computeAxesViolinplot().height() / 2);
        }
        break;
        case West:
        {
            pntCenter.setX(pntCenter.x() + computeAxesViolinplot().height() / 2);
            pntCenter.setY(pntCenter.y() - computeAxesViolinplot().width() / 2);
        }
        break;
    }

    return pntCenter;
}

QRect CDistributionView::computePlotRectViolinPlot() const
{
    QRect rectView = computeViewPortRect();
    rectView.adjust(marPlot_.left(), marPlot_.top(), -marPlot_.right(), -marPlot_.bottom());

    switch(orientationModeViolinPlot_)
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

QPoint CDistributionView::computeOriginViolinPlot() const
{
    QPoint pnt;

    switch(orientationModeViolinPlot_)
    {
        case North:
        {
            pnt = computePlotRectHistogram().bottomLeft();
        }
        break;
        case East:
        {
            pnt = computePlotRectHistogram().topLeft();
        }
        break;
        case South:
        {
            pnt = computePlotRectHistogram().topLeft();
        }
        break;
        case West:
        {
            pnt = computePlotRectHistogram().topRight();
        }
        break;
    }

    return pnt;
}

void CDistributionView::resizeEvent(QResizeEvent *event)
{
    render(true);
}

bool CDistributionView::geometryCacheReadyViolinPlot() const
{
    return bGeometryCacheReadyViolinPlot_;
}

void CDistributionView::setGeometryCacheReadyViolinPlot(bool bGeometryCacheReadyViolinPlot)
{
    bGeometryCacheReadyViolinPlot_ = bGeometryCacheReadyViolinPlot;
}

CDistributionView::OrientationMode CDistributionView::orientationModeViolinPlot() const
{
    return orientationModeViolinPlot_;
}

void CDistributionView::setOrientationModeViolinPlot(const OrientationMode &orientationModeViolinPlot)
{
    orientationModeViolinPlot_ = orientationModeViolinPlot;
}

QSize CDistributionView::computeAxesHistogram() const
{
    QSize size;

    switch(orientationModeHistogram_)
    {
    case North:
    {
        size = QSize(computePlotRectHistogram().width(), computePlotRectHistogram().height());
    }
        break;
    case East:
        {
            size = QSize(computePlotRectHistogram().height(), computePlotRectHistogram().width());
        }
        break;
        case South:
        {
            size = QSize(computePlotRectHistogram().width(), computePlotRectHistogram().height());
        }
        break;
        case West:
        {
            size = QSize(computePlotRectHistogram().height(), computePlotRectHistogram().width());
        }
        break;
    }

    return size;
}

QPoint CDistributionView::computeCenterHistogram() const
{
    QPoint pntCenter = computeOriginHistogram();

    switch(orientationModeHistogram_)
    {
        case North:
        {
            pntCenter.setX(pntCenter.x() + computeAxesHistogram().width() / 2);
            pntCenter.setY(pntCenter.y() - computeAxesHistogram().height() / 2);
        }
        break;
        case East:
        {
            pntCenter.setX(pntCenter.x() + computeAxesHistogram().height() / 2);
            pntCenter.setY(pntCenter.y() - computeAxesHistogram().width() / 2);
        }
        break;
        case South:
        {
            pntCenter.setX(pntCenter.x() + computeAxesHistogram().width() / 2);
            pntCenter.setY(pntCenter.y() - computeAxesHistogram().height() / 2);
        }
        break;
        case West:
        {
            pntCenter.setX(pntCenter.x() + computeAxesHistogram().height() / 2);
            pntCenter.setY(pntCenter.y() - computeAxesHistogram().width() / 2);
        }
        break;
    }

    return pntCenter;
}

bool CDistributionView::geometryCacheReadyDensityPlot() const
{
    return bGeometryCacheReadyDensityPlot_;
}

void CDistributionView::setGeometryCacheReadyDensityPlot(bool bGeometryCacheReadyDensityPlot)
{
    bGeometryCacheReadyDensityPlot_ = bGeometryCacheReadyDensityPlot;
}

QPair<qreal, qreal> CDistributionView::computeMinMaxHistogramValues() const
{
    QPair<qreal, qreal> pairMinMax;

    pairMinMax.first = 99999;
    pairMinMax.second = -99999;

    QMapIterator<QString, QMap<CGroup, qreal>> mapIt(mapDataHistogram_);
    while(mapIt.hasNext())
    {
        mapIt.next();
        QString strLevel = mapIt.key();
        QMap<CGroup, qreal> mapGroupValue = mapIt.value();

        /* Loop groups and levels */
        QMapIterator<CGroup, qreal> mapItGroupValue(mapGroupValue);
        while(mapItGroupValue.hasNext())
        {
            mapItGroupValue.next();
            CGroup group = mapItGroupValue.key();
            qreal rValue = mapItGroupValue.value();

            pairMinMax.first = qMin(pairMinMax.first, rValue);
            pairMinMax.second = qMax(pairMinMax.second, rValue);
        }
    }

    return pairMinMax;
}

bool CDistributionView::geometryCacheReadyHistogram() const
{
    return bGeometryCacheReadyHistogram_;
}

void CDistributionView::setGeometryCacheReadyHistogram(bool bGeometryCacheReady)
{
    bGeometryCacheReadyHistogram_ = bGeometryCacheReady;
}

void CDistributionView::clearGeometryCache()
{
    clearGeometryCacheHistogram();
    clearGeometryCacheDensityPlot();
    clearGeometryCacheViolinPlot();
}

void CDistributionView::updateGeometryCacheHistogram()
{
//    DEBUG_OBJ_INFO

    clearGeometryCacheHistogram();

    /* Fill value table */
    if(!lstDistributions_.isEmpty())
    {
        CDistribution firstDistribution = lstDistributions_.first();

        /* Initialize frequency table */
        foreach(CDistribution distribution, lstDistributions_)
        {
            /* Skip incompatible distributions */
            if(distribution.variable().name() != firstDistribution.variable().name())
                continue;

            /* Read and store level values (+properties) for distribution */
            foreach(QString strLevel, distribution.variable().levels())
            {
                QMap<CGroup, qreal> mapGroupValue = mapDataHistogram_.value(strLevel);
                mapGroupValue[distribution.group()] = distribution.frequencies().value(strLevel);
                mapDataHistogram_[strLevel] = mapGroupValue;
            }
        }

        if(iBarWidthHistogram_ < 12)
        {
            iBarWidthHistogram_ = 12;
        }
        /* Generate bars */
        QStringList lstLevels = firstDistribution.variable().levels();
        int iGroupCount = lstDistributions_.count();
        int iLevelCount = lstLevels.count();
        int iBarsOneLevelWidth = (iGroupCount * iBarWidthHistogram_) + ((iGroupCount-1) * iBarSpacingHistogram_);
        int iTotalWidth = (iLevelCount * iBarsOneLevelWidth) + ((iLevelCount - 1) * iLevelSpacingHistogram_);

        /* Auto scale */
//        int iMaxWidth = computePlotRectHistogram().width() - (marPlot_.left() + marPlot_.right()) -
//                                    (marLabels_.left() + marLabels_.right());
        int iMaxWidth = computePlotRectHistogram().width();

        qreal fracMaxWidth = 1;

//        /* First expand */
//        if(iTotalWidth < iMaxWidth)
//        {
//            while(iTotalWidth < fracMaxWidth*iMaxWidth)
//            {
//                iBarWidthHistogram_ *= 2;
//                iBarsOneLevelWidth = (iGroupCount * iBarWidthHistogram_) + ((iGroupCount-1) * iBarSpacingHistogram_);
//                iLevelSpacingHistogram_ = 0.3*iBarsOneLevelWidth;
//                iTotalWidth = (iLevelCount * iBarsOneLevelWidth) + ((iLevelCount - 1) * iLevelSpacingHistogram_);
//            }
//        }

        while(iTotalWidth > fracMaxWidth*iMaxWidth)
        {
            --iBarWidthHistogram_;
            iBarsOneLevelWidth = (iGroupCount * iBarWidthHistogram_) + ((iGroupCount-1) * iBarSpacingHistogram_);
            iLevelSpacingHistogram_ = 0.5*iBarsOneLevelWidth;
            iTotalWidth = (iLevelCount * iBarsOneLevelWidth) + ((iLevelCount - 1) * iLevelSpacingHistogram_);
        }

        int iStartAxis = 0;
        int iMaxBarHeight = 10;

        switch(orientationModeHistogram_)
        {
            case North:
            case South:
            {
                iStartAxis = computeCenterHistogram().x() - (iTotalWidth / 2);
                iMaxBarHeight = static_cast<int>(computePlotRectHistogram().height() * 0.90);
            }
            break;
            case East:
            case West:
            {
                iStartAxis = -computeCenterHistogram().y() - (iTotalWidth / 2);
                iMaxBarHeight = static_cast<int>(computePlotRectHistogram().width() * 0.90);
            }
            break;
        }

        QPair<qreal, qreal> pairMinMax = computeMinMaxHistogramValues();

        int iIlevelIndex = 0;
        /* Loop levels */
        QMapIterator<QString, QMap<CGroup, qreal>> mapIt(mapDataHistogram_);
        while(mapIt.hasNext())
        {
            mapIt.next();
            QString strLevel = mapIt.key();
            QMap<CGroup, qreal> mapGroupValue = mapIt.value();

            /* Loop groups */
            QMapIterator<CGroup, qreal> mapItGroupValue(mapGroupValue);
            while(mapItGroupValue.hasNext())
            {
                mapItGroupValue.next();
                CGroup group = mapItGroupValue.key();
                qreal rValue = mapItGroupValue.value();

                if(!mapLevelLabelsPositionHistogram_.contains(strLevel))
                {
                    switch(orientationModeHistogram_)
                    {
                        case North:
                            mapLevelLabelsPositionHistogram_[strLevel] = QRect(QPoint(iStartAxis,
                                                                             computeOriginHistogram().y() + iLevelLabelOffsetVertical_),
                                                             QSize(iBarsOneLevelWidth, iLevelLabelheight));
                        break;
                        case East:
                             mapLevelLabelsPositionHistogram_[strLevel] = QRect(QPoint(computeOriginHistogram().x() - iLevelLabelOffsetHorizontal_,
                                                                              iStartAxis + (iBarsOneLevelWidth/2)),
                                                                QSize(LABEL_MARGIN_LEFT, iBarsOneLevelWidth));
                        break;
                        case South:
                            mapLevelLabelsPositionHistogram_[strLevel] = QRect(QPoint(iStartAxis,
                                                                         computeOriginHistogram().y() - iLevelLabelOffsetVertical_),
                                                                    QSize(iBarsOneLevelWidth, iLevelLabelheight));
                        break;
                        case West:
                            mapLevelLabelsPositionHistogram_[strLevel] = QRect(QPoint(computeOriginHistogram().x() - iLevelLabelOffsetHorizontal_,
                                                                         iStartAxis + (iBarsOneLevelWidth/2)),
                                                           QSize(LABEL_MARGIN_RIGHT, iLevelLabelheight));
                        break;
                    }
                }

                qreal rFraction = rValue / pairMinMax.second;
                int iBarHeight = static_cast<int>(rFraction * iMaxBarHeight);

                QColor color = group.colorPrimary();
                if(bUseSecondaryGroupColors_)
                {
                    color = group.colorSecondary();
                }

                if(bAlternatePrimarySecondaryGroupColorsOverLevels_)
                {
                    if(strLevel != treatmentLevel())
                    {
                        color = group.colorSecondary();
                    }
                }

                vecBarColorsHistogram_.append(color);
                vecValueLabelsHistogram_.append(QString::number(rValue));

                QRect rectBar;
                QRect rectValueLabel;

                /* Generate geometry */
                switch(orientationModeHistogram_)
                {
                    case North:
                    {
                        rectBar = QRect(iStartAxis, computeOriginHistogram().y(), iBarWidthHistogram_, -iBarHeight);
                        rectValueLabel = rectBar;
                        rectValueLabel.setHeight(iValueLabelHeight);
                        rectValueLabel.translate(0, rectBar.height() -iValueLabelOffsetVertical_);
                    }
                    break;
                    case East:
                    {
                        rectBar = QRect(computeOriginHistogram().x(), computeOriginHistogram().y() + iStartAxis,
                                        iBarHeight, iBarWidthHistogram_);
                        rectValueLabel = rectBar;
                        rectValueLabel.setHeight(iValueLabelHeight);
                        rectValueLabel.translate(rectBar.width() + iValueLabelOffsetHorizontal_,
                                                 2*(rectBar.height()) - rectBar.height()/2 );
                    }
                    break;
                    case South:
                        rectBar = QRect(iStartAxis, computeOriginHistogram().y(), iBarWidthHistogram_, iBarHeight);
                        rectValueLabel = rectBar;
                        rectValueLabel.setHeight(iValueLabelHeight);
                        rectValueLabel.translate(0, rectBar.height() -iValueLabelOffsetVertical_);
                    break;
                    case West:
                    {
                        rectBar = QRect(computeOriginHistogram().x(), computeOriginHistogram().y() + iStartAxis,
                                        -iBarHeight, iBarWidthHistogram_);
                        rectValueLabel = rectBar;
                        rectValueLabel.setHeight(iValueLabelHeight);
                        rectValueLabel.translate(rectBar.width() - iValueLabelOffsetHorizontal_,
                                                 2*(rectBar.height()) - rectBar.height()/2 );
                    }
                    break;
                }

                /* Store bar and value label position */
                vecBarsHistogram_.append(rectBar);
                vecValueLabelPositionsHistogram_.append(rectValueLabel);

                /* Add tooltip */
                QString strToolTip = QString::number(rValue) +
                                    "(" + QString::number(rFraction) + ")" +
                                    " observations in group " + group.name() +
                                    " are also in subgroup " + strLevel;

                vecToolTipsHistogram_.append(strToolTip);

                iStartAxis += iBarWidthHistogram_ + iBarSpacingHistogram_;
            } // mapGroupValue

            iStartAxis += -iBarSpacingHistogram_ + iLevelSpacingHistogram_;
            ++iIlevelIndex;
        } // Levels
    }

    setGeometryCacheReadyHistogram(true);
}

void CDistributionView::updateGeometryCacheDensityPlot()
{
    clearGeometryCacheDensityPlot();

    /* Fill value table */
    if(!lstDistributions_.isEmpty())
    {
        /* Compute domain and ranges */
        QPair<qreal, qreal> pairDomain = QPair<qreal, qreal>(99999, -99999);
        QPair<qreal, qreal> pairRange = QPair<qreal, qreal>(99999, -99999);

        foreach(CDistribution distribution, lstDistributions_)
        {
            if(distribution.variable().type()=="factor")
                continue;

            QPair<qreal, qreal> distributionDomain = distribution.densitiesDomain();
            QPair<qreal, qreal> distributionRange = distribution.densitiesRange();

            pairDomain = QPair<qreal, qreal>(qMin(pairDomain.first, distributionDomain.first),
                                             qMax(pairDomain.second, distributionDomain.second));
            pairRange = QPair<qreal, qreal>(qMin(pairRange.first, distributionRange.first),
                                             qMax(pairRange.second, distributionRange.second));
        }

        QPoint pntOrigin = computeOriginDensityPlot();
        QSize sizeAxes = computeAxesDensityplot();
        qreal qScaleFactorVertical = (sizeAxes.height() / pairRange.second);
        qreal qScaleFactorHorizontal = (sizeAxes.width() / pairDomain.second);

        foreach(CDistribution distribution, lstDistributions_)
        {
            QPainterPath pathCurve;

            QVector<QPair<qreal, qreal>> vecCoorindates = distribution.densities();

            for(int i=0; i<vecCoorindates.count(); ++i)
            {
                QPair<qreal, qreal> coord = vecCoorindates.at(i);

                qreal x = coord.first;// / pairDomain.second;
                qreal y = coord.second;// / pairRange.second;

                switch(orientationModeHistogram_)
                {
                    case North:
                    {
                        if(i==0)
                        {
                            pathCurve.moveTo(pntOrigin.x() + qMax(0.0, (x * qScaleFactorHorizontal)),
                                             pntOrigin.y() - qMax(0.0, (y * qScaleFactorVertical)));
                        }
                        else
                        {
                            pathCurve.lineTo(pntOrigin.x() + qMax(0.0, (x * qScaleFactorHorizontal)),
                                             pntOrigin.y() - qMax(0.0, (y * qScaleFactorVertical)));
                        }
                    }
                    break;
                    case South:
                    {
                        if(i==0)
                        {
                            pathCurve.moveTo(pntOrigin.x() + qMax(0.0, (x * qScaleFactorHorizontal)),
                                             pntOrigin.y() + qMax(0.0, (y * qScaleFactorVertical)));
                        }
                        else
                        {
                            pathCurve.lineTo(pntOrigin.x() + qMax(0.0, (x * qScaleFactorHorizontal)),
                                             pntOrigin.y() + qMax(0.0, (y * qScaleFactorVertical)));
                        }
                    }
                    break;
                    case East:
                    {
                    }
                    break;
                    case West:
                    {
                    }
                    break;
                }



            }

//            pathCurve.closeSubpath();
            mapGroupCurves_[distribution.group()] = pathCurve;
        }
    }

    setGeometryCacheReadyDensityPlot(true);
}

void CDistributionView::updateGeometryCacheViolinPlot()
{
    clearGeometryCacheViolinPlot();

    /* Fill value table */
    if(!lstDistributions_.isEmpty())
    {
        /* Compute domain and ranges */
        QPair<qreal, qreal> pairDomain = QPair<qreal, qreal>(99999, -99999);
        QPair<qreal, qreal> pairRange = QPair<qreal, qreal>(99999, -99999);

        foreach(CDistribution distribution, lstDistributions_)
        {

            QPair<qreal, qreal> violinDomain = distribution.violinDomain();
            QPair<qreal, qreal> violinRange = distribution.violinRange();

            pairDomain = QPair<qreal, qreal>(qMin(pairDomain.first, violinDomain.first),
                                             qMax(pairDomain.second, violinDomain.second));
            pairRange = QPair<qreal, qreal>(qMin(pairRange.first, violinRange.first),
                                             qMax(pairRange.second, violinRange.second));
        }

        if(bUseGlobalDomainForViolinPlot_)
        {
            pairDomain = pairGlobalDomainViolinPlot_;
        }

        setViolinPlotDomain(pairDomain);

        QPoint pntOrigin = computeOriginDensityPlot();
        QSize sizeAxes = computeAxesDensityplot();
//        qreal qScaleFactorVertical = sizeAxes.height() * .9;
        qreal qScaleFactorHorizontal = (sizeAxes.width()/2.0 * 0.4);

        QPoint pntOriginCentered = pntOrigin;
        pntOriginCentered.setY(pntOrigin.y());
        pntOriginCentered.setX(pntOrigin.x() + sizeAxes.width() / 2);

        foreach(CDistribution distribution, lstDistributions_)
        {
            QPainterPath pathCurve;
            QPainterPath pathCurveMirror;

            QVector<QPair<qreal, qreal>> vecCoorindates = distribution.violinValues();
            for(int i=0; i<vecCoorindates.count(); ++i)
            {
                QPair<qreal, qreal> coord = vecCoorindates.at(i);

//                qScaleFactorVertical = 10;
                qreal offset = coord.first;// / pairDomain.second * qScaleFactorVertical;// / pairDomain.second;
                qreal width = coord.second / pairRange.second;
                if(i==0)
                {
//                    pathCurve.moveTo(pntOriginCentered.x() + (width * qScaleFactorHorizontal),
//                                     pntOriginCentered.y() - (offset * qScaleFactorVertical));
//                    pathCurveMirror.moveTo(pntOriginCentered.x() + (width * qScaleFactorHorizontal),
//                                           pntOriginCentered.y() - (offset * qScaleFactorVertical));
                    pathCurve.moveTo(pntOriginCentered.x() + (width * qScaleFactorHorizontal),
                                     pntOriginCentered.y() - violinPlotDomainToPixel(0));
                    pathCurveMirror.moveTo(pntOriginCentered.x() + (width * qScaleFactorHorizontal),
                                           pntOriginCentered.y() - violinPlotDomainToPixel(0));
                }
                else
                {
//                    pathCurve.lineTo(pntOriginCentered.x() + (width * qScaleFactorHorizontal),
//                                     pntOriginCentered.y() - (offset * qScaleFactorVertical));
//                    pathCurveMirror.lineTo(pntOriginCentered.x() - (width * qScaleFactorHorizontal),
//                                           pntOriginCentered.y() - (offset * qScaleFactorVertical));
                    pathCurve.lineTo(pntOriginCentered.x() + (width * qScaleFactorHorizontal),
                                     pntOriginCentered.y() - violinPlotDomainToPixel(offset));
                    pathCurveMirror.lineTo(pntOriginCentered.x() - (width * qScaleFactorHorizontal),
                                           pntOriginCentered.y() - violinPlotDomainToPixel(offset));
                }
            }

//            pathCurve.closeSubpath();
//            pathCurveMirror.closeSubpath();

//            pathCurve.closeSubpath();
            mapViolinData_[distribution.group()] = pathCurve;
            mapViolinMirrorData_[distribution.group()] = pathCurveMirror;

            setStrLabelXAxis(distribution.levelLabelViolin());

            /* Compute box */
            QPainterPath pathBoxBlot;
            QMap<QString, qreal> mapBoxPlotData = distribution.boxPlotViolin();
            qreal yLower = mapBoxPlotData["lower"];
            qreal yMiddle = mapBoxPlotData["middle"];
            qreal yUpper = mapBoxPlotData["upper"];
            qreal yNotchLower = mapBoxPlotData["notchlower"];
            qreal yNotchUpper = mapBoxPlotData["notchupper"];
            qreal yMaxFinal = mapBoxPlotData["ymax_final"];

            qreal offsetLower = pntOriginCentered.y() - (violinPlotDomainToPixel(yLower));
            qreal offsetNotchLower = pntOriginCentered.y() - (violinPlotDomainToPixel(yNotchLower));

            /* Bottom part */
            pathBoxBlot.moveTo(pntOriginCentered.x(), offsetLower);
            pathBoxBlot.lineTo(pntOriginCentered.x(), offsetNotchLower);

            qreal offsetNotchUpper = pntOriginCentered.y() - (violinPlotDomainToPixel(yNotchUpper));
            qreal offsetUpper = pntOriginCentered.y() - (violinPlotDomainToPixel(yUpper));
            qreal offsetMiddle = pntOriginCentered.y() - (violinPlotDomainToPixel(yMiddle));

            /* Top part */
            pathBoxBlot.moveTo(pntOriginCentered.x(), offsetNotchUpper);
            pathBoxBlot.lineTo(pntOriginCentered.x(), offsetUpper);

            /* Box */
            int iBoxWidth = 10;
            pathBoxBlot.addRect(QRect(QPoint(pntOriginCentered.x() - iBoxWidth / 2, offsetNotchLower),
                                    QPoint(pntOriginCentered.x() + iBoxWidth / 2, offsetMiddle)));

            pathBoxBlot.addRect(QRect(QPoint(pntOriginCentered.x() - iBoxWidth / 2, offsetMiddle),
                                    QPoint(pntOriginCentered.x() + iBoxWidth / 2, offsetNotchUpper)));

            mapViolinBoxPlotData_.insert(distribution.group(), pathBoxBlot);

            mapLabelsViolinPlot_.insert("lower", QString::number(yLower, 'f', 2));
            mapLabelsViolinPlot_.insert("middle", QString::number(yMiddle, 'f', 2));
            mapLabelsViolinPlot_.insert("upper", QString::number(yUpper, 'f', 2));
            mapLabelsViolinPlot_.insert("notchlower", QString::number(yNotchLower, 'f', 2));
            mapLabelsViolinPlot_.insert("notchupper", QString::number(yNotchUpper, 'f', 2));

            qreal rLabelX = computeOriginViolinPlot().x();
            qreal iLabelYShift = 0;
//            iLabelYShift = 15;

            qreal iLabelYCenter = offsetMiddle - 10;
            mapValueLabelPositionsViolinPlot_.insert("lower", QPointF(rLabelX, iLabelYCenter + 2.5*iLabelYShift));
            mapValueLabelPositionsViolinPlot_.insert("middle", QPointF(rLabelX, iLabelYCenter));
            mapValueLabelPositionsViolinPlot_.insert("upper", QPointF(rLabelX, iLabelYCenter - 2.5*iLabelYShift));
            mapValueLabelPositionsViolinPlot_.insert("notchlower", QPointF(rLabelX, iLabelYCenter + iLabelYShift));
            mapValueLabelPositionsViolinPlot_.insert("notchupper", QPointF(rLabelX, iLabelYCenter - iLabelYShift));
        }
    }

    setGeometryCacheReadyViolinPlot(true);
}

void CDistributionView::clearGeometryCacheHistogram()
{
    mapDataHistogram_.clear();
    vecBarsHistogram_.clear();
    vecBarColorsHistogram_.clear();
    mapLevelLabelsPositionHistogram_.clear();
    vecValueLabelsHistogram_.clear();
    vecValueLabelPositionsHistogram_.clear();
    vecToolTipsHistogram_.clear();
}

void CDistributionView::clearGeometryCacheDensityPlot()
{
    mapScaleLabelsDensityPlot_.clear();
    mapLevelLabelsPositionDensityPlot_.clear();
    mapGroupCurves_.clear();
    strLabelXAxis_.clear();
    strLabelYAxis_.clear();
    mapLabelsViolinPlot_.clear();
    mapValueLabelPositionsViolinPlot_.clear();
}

void CDistributionView::clearGeometryCacheViolinPlot()
{
    mapViolinData_.clear();
    mapViolinMirrorData_.clear();
    mapViolinBoxPlotData_.clear();
}

CDistributionView::OrientationMode CDistributionView::orientationMode() const
{
    return orientationModeHistogram_;
}

void CDistributionView::setOrientationMode(const CDistributionView::OrientationMode &orientationMode)
{
    orientationModeHistogram_ = orientationMode;
}

int CDistributionView::barWidthHistogram() const
{
    return iBarWidthHistogram_;
}

void CDistributionView::setBarWidthHistogram(int iBarWidthHistogram)
{
    iBarWidthHistogram_ = iBarWidthHistogram;
}

bool CDistributionView::drawValueLabels() const
{
    return bDrawValueLabels;
}

void CDistributionView::setDrawValueLabels(bool value)
{
    bDrawValueLabels = value;
}

bool CDistributionView::drawLevelLabelsHistogram() const
{
    return bDrawLevelLabelsHistogram;
}

void CDistributionView::setDrawLevelLabelsHistogram(bool value)
{
    bDrawLevelLabelsHistogram = value;
}

int CDistributionView::barSpacingHistogram() const
{
    return iBarSpacingHistogram_;
}

void CDistributionView::setBarSpacingHistogram(int iBarSpacingHistogram)
{
    iBarSpacingHistogram_ = iBarSpacingHistogram;
}

int CDistributionView::levelSpacingHistogram() const
{
    return iLevelSpacingHistogram_;
}

void CDistributionView::setLevelSpacingHistogram(int iLevelSpacingHistogram)
{
    iLevelSpacingHistogram_ = iLevelSpacingHistogram;
}

CDistributionsList CDistributionView::distributions() const
{
    return lstDistributions_;
}

void CDistributionView::setDistributions(const CDistributionsList &lstDistributions)
{
//    DEBUG_OBJ_INFO

    lstDistributions_ = lstDistributions;
//    lstDistributions_.removeLast();
    updateView(true);
}

void CDistributionView::onColorAndFontMapUpdated()
{
    applyStyleSheet();
    repaint();
}

void CDistributionView::onViewMousePressEvent(QMouseEvent *event)
{

}


