#ifndef EFFECTSSMATRIXHEADER_H
#define EFFECTSSMATRIXHEADER_H

#include <QHeaderView>

class CColorAndFontMap;

class CEffectsMatrixHeader : public QHeaderView
{
public:
    CEffectsMatrixHeader();

protected:
    CColorAndFontMap& _mapColorsFonts;
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
    void paintEvent(QPaintEvent *e) override;

};

#endif // EFFECTSSMATRIXHEADER_H
