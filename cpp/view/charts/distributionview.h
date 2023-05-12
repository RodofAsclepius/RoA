#ifndef BALANCEVIEW_H
#define BALANCEVIEW_H

#include <QWidget>
#include "distributionlist.h"

#define HDI_SCALING 1

class CColorAndFontMap;
class CExtendedGraphicsView;


class CDistributionView : public QWidget
{
    Q_OBJECT
public:
    CDistributionView(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~CDistributionView() override;

    void render(bool bUpdateGeometry = false);

    void renderHistogram(bool bUpdateGeometry = false);
    void renderAxesHistogram();
    void renderDensityPlot(bool bUpdateGeometry = false);
    void renderAxesDensityPlot();
    void renderViolinPlot(bool bUpdateGeometry = false);
    void renderAxesViolinPlot();

    CDistributionsList distributions() const;
    void setDistributions(const CDistributionsList &lstDistributions_);
    void clear();

    void updateView(bool bUpdateGeometry = false);

    typedef enum
    {
        Histogram,
        Densityplot,
        ViolinPlot
    } ViewMode;

    ViewMode viewMode() const;
    void setViewMode(const ViewMode &value);

    typedef enum
    {
        Fixed,
        Maximize
    } ScalingMode;

    typedef enum
    {
        North,
        East,
        South,
        West
    } OrientationMode;

    ScalingMode scalingModeHorizontal() const;
    void setScalingModeHorizontal(const ScalingMode &value);

    ScalingMode scalingModeVertical() const;
    void setScalingModeVertical(const ScalingMode &scalingModeVertical);

    void applyStyleSheet();

    bool drawBorder() const;
    void setDrawBorder(bool value);

    bool drawBorderPlot() const;
    void setDrawBorderPlot(bool value);

    void setDrawAxes(bool bDrawHorizontal, bool bDrawVertical);

    bool geometryCacheReadyHistogram() const;
    void setGeometryCacheReadyHistogram(bool geometryCacheReadyHistogram = true);

    bool geometryCacheReadyDensityPlot() const;
    void setGeometryCacheReadyDensityPlot(bool geometryCacheReadyDensityPlot);

    void clearGeometryCache();

    /* Histogram */
    void updateGeometryCacheHistogram();
    void updateGeometryCacheDensityPlot();
    void updateGeometryCacheViolinPlot();

    void clearGeometryCacheHistogram();
    void clearGeometryCacheDensityPlot();
    void clearGeometryCacheViolinPlot();

    OrientationMode orientationMode() const;
    void setOrientationMode(const OrientationMode &orientationMode);

    int barWidthHistogram() const;
    void setBarWidthHistogram(int barWidthHistogram);

    bool drawValueLabels() const;
    void setDrawValueLabels(bool value);

    bool drawLevelLabelsHistogram() const;
    void setDrawLevelLabelsHistogram(bool value);

    int barSpacingHistogram() const;
    void setBarSpacingHistogram(int barSpacingHistogram);

    int levelSpacingHistogram() const;
    void setLevelSpacingHistogram(int levelSpacingHistogram);

    bool geometryCacheReadyViolinPlot() const;
    void setGeometryCacheReadyViolinPlot(bool geometryCacheReadyViolinPlot);

    OrientationMode orientationModeViolinPlot() const;
    void setOrientationModeViolinPlot(const OrientationMode &orientationModeViolinPlot);

    QString getStrLabelXAxis() const;
    void setStrLabelXAxis(const QString &value);

    QString getStrLabelYAxis() const;
    void setStrLabelYAxis(const QString &value);

    QMargins marginsPlot() const;
    void setMarginsPlot(const QMargins &marPlot);

    QMargins margingsLabels() const;
    void setMarginsLabels(const QMargins &marLabels);

    bool useSecondaryGroupColors() const;
    void setUseSecondaryGroupColors(bool bUseSecondaryGroupColors);

    bool alternatePrimarySecondaryGroupColorsOverLevels() const;
    void setAlternatePrimarySecondaryGroupColorsOverLevels(bool bAlternatePrimarySecondaryGroupColorsOverLevels);

    QString treatmentLevel() const;
    void setTreatmentLevel(const QString &strTreatmentLevel);

    qreal axesWidth() const;
    void setAxesWidth(qreal rAxesWidth);

    bool boldLevelLabel() const;
    void setBoldLevelLabel(bool value);

    bool useGroupColorsForLables() const;
    void setUseGroupColorsForLables(bool bUserGroupColorsForLables);

    QPair<qreal, qreal> globalDomainViolinPlot() const;
    void setGlobalDomainViolinPlot(const QPair<qreal, qreal> &pairGlobalDomainViolinPlot);

    bool getUseGlobalDomainForViolinPlot() const;
    void setUseGlobalDomainForViolinPlot(bool bUseGlobalDomainForViolinPlot);

    QPair<qreal, qreal> violinPlotDomain() const;
    void setViolinPlotDomain(const QPair<qreal, qreal> &pairViolinPlotDomain);

    QPair<qreal, qreal> getViolinPlotRange() const;
    void setViolinPlotRange(const QPair<qreal, qreal> &pairViolinPlotRange);

    QString strMessage() const;
    void setMessage(const QString &strMessage);

    bool showMessage() const;
    void setShowMessage(bool bShowMessage);

    bool useGroupColorsForMessage() const;
    void setUseGroupColorsForMessage(bool bUseGroupColorsForMessage);

protected:
    CColorAndFontMap& mapsColorAndFonts_;
    CDistributionsList lstDistributions_;
    OrientationMode orientationModeHistogram_ = North;
    OrientationMode orientationModeViolinPlot_ = North;
    ViewMode viewMode_ = Densityplot;
    ScalingMode scalingModeHorizontal_ = Maximize;
    ScalingMode scalingModeVertical_ = Maximize;

    /* Display options */
    QMargins marPlot_;
    QMargins marLabels_;

    bool bBoldLevelLabel = false;
    bool bDrawBorder = false;
    bool bDrawBorderPlot = false;
    bool bDrawHorizontalAxis_ = false;
    bool bDrawVerticalAxis_ = false;
    bool bDrawValueLabels = true;
    QString strTreatmentLevel_;
    QString strMessage_ = "";
    bool bShowMessage_ = false;

    bool bDrawLevelLabelsHistogram = true;

    QColor clrBackground_ = Qt::white;
    bool bUseSecondaryGroupColors_ = false;
    bool bAlternatePrimarySecondaryGroupColorsOverLevels_ = false;
    bool bUseGroupColorsForLables_ = false;
    bool bUseGroupColorsForMessage_ = false;

    /* Histogram */
//    QColor clrBackgroundHistogram = QColor("#313131");
    QColor clrBackgroundHistogram = QColor("white");
    QColor clrAxisHistogram_ = Qt::black;
    QColor clrBarPenDefaultHistogram_ = Qt::white;
    QColor clrBarFillDefaultHistogram_ = Qt::white;
    QColor clrBarPenSelectedHistogram_ = Qt::red;
    QColor clrBarFillSelectedHistogram_ = Qt::white;
    QColor clrBarPenHighlightedHistogram_ = Qt::green;
    QColor clrBarFillHighlightedHistogram_ = Qt::white;

    /* Density plot*/
//    QColor clrBackgroundDensityPlot = QColor("#313131");
    QColor clrBackgroundDensityPlot = QColor("white");
    QColor clrAxisDensityPlot_ = Qt::black;
    QColor clrPenDefaultDensityPlot_ = Qt::white;
    qreal iDensityPlotLineWidth_ = .5;
    bool bDrawDensityPlotBorder_ = true;

    /* Histogram */
    int iBarSpacingHistogram_ = 1;
    int iBarWidthHistogram_ = 24;
    int iLevelSpacingHistogram_ = 12;
    qreal rHistogramPlotLineWidth_ = .5;
    int iLevelLabelOffsetVertical_ = 30;
    int iLevelLabelOffsetHorizontal_ = 26;
    int iLevelLabelheight = 26;
    int iValueLabelOffsetVertical_ = 10;
    int iValueLabelOffsetHorizontal_ = 10;
    int iValueLabelHeight = 20;
    int iLevelLabelFontSize = 12;
    int iValueLabelFontSize = 8;

    /* Violin plot*/
//    QColor clrBackgroundViolinPlot = QColor("#313131");
    QColor clrBackgroundViolinPlot = QColor("white");
    QColor clrAxisViolinPlot_ = Qt::black;
    QColor clrPenDefaultViolinPlot_ = Qt::white;
//    QColor clrBoxPlotViolinPlot_ = Qt::black;
    QColor clrBoxPlotViolinPlot_ = Qt::white;

    QPair<qreal, qreal> pairGlobalDomainViolinPlot_ = qMakePair<qreal, qreal>(0, 200);
    QPair<qreal, qreal> pairViolinPlotDomain_ = qMakePair<qreal, qreal>(0, 1);
    QPair<qreal, qreal> pairViolinPlotRange_= qMakePair<qreal, qreal>(0, 1);
    bool bUseGlobalDomainForViolinPlot_ = true;
    int violinPlotDomainToPixel(qreal rValue) const;
    int violinPlotRangeToPixel(qreal rValue) const;
    int iLevelLabelOffsetVerticalViolinPlot_ = 26;
    int iLevelLabelOffsetHorizontalViolinPlot_ = 26;
    int iValueLabelFontSizeViolinPlot_ = 4;
    qreal rViolinPlotLineWidth_ = 1;
    qreal rViolinPlotBoxPlotLineWidth_ = 1;
    QColor clrViolinPlotEdge_ = QColor(Qt::white);
    bool bDrawViolinPlotEdge_ = true;

    /* Axes */
    qreal rAxesWidth_ = .5;

    /* Render */
    CExtendedGraphicsView *pView_;
    QRect computeViewPortRect() const;

    /* Histogram */
    QMap<QString, QMap<CGroup, qreal>> mapDataHistogram_;
    QVector<QRect> vecBarsHistogram_;
    QVector<QColor> vecBarColorsHistogram_;
    QMap<QString, QRect> mapLevelLabelsPositionHistogram_;
    QVector<QString> vecValueLabelsHistogram_;
    QVector<QRect> vecValueLabelPositionsHistogram_;
    QVector<QString> vecToolTipsHistogram_;
    QPair<qreal, qreal> computeMinMaxHistogramValues() const;
    bool bGeometryCacheReadyHistogram_ = false;

    QSize computeAxesHistogram() const;
    QPoint computeCenterHistogram() const;
    QRect computePlotRectHistogram() const;
    QPoint computeOriginHistogram() const;

    /* Densityplot */
    QMap<QString, QRect> mapScaleLabelsDensityPlot_;
    QMap<QString, QRect> mapLevelLabelsPositionDensityPlot_;
    QMap<CGroup, QPainterPath> mapGroupCurves_;
    bool bGeometryCacheReadyDensityPlot_ = false;

    QSize computeAxesDensityplot() const;
    QPoint computeCenterDensityPlot() const;
    QRect computePlotRectDensityPlot() const;
    QPoint computeOriginDensityPlot() const;

    /* Violin plot */
    QMap<CGroup, QPainterPath> mapViolinData_;
    QMap<CGroup, QPainterPath> mapViolinMirrorData_;
    QMap<CGroup, QPainterPath> mapViolinBoxPlotData_;
    bool bGeometryCacheReadyViolinPlot_ = false;
    QString strLabelXAxis_;
    QString strLabelYAxis_;
    QMap<QString, QString> mapLabelsViolinPlot_;
    QMap<QString, QPointF> mapValueLabelPositionsViolinPlot_;

    QSize computeAxesViolinplot() const;
    QPoint computeCenterViolinPlot() const;
    QRect computePlotRectViolinPlot() const;
    QPoint computeOriginViolinPlot() const;

    virtual void resizeEvent(QResizeEvent *event);

protected slots:
    void onColorAndFontMapUpdated();

    void onViewMousePressEvent(QMouseEvent* event);
//    void onSetGlobalDomain(QPair<qreal, qreal> pairDomain);

signals:
//    void domainComputed(QPair<qreal, qreal> pairDomain);
};

#endif // BALANCEVIEW_H
