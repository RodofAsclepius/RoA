#ifndef REFRESHBUTTON_H
#define REFRESHBUTTON_H

#include <QPushButton>

class CMainController;
class CColorAndFontMap;

class CRefreshButton : public QPushButton
{
    Q_OBJECT
public:
    CRefreshButton(QWidget* pParent = nullptr);
    ~CRefreshButton();

    bool needRefreshState() const;
    void setNeedRefreshState(bool needRefreshState = true);

    void updateButton();
    bool autoRevertOnClick() const;
    void setAutoRevertOnClick(bool autoRevertOnClick);

protected:
    CMainController &mainController_;
    CColorAndFontMap &mapColorsAndFonts_;
    bool bNeedRefreshState_ = false;
    bool bAutoRevertOnClick_ = true;
protected slots:
    void onClicked();
};

#endif // REFRESHBUTTON_H
