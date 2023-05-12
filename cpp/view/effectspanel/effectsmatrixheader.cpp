#include "effectsmatrixheader.h"
#include "colorandfontmap.h"
#include <QPaintEvent>

#include <QPainter>
#include <QSize>

#include <QDebug>
#include <QModelIndex>

CEffectsMatrixHeader::CEffectsMatrixHeader()
    : QHeaderView(Qt::Horizontal),
      _mapColorsFonts(CColorAndFontMap::instance())
{

}

void CEffectsMatrixHeader::paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const
{
    QPen pen(Qt::black);
    painter->setPen(pen);

    painter->translate(sectionViewportPosition(logicalIndex) + sectionSize(logicalIndex) / 2, height() - 9);
    painter->rotate(-45);
    QRect copy = QRect(0, 0, 150, 16);
    painter->drawText(0, 0, model()->headerData(logicalIndex, Qt::Horizontal, Qt::DisplayRole).toString());
}

void CEffectsMatrixHeader::paintEvent(QPaintEvent *e)
{
    QPainter painter(this->viewport());

    QColor clrBackground = _mapColorsFonts.getColor("widget/generic").name();
    painter.fillRect(this->viewport()->rect(), QBrush(clrBackground));

    QModelIndexList selectedIndices = selectionModel()->selection().indexes();

    QVector<bool> rowSelections = QVector<bool>(model()->columnCount());
    foreach(QModelIndex index, selectedIndices)
    {
        rowSelections[index.column()] = true;
    }

    for(int i=0; i<model()->columnCount(); ++i)
    {
        painter.save();
        QFont font = painter.font();
        font.setBold(false);
        if(rowSelections[i])
        {
            font.setBold(true);
        }
        painter.setFont(font);
        paintSection(&painter, QRect(), i);
        painter.restore();
    }
}
