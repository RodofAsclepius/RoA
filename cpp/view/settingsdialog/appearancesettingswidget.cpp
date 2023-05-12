#include "appearancesettingswidget.h"
#include "ui_appearancesettingswidget.h"
#include "colorandfontmap.h"
#include "utils.h"

#include <QSettings>
#include <QColorDialog>
#include <QFontDialog>



CAppearanceSettingsWidget::CAppearanceSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CAppearanceSettingsWidget),
    _mapColorsAndFonts(CColorAndFontMap::instance())
{
    ui->setupUi(this);

    loadSettings();
    updateUI();
}


CAppearanceSettingsWidget::~CAppearanceSettingsWidget()
{
    delete ui;
}


void CAppearanceSettingsWidget::loadSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TU/e", "Asclepius");

    /* Appearance */
    settings.beginGroup("Appearance");
        /* Colors */
        _mapColors.clear();
        settings.beginGroup("Colors");
        foreach(QString strColorKey, _mapColorsAndFonts.getDefaultColorKeys())
        {
            QVariant varColor = settings.value(strColorKey, QVariant::fromValue(_mapColorsAndFonts.getDefaultColor(strColorKey)));
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
        foreach(QString strFontKey, _mapColorsAndFonts.getDefaultFontKeys())
        {
            QVariant varFont = settings.value(strFontKey, QVariant::fromValue(_mapColorsAndFonts.getDefaultFont(strFontKey)));
            QFont font;
            if(varFont.canConvert<QFont>())
            {
                font = varFont.value<QFont>();
            }

            _mapFonts.insert(strFontKey, font);
        }
        settings.endGroup();
    settings.endGroup();
}


void CAppearanceSettingsWidget::saveSettings() const
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TU/e", "Asclepius");

    /* Appearance */
    settings.beginGroup("Appearance");
        /* Colors */
        settings.beginGroup("Colors");
        QMapIterator<QString, QColor> mapIteratorColors(_mapColors);
        while(mapIteratorColors.hasNext())
        {
            mapIteratorColors.next();
            QString strEntry = mapIteratorColors.key();
            QColor color = mapIteratorColors.value();

            settings.setValue(strEntry, QVariant::fromValue(color));
        }
        settings.endGroup();

        /* Fonts */
        settings.beginGroup("Fonts");
        QMapIterator<QString, QFont> mapIteratorFonts(_mapFonts);
        while(mapIteratorFonts.hasNext())
        {
            mapIteratorFonts.next();
            QString strEntry = mapIteratorFonts.key();
            QFont font = mapIteratorFonts.value();

            settings.setValue(strEntry, QVariant::fromValue(font));
        }
        settings.endGroup();
    settings.endGroup();
}

