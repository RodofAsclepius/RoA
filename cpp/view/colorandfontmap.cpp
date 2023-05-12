
#include "colorandfontmap.h"

#include <QSettings>
#include <QDebug>
#include <QRandomGenerator>


CColorAndFontMap::~CColorAndFontMap()
{

}


void CColorAndFontMap::reloadSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TU/e", "Asclepius");

    /* Appearance */
    settings.beginGroup("Appearance");
        /* Colors */
        _mapColors.clear();
        settings.beginGroup("Colors");
        foreach(QString strColorKey, getDefaultColorKeys())
        {
            QVariant varColor = settings.value(strColorKey, QVariant::fromValue(getDefaultColor(strColorKey)));
            QColor color;
            if(varColor.canConvert<QColor>())
            {
                color = varColor.value<QColor>();
            }

            _mapColors.insert(strColorKey, color);
        }
        settings.endGroup();

        /* Fonts */
        _mapFonts.clear();
        settings.beginGroup("Fonts");
        foreach(QString strFontKey, getDefaultFontKeys())
        {
            QVariant varFont = settings.value(strFontKey, QVariant::fromValue(getDefaultFont(strFontKey)));
            QFont font;
            if(varFont.canConvert<QFont>())
            {
                font = varFont.value<QFont>();
            }

            _mapFonts.insert(strFontKey, font);
        }
        settings.endGroup();
    settings.endGroup();

    emit updated();
}


void CColorAndFontMap::insert(const QString &strName, const QColor &color)
{
    _mapColors[strName] = color;
}


void CColorAndFontMap::insert(const QString &strName, const QFont &font)
{
    _mapFonts[strName] = font;
}


QColor CColorAndFontMap::getColor(const QString &strName) const
{
    if(!_mapColors.contains(strName))
    {
        qDebug() << "Color for " << strName << " not found...";
    }

    return _mapColors[strName];
}


QFont CColorAndFontMap::getFont(const QString &strName) const
{
    if(!_mapFonts.contains(strName))
    {
        qDebug() << "Font for " << strName << " not found...";
    }

    return _mapFonts[strName];
}


QJsonObject CColorAndFontMap::toJsonObject() const
{
    QJsonObject jsonObject;

    QStringList lstKeys = _mapColors.keys();
    foreach(QString strKey, lstKeys)
    {
        jsonObject[strKey] = _mapColors.value(strKey).name();
    }

    return jsonObject;
}

QColor CColorAndFontMap::category12Pallete1(int iCategoryNumber)
{
    /* Based on colorbrewer */
    QColor colorResult;

    iCategoryNumber %= 12;
    switch(iCategoryNumber)
    {
        case 0:
            colorResult = QColor(141, 211, 199);
        break;
        case 1:
            colorResult = QColor(255, 255, 179);
        break;
        case 2:
            colorResult = QColor(190,186,218);
        break;
        case 3:
            colorResult = QColor(251,128,114);
        break;
        case 4:
            colorResult = QColor(128, 177, 211);
        break;
        case 5:
            colorResult = QColor(253, 180, 98);
        break;
        case 6:
            colorResult = QColor(179,222,105);
        break;
        case 7:
            colorResult = QColor(252,205,229);
        break;
        case 8:
            colorResult = QColor(217,217,217);
        break;
        case 9:
            colorResult = QColor(188,128,189);
        break;
        case 10:
            colorResult = QColor(204,235,197);
        break;
        case 11:
            colorResult = QColor(255,237,111);
        break;
    }

    return colorResult;
}

QColor CColorAndFontMap::category12Pallete2(int iCategoryNumber)
{
    /* Based on colorbrewer */
    QColor colorResult;

    iCategoryNumber %= 12;
    switch(iCategoryNumber)
    {
        case 0:
            colorResult = QColor(166,206,227);
        break;
        case 1:
            colorResult = QColor(31,120,180);
        break;
        case 2:
            colorResult = QColor(178,223,138);
        break;
        case 3:
            colorResult = QColor(51,160,44);
        break;
        case 4:
            colorResult = QColor(251,154,153);
        break;
        case 5:
            colorResult = QColor(227,26,28);
        break;
        case 6:
            colorResult = QColor(253,191,111);
        break;
        case 7:
            colorResult = QColor(255,127,0);
        break;
        case 8:
            colorResult = QColor(202,178,214);
        break;
        case 9:
            colorResult = QColor(106,61,154);
        break;
        case 10:
            colorResult = QColor(255,255,153);
        break;
        case 11:
            colorResult = QColor(177,89,40);
        break;
    }

    return colorResult;
}


QList<QString> CColorAndFontMap::getColorKeys() const
{
    return _mapFonts.keys();
}


QList<QString> CColorAndFontMap::getFontKeys() const
{
    return _mapColors.keys();
}


QColor CColorAndFontMap::getDefaultColor(const QString &strName) const
{
    if(!_mapDefaultColors.contains(strName))
    {
        qDebug() << "Default color for " << strName << " not found...";
    }

    return _mapDefaultColors[strName];
}

QFont CColorAndFontMap::getDefaultFont(const QString &strName) const
{
    if(!_mapDefaultFonts.contains(strName))
    {
        qDebug() << "Default font for " << strName << " not found...";
    }

    return _mapDefaultFonts[strName];
}

QList<QString> CColorAndFontMap::getDefaultColorKeys() const
{
    return _mapDefaultColors.keys();
}

QList<QString> CColorAndFontMap::getDefaultFontKeys() const
{
    return _mapDefaultFonts.keys();
}

