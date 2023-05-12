#include "balanceviewheader.h"
#include "colorandfontmap.h"
#include "utils.h"
#include <QPainter>

#define CHART_MARGIN_LEFT 6
#define CHART_MARGIN_TOP 6
#define CHART_MARGIN_RIGHT 6
#define CHART_MARGIN_BOTTOM 6

using namespace utils;

CBalanceViewHeader::CBalanceViewHeader(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f),
        mapsColorAndFonts_(CColorAndFontMap::instance())
{
    setContentsMargins(0, 0, 0 ,0);
    marPlot_ = QMargins(CHART_MARGIN_LEFT, CHART_MARGIN_TOP, CHART_MARGIN_RIGHT, CHART_MARGIN_BOTTOM);

//    setFixedHeight(20);
    iTickHeight_ = 4;
    iAxisWidth_ = 3;
}

CBalanceViewHeader::~CBalanceViewHeader()
{

}

QMargins CBalanceViewHeader::marginsPlot() const
{
    return marPlot_;
}

void CBalanceViewHeader::setMarginsPlot(const QMargins &marPlot)
{
    marPlot_ = marPlot;
}

void CBalanceViewHeader::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    /* Background */
    QColor altRowColor = palette().alternateBase().color();
    painter.fillRect(rect(), altRowColor);

    QPen penAxis(Qt::lightGray, iAxisWidth_);
    painter.setPen(penAxis);
    /* Axis */
    painter.drawLine(0, height(), width(), height());
    painter.drawLine(width()/2, height(), width()/2, height() - iTickHeight_);
    painter.drawLine(0, height(), 0, height() - height()/2);
    painter.drawLine(width(), height(), width(), height() - height()/2);

    /* Ticks and labels */
    QPen penAxisLabels(Qt::black, 1);
    QFont fontLabels = painter.font();
    fontLabels.setPointSize(6);
    fontLabels.setBold(false);
    painter.setFont(fontLabels);
    QMapIterator<QString, qreal> mapIt(mapTicksLabels_);
    while(mapIt.hasNext())
    {
        mapIt.next();
        QString strLabel = mapIt.key();
        int qXValue = mapIt.value();

        painter.setPen(penAxis);
        painter.drawLine(qXValue, height(), qXValue, height() - iTickHeight_);

        painter.save();
        QPoint anchorLabel = QPoint(qXValue, height() - 3* iTickHeight_);

        painter.translate(anchorLabel + QPoint(0, 7));
        painter.rotate(-45);
        painter.setPen(penAxisLabels);
        painter.drawText(QPoint(0, 0), strLabel);
        painter.restore();
    }

//    painter.drawText(QPoint(width()/2 - 15, height() - 2* iTickHeight_), "MSD");
}

QMap<QString, qreal> CBalanceViewHeader::ticksLabels() const
{
    return mapTicksLabels_;
}

void CBalanceViewHeader::setTicksLabels(const QMap<QString, qreal> &mapTicksLabels)
{
    mapTicksLabels_ = mapTicksLabels;

    repaint();
}

void CBalanceViewHeader::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}


