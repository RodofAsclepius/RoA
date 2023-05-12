#ifndef APPEARANCESETTINGSWIDGET_H
#define APPEARANCESETTINGSWIDGET_H

#include <QWidget>
#include <QColor>
#include <QFont>
#include <QMap>

namespace Ui {
class CAppearanceSettingsWidget;
}

class CColorAndFontMap;

class CAppearanceSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CAppearanceSettingsWidget(QWidget *parent = 0);
    ~CAppearanceSettingsWidget();

    void loadSettings();
    void saveSettings() const;

private:
    Ui::CAppearanceSettingsWidget *ui;
protected:
    CColorAndFontMap& _mapColorsAndFonts;
    QMap<QString, QColor> _mapColors;
    QMap<QString, QFont> _mapFonts;

    void updateUI();

private slots:
    void on_treeWidgetColors_doubleClicked(const QModelIndex &index);

    void on_treeWidgetFonts_doubleClicked(const QModelIndex &index);

    void on_pushButtonResetColors_clicked();

    void on_pushButtonResetFonts_clicked();
};

#endif // APPEARANCESETTINGSWIDGET_H