CColorAndFontMap::CColorAndFontMap()
{
    /* ***** Colors ***** */

    /* widgets */
//    _mapDefaultColors.insert("widget/mainwindow", QColor("#525252"));
//    _mapDefaultColors.insert("widget/generic", QColor("#525252"));
//    _mapDefaultColors.insert("widget/chart", QColor("#313131"));

    _mapDefaultColors.insert("widget/mainwindow", QColor("white"));
    _mapDefaultColors.insert("widget/generic", QColor("white"));
    _mapDefaultColors.insert("widget/chart", QColor("white"));

    /* Color brewer colors */
    /* Qual 12 */
//    QColor clrLightBlue = QColor(66,206,227);
//    QColor clrBlue = QColor(31,120,180);

//    QColor clrLightGreen = QColor(178,223,138);
//    QColor clrGreen = QColor(51,160,44);

//    QColor clrLightRed = QColor(251,154,153);
//    QColor clrRed = QColor(227,26,28);

//    QColor clrLightOrange = QColor(253,191,111);
//    QColor clrOrange = QColor(255,127,0);

//    QColor clrLightPurple = QColor(202,178,214);
//    QColor clrPurple = QColor(106,61,154);

//    QColor clrLightBrownYellow = QColor(255,255,153);
//    QColor clrBrown = QColor(177,89,40);

    /* Color brewer qual 8 */
    QColor clrRed = QColor(228,26,28);
    QColor clrLightRed = QColor(251,154,153);

    QColor clrBlue = QColor(55,126,184);
    QColor clrLightBlue = QColor(66,206,227);

    QColor clrGreen = QColor(77,175,74);
    QColor clrLightGreen = QColor(178,223,138);

    QColor clrPurple = QColor(152,78,163);
    QColor clrLightPurple = QColor(202,178,214);

    QColor clrOrange = QColor(255,127,0);
    QColor clrLightOrange = QColor(253,191,111);

    QColor clrYellow = QColor(255,255,51);
    QColor clrBrown = QColor(166,86,40);
    QColor clrPink = QColor(247,129,191);

    /* Color brewer qual 8 pal 4 */
    QColor clrOK = QColor(204,235,197);
    QColor clrAlert = clrRed;
    QColor clrWarning = QColor(255,255,204);

    /* Generic */
    _mapDefaultColors.insert("generic/ok", clrOK);
    _mapDefaultColors.insert("generic/alert", clrAlert);
    _mapDefaultColors.insert("generic/warning", clrWarning);

    _mapDefaultColors.insert("identifiability/blockedpath", clrLightRed);
    _mapDefaultColors.insert("identifiability/adjustablepath", clrLightGreen);

//    _mapDefaultColors.insert("general/selection", QColor(255,140,0));
//    _mapDefaultColors.insert("general/hover", QColor(255,140,0));
//    _mapDefaultColors.insert("general/highlight", QColor(255,140,0));

    /* Groups */
    _mapDefaultColors.insert("groups/1/primary", clrBlue); /* Blue */
    _mapDefaultColors.insert("groups/1/secondary",clrLightBlue);
    _mapDefaultColors.insert("groups/2/primary", clrOrange);
    _mapDefaultColors.insert("groups/2/secondary", clrLightOrange);
    _mapDefaultColors.insert("groups/3/primary", clrPurple);
    _mapDefaultColors.insert("groups/3/secondary", clrLightPurple);

    /* Distributions */
    _mapDefaultColors.insert("distribution/background", Qt::white); // 204,234,170 (green-yellow)
    _mapDefaultColors.insert("distribution/axis", QColor(255,255,204)); // gray

    /* Panel Labels */
    _mapDefaultColors.insert("label/panel/level/1", QColor(0, 0, 0));
    _mapDefaultColors.insert("label/panel/level/2", QColor(0, 0, 0));
    _mapDefaultColors.insert("label/panel/level/3", QColor(0, 0, 0));

    /* ***** Fonts ***** */
    QFont fontGeneralDefault("Calibri");
    _mapDefaultFonts.insert("general/default", fontGeneralDefault);

//    QFont fontPanelLabel1( "Calibri", 16, QFont::Bold);
    QFont fontPanelLabel1( "Calibri", 16);
    _mapDefaultFonts.insert("label/panel/level/1", fontPanelLabel1);

//    QFont fontPanelLabel2( "Calibri", 12, QFont::Bold);
    QFont fontPanelLabel2( "Calibri", 12);
    _mapDefaultFonts.insert("label/panel/level/2", fontPanelLabel2);

    QFont fontBartChartLabels("Calibri");
    _mapDefaultFonts.insert("plots/bar/font/labels", fontBartChartLabels);

    QFont fontBartChartTips("Calibri");
    _mapDefaultFonts.insert("plots/bar/font/tips", fontBartChartTips);

    /* Initialize the maps */
    _mapColors = _mapDefaultColors;
    _mapFonts = _mapDefaultFonts;

    reloadSettings();
}

QColor CColorAndFontMap::contrastColor(const QColor &color)
{
    int d = 0;

    // Counting the perceptive luminance - human eye favors green color...
    double luminance = ( 0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue())/255;

    if (luminance > 0.5)
       d = 0; // bright colors - black font
    else
       d = 255; // dark colors - white font

    return  QColor::fromRgb(d, d, d);
}

QColor CColorAndFontMap::randomColor() const
{
    int r = QRandomGenerator::global()->bounded(255);
    int g = QRandomGenerator::global()->bounded(255);
    int b = QRandomGenerator::global()->bounded(255);
    QColor color(r,g,b);

    return color;
}