/*****************************************************************************/
void CAppearanceSettingsWidget::updateUI()
{
    /* Colors */
    ui->treeWidgetColors->clear();
    ui->treeWidgetColors->setColumnCount(3);
    ui->treeWidgetColors->setHeaderLabels(QString("Name; Color").split(";"));

    QMapIterator<QString, QColor> mapIteratorColors(_mapColors);
    while(mapIteratorColors.hasNext())
    {
        mapIteratorColors.next();
        QString strEntry = mapIteratorColors.key();
        QColor color = mapIteratorColors.value();

        QStringList lstLevels = strEntry.split("/");
        QString colorKey = lstLevels.last();
        lstLevels.removeLast();

        QTreeWidgetItem* ptreeWidgetTopLevelItem = new QTreeWidgetItem();
        ptreeWidgetTopLevelItem->setText(0, strEntry);
        ptreeWidgetTopLevelItem->setText(1, " ");
        ptreeWidgetTopLevelItem->setBackground(1, QBrush(QColor::HexArgb));
        ptreeWidgetTopLevelItem->setText(2, color.name(QColor::HexArgb));
        ui->treeWidgetColors->addTopLevelItem(ptreeWidgetTopLevelItem);
    }
    ui->treeWidgetColors->resizeColumnToContents(0);
    ui->treeWidgetColors->resizeColumnToContents(1);
    ui->treeWidgetColors->resizeColumnToContents(2);

    /* Fonts */
    ui->treeWidgetFonts->clear();
    ui->treeWidgetFonts->setColumnCount(2);
    ui->treeWidgetFonts->setHeaderLabels(QString("Name; Font").split(";"));

    QMapIterator<QString, QFont> mapIteratorFonts(_mapFonts);
    while(mapIteratorFonts.hasNext())
    {
        mapIteratorFonts.next();
        QString strEntry = mapIteratorFonts.key();
        QFont font = mapIteratorFonts.value();

        QTreeWidgetItem* ptreeWidgetTopLevelItem = new QTreeWidgetItem();
        ptreeWidgetTopLevelItem->setText(0, strEntry);
        QString strFontDescription = "[" + font.family() + ", " + QString::number(font.pixelSize()) + "]";
        ptreeWidgetTopLevelItem->setText(1, strFontDescription);
        ui->treeWidgetFonts->addTopLevelItem(ptreeWidgetTopLevelItem);
    }
    ui->treeWidgetFonts->resizeColumnToContents(0);
    ui->treeWidgetFonts->resizeColumnToContents(1);
    ui->treeWidgetFonts->resizeColumnToContents(2);
}



void CAppearanceSettingsWidget::on_treeWidgetColors_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    QTreeWidgetItem* pTreeWidgetItem = ui->treeWidgetColors->selectedItems().first();

    QString strColorKey = pTreeWidgetItem->text(0);
    QColor currentColor = _mapColors.value(strColorKey);

    QColorDialog dlgColor(currentColor);
    dlgColor.setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::ShowAlphaChannel);
    dlgColor.setCurrentColor(currentColor);

    if(dlgColor.exec() == QDialog::Accepted)
    {
        QColor color = dlgColor.currentColor();
        _mapColors.insert(strColorKey, color);

        updateUI();
    }
}


void CAppearanceSettingsWidget::on_treeWidgetFonts_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    QTreeWidgetItem* pTreeWidgetItem = ui->treeWidgetFonts->selectedItems().first();

    QString strFontKey = pTreeWidgetItem->text(0);
    QFont currentFont = _mapFonts.value(strFontKey);

    QFontDialog dlgFont(currentFont);
    dlgFont.setOptions(QFontDialog::DontUseNativeDialog);
    dlgFont.setCurrentFont(currentFont);

    if(dlgFont.exec() == QDialog::Accepted)
    {
        QFont font = dlgFont.currentFont();
        _mapFonts.insert(strFontKey, font);

        updateUI();
    }
}


void CAppearanceSettingsWidget::on_pushButtonResetColors_clicked()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TU/e", "Asclepius");

    /* Appearance */
    settings.beginGroup("Appearance");
        /* Colors */
        _mapColors.clear();
        settings.beginGroup("Colors");
        foreach(QString strColorKey, _mapColorsAndFonts.getDefaultColorKeys())
        {
            _mapColors.insert(strColorKey, _mapColorsAndFonts.getDefaultColor(strColorKey));
        }
        settings.endGroup();
    settings.endGroup();

    updateUI();
}


void CAppearanceSettingsWidget::on_pushButtonResetFonts_clicked()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TU/e", "Asclepius");

    /* Appearance */
    settings.beginGroup("Appearance");
        /* Fonts */
        _mapFonts.clear();
        settings.beginGroup("Fonts");
        foreach(QString strFontKey, _mapColorsAndFonts.getDefaultFontKeys())
        {
            _mapFonts.insert(strFontKey, _mapColorsAndFonts.getDefaultFont(strFontKey));
        }
        settings.endGroup();
    settings.endGroup();

    updateUI();
}


