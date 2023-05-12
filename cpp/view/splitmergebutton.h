#ifndef SPLITMERGEBUTTON_H
#define SPLITMERGEBUTTON_H

#include <QPushButton>

class CMainController;
class CColorAndFontMap;

class CSplitMergeButton : public QPushButton
{
    Q_OBJECT
public:
    CSplitMergeButton(QWidget* pParent = nullptr);
    ~CSplitMergeButton();

    bool inSplitMode() const;
    void setInSplitMode(bool inSplitMode = true);

    void updateButton();
    bool autoToggleOnClick() const;
    void setAutoToggleOnClick(bool autoToggleOnClick);

protected:
    CMainController &mainController_;
    CColorAndFontMap &mapColorsAndFonts_;
    bool bInSplitMode_ = false;
    bool bAutoToggle_ = true;
protected slots:
    void onClicked();
};

#endif // SPLITMERGEBUTTON_H
