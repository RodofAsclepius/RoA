#include "causalitypanelgraphviewlegend.h"
#include <QGridLayout>
#include "panellabel.h"
#include <QGraphicsBlurEffect>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QVector2D>
#include <QPainterPath>

CCausalityPanelGraphViewLegend::CCausalityPanelGraphViewLegend(QWidget *parent)
    : QWidget(parent)
{
    setContentsMargins(6, 6, 6, 6);

    setFixedWidth(sizeLegend_.width());
    setFixedHeight(sizeLegend_.height());
}

void CCausalityPanelGraphViewLegend::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    QFont fontOriginal = p.font();
    QRect rectAdjusted = rect().adjusted(contentsMargins().left(), contentsMargins().top(), -contentsMargins().right(), -contentsMargins().bottom());
    qreal rArrowWidth = 65;

    /* Draw background and border*/
    QColor clrBackground = QColor(100, 100, 100, 25);
    clrBackground = Qt::white;
    QColor clrBorder = QColor(100, 100, 100, 255);
    p.fillRect(rectAdjusted, clrBackground);

    int iBorderWidth = 2;
    QPen penBorder = QPen(clrBorder, iBorderWidth);
    QRect rectBorder = rectAdjusted.adjusted(0, 0, -iBorderWidth, -iBorderWidth);
    p.setPen(penBorder);
    p.drawRect(rectBorder);

    /* Colors */
    QPen penText(Qt::white, 2);
    QColor color1;
    QColor color2;
    QColor colorEdge = Qt::lightGray;
    QString strGraph1Label;
    QString strGraph2Label;

    /* Comparing group with itself? */
    strGraph1Label = group1().name();
    strGraph2Label = group2().name();

    if(group1_.name() == group2_.name())
    {
        color1 = group1_.colorPrimary();
        color2 = clrMinedGraph_;
    }
    else
    {   // no
        color1 = group1_.colorPrimary();

        if(bFirstGroupIsMinedNetwork_)
        {
            color1 = group1_.colorSecondary();
            color1 = clrMinedGraph_;
            strGraph1Label = group1().name() + " (mined)";
        }

        color2 = group2_.colorPrimary();
        if(bSecondGroupIsMinedNetwork_)
        {
            color2 = clrMinedGraph_;
            strGraph2Label = group1().name() + " (mined)";
        }
    }

    /* Legend */
    QFont fontLegend = p.font();
    fontLegend.setBold(true);
    fontLegend.setPixelSize(14);
    p.setFont(fontLegend);

    QPen penLegend(Qt::black, 1);
    p.setPen(penLegend);
    QString strLegend = "Legend";
    QFontMetrics fmLegendText(fontLegend);
    QPointF pntTextLegend = QPointF(rectAdjusted.left() + 6,
                                    rectAdjusted.top() + fmLegendText.height());
    p.drawText(pntTextLegend, strLegend);

    QFont fontArrowText = p.font();
    fontArrowText.setBold(true);
    fontArrowText.setPixelSize(14);
    p.setFont(fontArrowText);
    QPen penArrowText(color1, 1);

    /* Group arrows */
