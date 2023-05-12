#include "refreshbutton.h"
#include "maincontroller.h"
#include "colorandfontmap.h"

CRefreshButton::CRefreshButton(QWidget *pParent)
    : QPushButton(pParent),
      mainController_(CMainController::instance()),
      mapColorsAndFonts_(CColorAndFontMap::instance())
{
    connect(this, &QPushButton::clicked, this, &CRefreshButton::onClicked);

    setFixedSize(32, 32);
    updateButton();

    setStyleSheet("QPushButton {background-color: #e0ffff; margin: 1px 1px 1px 1px; border: solid gray 1px;}");
}

CRefreshButton::~CRefreshButton()
{

}

bool CRefreshButton::needRefreshState() const
{
    return bNeedRefreshState_;
}

void CRefreshButton::setNeedRefreshState(bool bNeedRefreshState)
{
    bNeedRefreshState_ = bNeedRefreshState;
    updateButton();
}

void CRefreshButton::updateButton()
{
    if(bNeedRefreshState_)
    {
        QPixmap pix(":/view/images/needrefresh.png");
        QIcon icon(pix);
        setIcon(icon);
        setIconSize(QSize(16, 16));
        update();
    }
    else
    {
        QPixmap pix(":/view/images/uptodate.png");
        QIcon icon(pix);
        setIcon(icon);
        setIconSize(QSize(16, 16));
        update();
    }
}

bool CRefreshButton::autoRevertOnClick() const
{
    return bAutoRevertOnClick_;
}

void CRefreshButton::setAutoRevertOnClick(bool bAutoRevertOnClick)
{
    bAutoRevertOnClick_ = bAutoRevertOnClick;
}

void CRefreshButton::onClicked()
{
    if(bAutoRevertOnClick_)
    {
        bNeedRefreshState_ = false;
        updateButton();
    }
}
