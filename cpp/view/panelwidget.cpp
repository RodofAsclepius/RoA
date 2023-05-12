#include "panelwidget.h"
#include "panellabel.h"
#include <QGridLayout>
#include <QPainter>

CPanelWidget::CPanelWidget(QWidget *parent)
    : QWidget(parent),
      margins_(QMargins(1, 1, 1, 1)),
      pToolBar_(new CToolBar()),
      pGridLayout_(new QGridLayout()),
      pPanelLabel_(new CPanelLabel("Panel")),
      colorBorder_(QColor("#F8F8FF"))
//    colorBorder_(QColor("#F8F8F8"))

{
    updateStyleSheet();
    setContentsMargins(margins_);

    QGridLayout *pLayoutContainer = new QGridLayout();
    pLayoutContainer->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayoutContainer);

    pLayoutContainer->addWidget(pToolBar_, 0, 0, 1, 1);
    pLayoutContainer->setSpacing(0);
    pLayoutContainer->addLayout(pGridLayout_, 1, 0, 1, 1);

    pGridLayout_->setContentsMargins(0, 0, 0, 0);
    pToolBar_->addWidget(pPanelLabel_);

    qreal rScaleFactor = 1;
    rScaleFactor = 1;

    pPanelLabel_->setFixedHeight(rScaleFactor*32);

    pToolBar_->setColorBackground(colorBorder_);
}

CPanelWidget::~CPanelWidget()
{

}

void CPanelWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen(QBrush(colorBorder_), 2);
    painter.setPen(pen);
//    painter.drawRect(rect().adjusted(0, 0, -margins_.right(), -margins_.bottom()));
}

CPanelLabel *CPanelWidget::panelLabel() const
{
    return pPanelLabel_;
}

void CPanelWidget::setPanelLabel(CPanelLabel *pPanelLabel)
{
    pPanelLabel_ = pPanelLabel;
}

void CPanelWidget::updateStyleSheet()
{

}

CToolBar *CPanelWidget::toolBar() const
{
    return pToolBar_;
}

void CPanelWidget::setToolBar(CToolBar *pToolBar)
{
    pToolBar_ = pToolBar;
}

QGridLayout *CPanelWidget::gridLayout() const
{
    return pGridLayout_;
}

void CPanelWidget::setGridLayout(QGridLayout *pGridLayout)
{
    pGridLayout_ = pGridLayout;
}

QColor CPanelWidget::colorBorder() const
{
    return colorBorder_;
}

void CPanelWidget::setColorBorder(const QColor &value)
{
    colorBorder_ = value;
}