//    qreal rHorizontalSpacing = 5;
    qreal rVerticalSpacing = 15;
    int iHorizontalOffset = 25;
    int iVerticalOffset = 45;
    int iIndex = 1;
    paintArrow(&p, QPointF(iHorizontalOffset,
                           iVerticalOffset), rArrowWidth, color1, color2, false, colorEdge);
    QFontMetrics fmArrowText(fontArrowText);
    QString strArrowText = strGraph1Label;
    if (bFirstGroupIsMinedNetwork_)
    {
        strArrowText += " (mined)";
    }
    QPointF pntText = QPointF(QPointF(rectAdjusted.center().x() - fmArrowText.horizontalAdvance(strArrowText)/2.0,
                                      iVerticalOffset + 20));
    p.setPen(penArrowText);
    p.drawText(pntText, strArrowText);

    setFixedWidth(sizeLegendCompact_.width());
    sizeLegend_ = QSize(120, 100);
    setFixedHeight(sizeLegend_.height());

    int iShift = 40;

    if(bMultipGroup_)
    {
        sizeLegend_ = QSize(120, 210);
        setFixedHeight(sizeLegend_.height());
        setFixedWidth(sizeLegend_.width());

        /* Comparision group arrow */
        iIndex = 2;
        paintArrow(&p, QPointF(iHorizontalOffset, iVerticalOffset + iShift), rArrowWidth, color2, color1, false, colorEdge);
        QString strArrowText = strGraph2Label;
        if (bSecondGroupIsMinedNetwork_)
        {
            strArrowText += " (mined)";
        }
        QPointF pntText = QPointF(QPointF(rectAdjusted.center().x() - fmArrowText.horizontalAdvance(strArrowText)/2.0,
                                          iVerticalOffset + 20 + iShift));

        penArrowText = QPen(color2, 1);
        p.setPen(penArrowText);
        p.drawText(pntText, strArrowText);

        iShift += 40;

        /* Both groups arrow */
        iIndex = 3;
        paintArrow(&p, QPointF(iHorizontalOffset, iVerticalOffset + iShift), rArrowWidth, clrBothGroups, clrBothGroups, false, colorEdge);
        strArrowText = "Both";
        pntText = QPointF(QPointF(rectAdjusted.center().x() - fmArrowText.horizontalAdvance(strArrowText)/2.0,
                                          iVerticalOffset + 20 + iShift));
        penArrowText = QPen(Qt::black, 1);
        p.setPen(penArrowText);
        p.drawText(pntText, strArrowText);

        iShift += 40;

        /* Opposite arrows */
        iIndex = 4;
        paintArrow(&p, QPointF(iHorizontalOffset, iVerticalOffset + iShift), rArrowWidth, color1, color2, true, colorEdge);
        strArrowText = "Opposite";
        pntText = QPointF(QPointF(rectAdjusted.center().x() - fmArrowText.horizontalAdvance(strArrowText)/2.0,
                                          iVerticalOffset + 20 + iShift));
        penArrowText = QPen(Qt::black, 1);
        p.setPen(penArrowText);
        p.drawText(pntText, strArrowText);
    }
}

bool CCausalityPanelGraphViewLegend::MultipGroupEnabled() const
{
    return bMultipGroup_;
}

void CCausalityPanelGraphViewLegend::setMultipGroupEnabled(bool bMultipGroup)
{
    bMultipGroup_ = bMultipGroup;
    repaint();
}

CGroup CCausalityPanelGraphViewLegend::group1() const
{
    return group1_;
}

void CCausalityPanelGraphViewLegend::setGroup1(const CGroup &group1)
{
    group1_ = group1;
    repaint();
}

CGroup CCausalityPanelGraphViewLegend::group2() const
{
    return group2_;
}

void CCausalityPanelGraphViewLegend::setGroup2(const CGroup &group2)
{
    group2_ = group2;
    repaint();
}

QColor CCausalityPanelGraphViewLegend::colorBothGroups() const
{
    return clrBothGroups;
}

void CCausalityPanelGraphViewLegend::setColorBothGroups(const QColor &value)
{
    clrBothGroups = value;
    repaint();
}

qreal CCausalityPanelGraphViewLegend::arrowLength() const
{
    return arrowLength_;
}

void CCausalityPanelGraphViewLegend::setArrowLength(const qreal &arrowLength)
{
    arrowLength_ = arrowLength;
    repaint();
}

qreal CCausalityPanelGraphViewLegend::edgeWidth() const
{
    return edgeWidth_;
}

void CCausalityPanelGraphViewLegend::setEdgeWidth(const qreal &edgeWidth)
{
    edgeWidth_ = edgeWidth;
    repaint();
}

