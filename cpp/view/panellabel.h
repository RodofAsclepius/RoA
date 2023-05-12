#ifndef PANELLABEL_H
#define PANELLABEL_H


#include <QLabel>


class QTextEdit;
class CColorAndFontMap;



class CPanelLabel : public QLabel
{
public:
    CPanelLabel(const QString &text = QString(), int iLevel=1);
    ~CPanelLabel();

    void setStyle();
    qreal textScaleFactor() const;
    void setTextScaleFactor(const qreal &textScaleFactor);

protected:
    CColorAndFontMap& _mapColorsAndFonts;
    QTextEdit* _pTextEdit;
    int iLevel;
    qreal _rTextScaleFactor = 1.3;

    void onColorAndFontMapUpdated();
};


#endif // PANELLABEL_H
