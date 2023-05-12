#include "splitmergebutton.h"
#include "maincontroller.h"
#include "colorandfontmap.h"

CSplitMergeButton::CSplitMergeButton(QWidget *pParent)
    : QPushButton(pParent),
      mainController_(CMainController::instance()),
      mapColorsAndFonts_(CColorAndFontMap::instance())
{
    connect(this, &QPushButton::clicked, this, &CSplitMergeButton::onClicked);

//    setFlat(true);
    setFixedSize(18, 18);
    updateButton();

//    QWidget::setAttribute(Qt::WA_TranslucentBackground);
}

CSplitMergeButton::~CSplitMergeButton()
{

}

bool CSplitMergeButton::inSplitMode() const
{
    return bInSplitMode_;
}

void CSplitMergeButton::setInSplitMode(bool bInSplitMode)
{
    bInSplitMode_ = bInSplitMode;
    updateButton();
}

void CSplitMergeButton::updateButton()
{
    if(bInSplitMode_)
    {
//        QPalette pal = palette();
//        pal.setColor(QPalette::Button, QColor(Qt::red));
//        setAutoFillBackground(true);
//        setPalette(pal);

        QPixmap pix(":/view/images/merge.png");
        QIcon icon(pix);
        setIcon(icon);
        setIconSize(size());
        update();
    }
    else
    {
//        QPalette pal = palette();
//        pal.setColor(QPalette::Button, QColor(Qt::green));
//        setAutoFillBackground(true);
//        setPalette(pal);

        QPixmap pix(":/view/images/split.png");
        QIcon icon(pix);
        setIcon(icon);
        setIconSize(size());
        update();
    }
}

bool CSplitMergeButton::autoToggleOnClick() const
{
    return bAutoToggle_;
}

void CSplitMergeButton::setAutoToggleOnClick(bool bAutoRevertOnClick)
{
    bAutoToggle_ = bAutoRevertOnClick;
}

void CSplitMergeButton::onClicked()
{
    if(bAutoToggle_)
    {
        bInSplitMode_ = !bInSplitMode_;
        updateButton();
    }
}
