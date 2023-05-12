#ifndef CCAUSALITYPANELLEGEND_H
#define CCAUSALITYPANELLEGEND_H

#include <QDialog>
#include "group.h"

class CCausalityPanelGraphViewLegend : public QWidget
{
    Q_OBJECT
public:
    explicit CCausalityPanelGraphViewLegend(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *e) override;
    bool MultipGroupEnabled() const;
    void setMultipGroupEnabled(bool MultipGroupEnabled);

    CGroup group1() const;
    void setGroup1(const CGroup &group1);

    CGroup group2() const;
    void setGroup2(const CGroup &group2);

    QColor colorBothGroups() const;
    void setColorBothGroups(const QColor &value);

    qreal arrowLength() const;
    void setArrowLength(const qreal &arrowLength);

    qreal edgeWidth() const;
    void setEdgeWidth(const qreal &edgeWidth);

    QColor colorArrowBorder() const;
    void setColorArrowBorder(const QColor &colorArrowBorder);

    qreal arrowBorderWidth() const;
    void setArrowBorderWidth(const qreal &rArrowBorderWidth);
    
    bool firstGroupIsMinedNetwork() const;
    void setFirstGroupIsMinedNetwork(bool bFirstGroupAlg);

    bool secondGroupIsMinedNetwork() const;
    void setSecondGroupIsMinedNetwork(bool bSecondGroupAlg);

    bool comparionGroupEnabled() const;
    void setComparionGroupEnabled(bool value);

protected:
    qreal rScaleFactor_ = 1;

    bool bMultipGroup_ = false;
    bool bFirstGroupIsMinedNetwork_ = false;
    bool bSecondGroupIsMinedNetwork_ = false;
    CGroup group1_;
    CGroup group2_;
    QColor clrBothGroups = Qt::white;
    QColor clrMinedGraph_ = Qt::darkGray;
    qreal arrowLength_ = rScaleFactor_*16;
    qreal edgeWidth_ = rScaleFactor_*3;
    QColor colorArrowBorder_ = Qt::black;
//    QColor colorArrowBorder_ = Qt::white;
    qreal rArrowBorderWidth_ = rScaleFactor_*1;
//    QSize sizeLegend_ = QSize(150, 300);
    QSize sizeLegend_ = QSize(120, 100);
    QSize sizeLegendCompact_ = QSize(120, 210);
    bool bComparionGroupEnabled_ = false;

    void paintArrow(QPainter* painter, QPointF pntOffset, qreal rWidth, QColor color1, QColor color2,
                    bool bBidirectional = false, QColor colorBorder = Qt::white);
signals:

};

#endif // CCAUSALITYPANELLEGEND_H
