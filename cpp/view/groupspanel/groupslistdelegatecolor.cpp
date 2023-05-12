#include "groupslistdelegatecolor.h"

#include <QtWidgets>
#include "group.h"

void CGroupsListDelegateColor::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    QVariant var = index.data(Qt::UserRole);
    if(var.canConvert<CGroup>())
    {
        CGroup group = var.value<CGroup>();
        QMargins margins = QMargins(6, 6, 6, 6);
        QRectF rect = option.rect.adjusted(margins.left(), margins.top(),
                                          -margins.right(), -margins.bottom());

        QRectF rectTop = rect;
        rectTop.setBottom(rectTop.center().y());

        QRectF rectBottom = rect;
        rectBottom.setTop(rectBottom.center().y());

        painter->setRenderHint(QPainter::Antialiasing);

        QPen pen(Qt::black, .1);
        painter->setPen(pen);

        /* Primary color */
        QPainterPath pathTop;
//        pathTop.addRoundedRect(rectTop, 4, 4);
        pathTop.addRect(rectTop);
        painter->fillPath(pathTop, group.colorPrimary());

        /* Secondary color */
        QPainterPath pathBottom;
//        pathBottom.addRoundedRect(rectBottom, 4, 4);
        pathBottom.addRect(rectBottom);
        painter->fillPath(pathBottom, group.colorSecondary());

        QPen penBorder(Qt::black, 1);
        painter->setPen(penBorder);
        painter->drawRect(rect);
    }





//    painter->setRenderHint(QPainter::Antialiasing, true);
//    QVariant var = index.data(Qt::UserRole);
//    if(var.canConvert<CGroup>())
//    {
//        CGroup group = var.value<CGroup>();
//        QMargins margins = QMargins(6, 6, 6, 6);
//        QRectF rect = option.rect.adjusted(margins.left(), margins.top(),
//                                          -margins.right(), -margins.bottom());

//        painter->setRenderHint(QPainter::Antialiasing);
//        QPen pen(Qt::black, .1);
//        painter->setPen(pen);

//        /* Primary color */
//        QPainterPath path;
//        path.addRoundedRect(rect, 4, 4);
//        painter->fillPath(path, group.colorPrimary());

//        /* Secondary color */
//        QPainterPath pathPolygon;
//        QPolygonF polygon;
//        polygon << rect.topLeft() << rect.bottomRight() << rect.topRight();
//        pathPolygon.addPolygon(polygon);

//        QPainterPath pathClipped = path.subtracted(pathPolygon);
////        pathClip.addPolygon(rect.topLeft(), rect.bottomLeft(), rect.topRight());
//        painter->fillPath(pathClipped, group.colorSecondary());

//        painter->drawPath(path);
//    }
}
