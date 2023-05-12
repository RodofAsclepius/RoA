#include "panellabel.h"
#include "colorandfontmap.h"
#include "utils.h"

#include <QTextEdit>
#include <QPalette>
#include <QLayout>

CPanelLabel::CPanelLabel(const QString &text, int iLevel)
    : QLabel(text),
      _mapColorsAndFonts(CColorAndFontMap::instance()),
      iLevel(iLevel)
{
    setContentsMargins(0, 0, 0, 0);
    setStyle();
    connect(&_mapColorsAndFonts, &CColorAndFontMap::updated, this, &CPanelLabel::onColorAndFontMapUpdated);
}


CPanelLabel::~CPanelLabel()
{

}


void CPanelLabel::setStyle()
{
    QFont font;
    QColor color;

    switch(iLevel)
    {
        case 2:
            color = _mapColorsAndFonts.getColor("label/panel/level/2");
            font = _mapColorsAndFonts.getFont("label/panel/level/2");
        break;
        default:
            color = _mapColorsAndFonts.getColor("label/panel/level/1");
            font = _mapColorsAndFonts.getFont("label/panel/level/1");
        break;
    }

    font.setPointSizeF(_rTextScaleFactor * font.pointSizeF());

    setFont(font);
//    color = Qt::lightGray;
    color = Qt::black;
    setStyleSheet("color: " + color.name());
    QWidget::setAttribute(Qt::WA_TranslucentBackground);
}

void CPanelLabel::setTextScaleFactor(const qreal &rTextScaleFactor)
{
    _rTextScaleFactor = rTextScaleFactor;
}

qreal CPanelLabel::textScaleFactor() const
{
    return _rTextScaleFactor;
}

void CPanelLabel::onColorAndFontMapUpdated()
{
    setStyle();
}


