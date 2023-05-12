#include "groupslistdelegatesize.h"

#include <QtWidgets>
#include "group.h"
#include "colorandfontmap.h"

void CGroupsListDelegateSize::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    QVariant var = index.data(Qt::UserRole);
    if(var.canConvert<CGroup>())
    {
        CGroup group = var.value<CGroup>();

        QMargins margins = QMargins(6, 16, 6, 16);
        QRectF rect = option.rect.adjusted(margins.left(), margins.top(),
                                          -margins.right(), -margins.bottom());

        int iGroupSize = group.size();
        int iTotalSize = group.getProperty<int>("maintable_rowcount");
        qreal rRatio = static_cast<qreal>(iGroupSize) / static_cast<qreal>(iTotalSize);

        QRectF rectSizeBar = rect;
        rectSizeBar.setWidth(rectSizeBar.width() * rRatio);

        painter->setRenderHint(QPainter::Antialiasing);
        painter->fillRect(rectSizeBar, group.colorPrimary());

        QPen pen(Qt::darkGray, .5);
        painter->setPen(pen);
        painter->drawRect(rect);

//        QColor contrastColor = CColorAndFontMap::instance().contrastColor(group.color());
//        QPen penText(contrastColor, 1);
//        painter->setPen(penText);
//        painter->drawText(rect, Qt::AlignCenter, QString::number(iGroupSize));
    }
}
