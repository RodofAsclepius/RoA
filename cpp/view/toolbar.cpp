#include "toolbar.h"
#include <QHBoxLayout>
#include <QPainter>

CToolBar::CToolBar(QWidget *parent)
    : QWidget(parent)
{
    setContentsMargins(6, 0, 6, 0);

    pLayout_ = new QHBoxLayout();
    pLayout_->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout_);

    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, colorBackground_);
    setPalette(pal);
    setFixedHeight(48);
}

CToolBar::~CToolBar()
{

}

void CToolBar::addWidget(QWidget *pWidget)
{
    pLayout_->addWidget(pWidget);
}

void CToolBar::addLayout(QLayout *pLayout)
{
    pLayout_->addLayout(pLayout);
}

void CToolBar::addItem(QLayout *pItem)
{
    pLayout_->addItem(pItem);
}

void CToolBar::addSpacer(QSpacerItem *pSpacerItem)
{
    if(pSpacerItem == nullptr)
    {
        pSpacerItem = new QSpacerItem(10, height(), QSizePolicy::Expanding, QSizePolicy::Fixed);
    }

    pLayout_->addItem(pSpacerItem);
}

QColor CToolBar::colorBackground() const
{
    return colorBackground_;
}

void CToolBar::setColorBackground(const QColor &colorBackground)
{
//    colorBackground_ = colorBackground;
}

void CToolBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QBrush brush = QBrush(QColor(colorBackground_));
    painter.fillRect(rect(), brush);
}
