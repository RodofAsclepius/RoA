#include "balancepaneltreeheader.h"
#include "group.h"
#include <QPainter>
#include <QDebug>

CBalancePanelTreeHeader::CBalancePanelTreeHeader(Qt::Orientation orientation, QWidget *parent)
    : QHeaderView(orientation, parent)
{
}

CBalancePanelTreeHeader::~CBalancePanelTreeHeader()
{

}

void CBalancePanelTreeHeader::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    QVariant varData = model()->headerData(logicalIndex, orientation(), Qt::DisplayRole);

//    QRectF rectTarget = QRectF(0, 0, 16, 16);
//    rectTarget.setTopLeft(rect.center() - QPointF(8, 0));

    if(varData.canConvert<CGroup>())
    {
        CGroup group = varData.value<CGroup>();

        painter->fillRect(rect, QBrush(group.colorPrimary()));
        QImage imgWeight = QImage(":/view/images/weight.png");
        painter->drawImage(rect, imgWeight, imgWeight.rect(), Qt::AutoColor);
    }
    else
    {
        QHeaderView::paintSection(painter, rect, logicalIndex);
    }
}