QColor CCausalityPanelGraphViewLegend::colorArrowBorder() const
{
    return colorArrowBorder_;
}

void CCausalityPanelGraphViewLegend::setColorArrowBorder(const QColor &colorArrowBorder)
{
    colorArrowBorder_ = colorArrowBorder;
    repaint();
}

qreal CCausalityPanelGraphViewLegend::arrowBorderWidth() const
{
    return rArrowBorderWidth_;
}

void CCausalityPanelGraphViewLegend::setArrowBorderWidth(const qreal &rArrowBorderWidth)
{
    rArrowBorderWidth_ = rArrowBorderWidth;
    repaint();
}

bool CCausalityPanelGraphViewLegend::firstGroupIsMinedNetwork() const
{
    return bFirstGroupIsMinedNetwork_;
}

void CCausalityPanelGraphViewLegend::setFirstGroupIsMinedNetwork(bool bFirstGroupAlg)
{
    bFirstGroupIsMinedNetwork_ = bFirstGroupAlg;
    repaint();
}

bool CCausalityPanelGraphViewLegend::secondGroupIsMinedNetwork() const
{
    return bSecondGroupIsMinedNetwork_;
}

void CCausalityPanelGraphViewLegend::setSecondGroupIsMinedNetwork(bool bSecondGroupAlg)
{
    bSecondGroupIsMinedNetwork_ = bSecondGroupAlg;
    repaint();
}

bool CCausalityPanelGraphViewLegend::comparionGroupEnabled() const
{
    return bComparionGroupEnabled_;
}

void CCausalityPanelGraphViewLegend::setComparionGroupEnabled(bool value)
{
    bComparionGroupEnabled_ = value;
}

