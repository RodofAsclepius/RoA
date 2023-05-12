#ifndef COLORMAP_H
#define COLORMAP_H


#include <QObject>
#include <QMap>
#include <QString>
#include <QColor>
#include <QJsonObject>
#include <QFont>



class CColorAndFontMap : public QObject
{
    Q_OBJECT
public:
    /* Singleton definition */
    static CColorAndFontMap& instance()
    {
        static CColorAndFontMap instance;
        return instance;
    }
    ~CColorAndFontMap();

    void reloadSettings();

    void insert(const QString& strName, const QColor& getColor);
    void insert(const QString& strName, const QFont& getFont);

    QColor getColor(const QString& strName) const;
    QFont getFont(const QString& strName) const;
    QList<QString> getColorKeys() const;
    QList<QString> getFontKeys() const;

    QColor getDefaultColor(const QString& strName) const;
    QFont getDefaultFont(const QString& strName) const;
    QList<QString> getDefaultColorKeys() const;
    QList<QString> getDefaultFontKeys() const;

    QJsonObject toJsonObject() const;

    QColor category12Pallete1(int iCategoryNumber);
    QColor category12Pallete2(int iCategoryNumber);

    QColor contrastColor(const QColor &color);

    QColor randomColor() const;
protected:
    QMap<QString, QColor> _mapColors;
    QMap<QString, QFont> _mapFonts;
    QMap<QString, QColor> _mapDefaultColors;
    QMap<QString, QFont> _mapDefaultFonts;
private:
    CColorAndFontMap();
signals:
    void updated();

public slots:
};


#endif // COLORMAP_H
