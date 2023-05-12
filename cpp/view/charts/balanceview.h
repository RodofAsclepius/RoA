#ifndef BALANCEVIEW_H
#define BALANCEVIEW_H

#include <QWidget>
#include "balancemodelslist.h"

#define HDI_SCALING 1

class CColorAndFontMap;
class CExtendedGraphicsView;
class CBalanceViewHeader;

class CBalanceView : public QWidget
{
    Q_OBJECT
public:
    CBalanceView(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~CBalanceView() override;

    CBalanceModelsList balanceModels() const;
    void setBalanceModels(const CBalanceModelsList &balanceModels);

    void render(bool bUpdateGeometry = false);
    void renderLovePlotCurves(bool bUpdateGeometry = false);
    qreal scaleXLovePlot(qreal rX) const;

    void clear();

    void updateView(bool bUpdateGeometry = false);

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

    bool drawAxes() const;
    void setDrawAxes(bool value);

    void clearGeometryCache();
    void updateGeometryCacheLovePlot();
    void clearGeometryCacheLovePlot();

    OrientationMode orientationMode() const;
    void setOrientationMode(const OrientationMode &orientationMode);

    bool drawValueLabels() const;
    void setDrawValueLabels(bool value);

    bool geometryCacheReadyLovePlot() const;
    void setGeometryCacheReadyLovePlot(bool geometryCacheReadyLovePlot);

    OrientationMode orientationModeLovePlot() const;
    void setOrientationModeLovePlot(const OrientationMode &orientationModeLovePlot);

    QPair<CVariablesList, QVector<QStringList> > variablesAndLevels() const;
    void setVariablesAndLevels(const QPair<CVariablesList, QVector<QStringList> > &variablesAndLevels);

    bool showUnadjustedValues() const;
    void setShowUnadjustedValues(bool showUnadjustedValues);

    int rowHeight() const;
    void setRowHeight(int rowHeight);

    void translateY(qreal ry);

    CExtendedGraphicsView *pView() const;
    void setPView(CExtendedGraphicsView *pView);

    bool getLogarithmicScaling() const;
    void setLogarithmicScaling(bool value);

    CBalanceViewHeader *header() const;

protected:
    QMutex mutex_;
    CBalanceViewHeader *pHeader_;

    /* Data */
    CColorAndFontMap& mapsColorAndFonts_;
    CBalanceModelsList lstBalanceModels_;
    QPair<CVariablesList, QVector<QStringList>> vecVariablesAndLevels_;

    QMap<CGroup, QPainterPath> mapLoveLinesUn_;
    QMap<CGroup, QPainterPath> mapLoveLinesAdj_;
    QMap<CGroup, QPainterPath> mapLoveCurvesUn_;
    QMap<CGroup, QPainterPath> mapLoveCurvesAdj_;
    QMap<CGroup, QPainterPath> mapLoveDotsUn_;
    QMap<CGroup, QPainterPath> mapLoveDotsAdj_;
    QMap<CGroup, QPainterPath> mapLoveGridLines_;

    QMap<CGroup, QVector<QPointF>> mapGroupCoordinatesUn_;
    QMap<CGroup, QVector<QPointF>> mapGroupCoordinatesAdj_;
    QMap<CGroup, QVector<QVariantMap>> mapGroupCoordinatesDataUn_;
    QMap<CGroup, QVector<QVariantMap>> mapGroupCoordinatesDataAdj_;

    /* Display options */
    QMargins marPlot_;
    QMargins marLabels_;
    int iTranslationX_ = 0;
    int iTranslationY_ = 0;
    qreal rCellOffset_ = 0;
    int iTotalSceneHeight_ = 0;

    OrientationMode orientationModeLovePlot_ = North;
    ScalingMode scalingModeHorizontal_ = Maximize;
    ScalingMode scalingModeVertical_ = Maximize;

    bool bDrawBorder = false;
    bool bDrawBorderPlot = false;
    bool bDrawAxes = true;
    bool bDrawValueLabels = true;
    bool bDrawLevelLabelsHistogram = true;
    bool bLogarithmicScaling = true;

    QColor clrBackground_ = QColor("#313131");

    /* LoveLines */
//    QColor clrBackgroundLovePlot = QColor("#313131");
    QColor clrBackgroundLovePlot = Qt::white;
    QColor clrAxisLovePlot_ = Qt::black;
    QColor clrPenDefaultLovePlot_ = Qt::black;

    bool bShowUnadjustedValues_ = true;
    QPointF _ptHorizontalRange;
    int iRowHeight_ = 45;
    QMarginsF _margin = QMarginsF(0, 0, 0, 0);
    qreal rHorizontalAxisMin_ = 0.0,
          rHorizontalAxisMax_ = 1,
          rInnerTreshold_ = 0.1,
          rOuterTreshold_ = 0.25;

    qreal rPointSizeBalanced_ = 5,
          rPointSizeUnBalanced_ = 10,
          rPointEdgeWidth_ = 1.5,
          rCurveWidth_ = 4;
    QColor _colorPoint,
           _colorCurve;
    QBrush _brushPoint;

//    void generateLoveCurvesOld();
    void generateLoveCurves();

    /* Histogram */
    int iBarSpacingHistogram_ = 2;
    int iBarWidthHistogram_ = 10 * 2;
    int iLevelSpacingHistogram_ = 12;
    int iBarLineWidthHistogram_ = 1;
    int iLevelLabelOffsetVertical_ = 30;
    int iLevelLabelOffsetHorizontal_ = 26;
    int iLevelLabelheight = 26;
    int iValueLabelOffsetVertical_ = 10;
    int iValueLabelOffsetHorizontal_ = 20;
    int iValueLabelHeight = 26;
    int iLevelLabelFontSize = 12;
    int iValueLabelFontSize = 8;

    /* Density plot */
    qreal iDensityPlotWidth = 2;
    /* Axes */
    int iAxesWidth_ = 1;

    /* Render */
    CExtendedGraphicsView *pView_;
    QRect computeViewPortRect() const;

    /* LovePlot */
    QMap<QString, QMap<CGroup, qreal>> mapDataLovePlot_;
    QVector<QRect> vecBarsLovePlot_;
    QVector<QColor> vecBarColorsLovePlot_;
    QMap<QString, QRect> mapLevelLabelsPositionLovePlot_;
    QVector<QString> vecValueLabelsLovePlot_;
    QVector<QRect> vecValueLabelPositionsLovePlot_;
    QVector<QString> vecToolTipsLovePlot_;
    QPair<qreal, qreal> computeMinMaxLovePlotValues() const;
    bool bGeometryCacheReadyLovePlot_ = false;

    QSize computeAxesLovePlot() const;
    QPoint computeCenterLovePlot() const;
    QRect computePlotRectLovePlot() const;
    QPoint computeOriginLovePlot() const;

    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

protected slots:
    void onColorAndFontMapUpdated();
    void onViewMousePressEvent(QMouseEvent* event);
};

#endif // BALANCEVIEW_H