void CCausalityPanelGraphViewLegend::paintArrow(QPainter *painter, QPointF pntOffset, qreal rWidth, QColor color1, QColor color2,
                                                bool bBidirectional, QColor colorBorder)
{
    /* Draw arrow heads */
    if(bBidirectional)
    {
        qreal arrowWidth = 8;
        QPointF pntTarget = pntOffset + QPointF(rWidth, 0);
        QLineF lineEdge(pntOffset, pntTarget);

        QVector2D vecDir = QVector2D(QPointF(-lineEdge.dy(), lineEdge.dx()));
        vecDir.normalize();

        QPointF pntSource = lineEdge.p1();
        QVector2D vecDirSourceTarget = QVector2D(pntTarget - pntSource);
        vecDirSourceTarget.normalize();
        QVector2D vecDirOrth = QVector2D(QPointF(-lineEdge.dy(), lineEdge.dx()));
        vecDirOrth.normalize();

        QPointF pntSourceOffset = pntSource + (arrowLength_/2.0 * vecDirSourceTarget.toPointF());
        QPointF pntTargetOffset = pntTarget - (arrowLength_/2.0 * vecDirSourceTarget.toPointF());

        QPointF pntSourceOffset2 = pntSourceOffset + (arrowLength_/2.0 * vecDirSourceTarget.toPointF());
        QPointF pntTargetOffset2 = pntTargetOffset - (arrowLength_/2.0 * vecDirSourceTarget.toPointF());

        QPointF pntSourceLeft = pntSourceOffset + vecDirOrth.toPointF() * edgeWidth_/3.0;
        QPointF pntSourceRight = pntSourceOffset + vecDirOrth.toPointF() * -edgeWidth_/3.0;
        QPointF pntTargetLeft = pntTargetOffset + vecDirOrth.toPointF() * edgeWidth_/3.0;
        QPointF pntTargetRight = pntTargetOffset + vecDirOrth.toPointF() * -edgeWidth_/3.0;

        QPointF pntSourceLeft2 = pntSourceOffset2 + vecDirOrth.toPointF() * edgeWidth_/2.0;
        QPointF pntSourceRight2 = pntSourceOffset2 + vecDirOrth.toPointF() * -edgeWidth_/2.0;
        QPointF pntTargetLeft2 = pntTargetOffset2 + vecDirOrth.toPointF() * edgeWidth_/2.0;
        QPointF pntTargetRight2 = pntTargetOffset2 + vecDirOrth.toPointF() * -edgeWidth_/2.0;

        QPolygonF polyGon;
        polyGon << pntSource
                << pntSourceRight
                << pntSourceRight2
                << pntTargetRight2
                << pntTargetRight
                << pntTarget
                << pntTargetLeft
                << pntTargetLeft2
                << pntSourceLeft2
                << pntSourceLeft
                << pntSource;

        QLinearGradient gradient(lineEdge.center() + vecDir.toPointF() * -edgeWidth_,
                                 lineEdge.center() + vecDir.toPointF() * edgeWidth_);
        gradient.setColorAt(0.0, color1);
        gradient.setColorAt(0.49, color1);
        gradient.setColorAt(0.5, color2);
        gradient.setColorAt(1.0, color2);
        QBrush brush(gradient);

        /* Edge*/
        QPen penCurve = QPen(gradient, 1);
        painter->setPen(penCurve);
        painter->setBrush(brush);
        painter->drawPolygon(polyGon);
        QPen dividingLine = QPen(Qt::white, .5);
        painter->setPen(dividingLine);
        painter->drawLine(lineEdge);

        /* Arrow source */
        QPainterPath pathArrowSource = QPainterPath();
        QVector2D vecDirToTarget = QVector2D(lineEdge.p2() - lineEdge.p1());
        vecDirToTarget.normalize();
        QVector2D vecDirOrtho = QVector2D(QPointF(-lineEdge.dy(), lineEdge.dx()));
        vecDirOrtho.normalize();

        QPointF pntArrowBase = pntOffset + (arrowLength_ * vecDirToTarget.toPointF());
        QPointF pntArrowBaseLeft = pntArrowBase - (arrowWidth * vecDirOrtho.toPointF());
        QPointF pntArrowBaseRight = pntArrowBase + (arrowWidth * vecDirOrtho.toPointF());
        pathArrowSource.addPolygon(QPolygonF() << pntOffset << pntArrowBaseLeft << pntArrowBaseRight << pntOffset);

        QPen penArrowSource = QPen(colorArrowBorder_, rArrowBorderWidth_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(penArrowSource);
        QBrush brushArrowSource(color1);
        brushArrowSource.setStyle(Qt::BrushStyle::SolidPattern);
        painter->setBrush(brushArrowSource);
        painter->drawPath(pathArrowSource);

        /* Arrow target */
        QPainterPath pathArrowTarget = QPainterPath();
        QVector2D vecDirToSource = QVector2D(lineEdge.p1() - lineEdge.p2());
        vecDirToSource.normalize();
        QVector2D vecDirOrthoTarget = QVector2D(QPointF(-lineEdge.dy(), lineEdge.dx()));
        vecDirOrthoTarget.normalize();

        QPointF pntArrowBaseTarget = pntTarget + (arrowLength_ * vecDirToSource.toPointF());
        QPointF pntArrowBaseLeftTarget = pntArrowBaseTarget - (arrowWidth * vecDirOrthoTarget.toPointF());
        QPointF pntArrowBaseRightTarget = pntArrowBaseTarget + (arrowWidth * vecDirOrthoTarget.toPointF());
        pathArrowTarget.addPolygon(QPolygonF() << pntTarget << pntArrowBaseLeftTarget << pntArrowBaseRightTarget << pntTarget);

        QPen penArrowTarget= QPen(colorArrowBorder_, rArrowBorderWidth_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(penArrowTarget);
        QBrush brushArrowTarget(color2);
        brushArrowTarget.setStyle(Qt::BrushStyle::SolidPattern);
        painter->setBrush(brushArrowTarget);
        painter->drawPath(pathArrowTarget);
    }
    else
    {
        /* One group */
        qreal arrowWidth = 5;
        QPointF pntTarget = pntOffset + QPointF(rWidth, 0);
        QLineF lineEdge(pntOffset, pntTarget);

        /* Draw edge */
        QVector2D vecDir = QVector2D(QPointF(-lineEdge.dy(), lineEdge.dx()));
        vecDir.normalize();

        QPointF pntSource = lineEdge.p1();
        QVector2D vecDirSourceTarget = QVector2D(pntTarget - pntSource);
        vecDirSourceTarget.normalize();
        QVector2D vecDirOrth = QVector2D(QPointF(-lineEdge.dy(), lineEdge.dx()));
        vecDirOrth.normalize();

        QPointF pntSourceOffset = pntSource + (arrowLength_/2.0 * vecDirSourceTarget.toPointF());
        QPointF pntTargetOffset = pntTarget - (arrowLength_/2.0 * vecDirSourceTarget.toPointF());

        QPointF pntSourceOffset2 = pntSourceOffset + (arrowLength_/2.0 * vecDirSourceTarget.toPointF());
        QPointF pntTargetOffset2 = pntTargetOffset - (arrowLength_/2.0 * vecDirSourceTarget.toPointF());

        QPointF pntSourceLeft = pntSourceOffset + vecDirOrth.toPointF() * edgeWidth_/3.0;
        QPointF pntSourceRight = pntSourceOffset + vecDirOrth.toPointF() * -edgeWidth_/3.0;
        QPointF pntTargetLeft = pntTargetOffset + vecDirOrth.toPointF() * edgeWidth_/3.0;
        QPointF pntTargetRight = pntTargetOffset + vecDirOrth.toPointF() * -edgeWidth_/3.0;

        QPointF pntSourceLeft2 = pntSourceOffset2 + vecDirOrth.toPointF() * edgeWidth_/2.0;
        QPointF pntSourceRight2 = pntSourceOffset2 + vecDirOrth.toPointF() * -edgeWidth_/2.0;
        QPointF pntTargetLeft2 = pntTargetOffset2 + vecDirOrth.toPointF() * edgeWidth_/2.0;
        QPointF pntTargetRight2 = pntTargetOffset2 + vecDirOrth.toPointF() * -edgeWidth_/2.0;

        QPolygonF polyGon;
        polyGon << pntSource
                << pntSourceRight
                << pntSourceRight2
                << pntTargetRight2
                << pntTargetRight
                << pntTarget
                << pntTargetLeft
                << pntTargetLeft2
                << pntSourceLeft2
                << pntSourceLeft
                << pntSource;

        /* Edge*/
        QBrush brush(color1);
        QPen penCurve = QPen(colorArrowBorder_, 1);
        painter->setPen(penCurve);
        painter->setBrush(brush);
        painter->drawPolygon(polyGon);

        /* Arrow Target */
        QPainterPath path = QPainterPath();
        QVector2D vecDirToSource = QVector2D(lineEdge.p1() - lineEdge.p2());
        vecDirToSource.normalize();
        QVector2D vecDirOrthoTarget = QVector2D(QPointF(-lineEdge.dy(), lineEdge.dx()));
        vecDirOrthoTarget.normalize();

        QPointF pntArrowBaseTarget = pntTarget + (arrowLength_ * vecDirToSource.toPointF());
        QPointF pntArrowBaseLeftTarget = pntArrowBaseTarget - (arrowWidth * vecDirOrthoTarget.toPointF());
        QPointF pntArrowBaseRightTarget = pntArrowBaseTarget + (arrowWidth * vecDirOrthoTarget.toPointF());
        path.addPolygon(QPolygonF() << pntTarget << pntArrowBaseLeftTarget << pntArrowBaseRightTarget << pntTarget);

        painter->drawPath(path);
    }
}
