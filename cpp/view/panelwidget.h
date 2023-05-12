#ifndef CPANELWIDGET_H
#define CPANELWIDGET_H

#include <QWidget>
#include "toolbar.h"
#include "panellabel.h"

class QGridLayout;
class CPanelLabel;

class CPanelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPanelWidget(QWidget *parent = nullptr);
    ~CPanelWidget();

    QColor colorBorder() const;
    void setColorBorder(const QColor &value);

    QGridLayout *gridLayout() const;
    void setGridLayout(QGridLayout *gridLayout);

    CToolBar *toolBar() const;
    void setToolBar(CToolBar *toolBar);

    CPanelLabel *panelLabel() const;
    void setPanelLabel(CPanelLabel *panelLabel);

    void updateStyleSheet();

protected:
    QMargins margins_;
    CToolBar *pToolBar_;
    QGridLayout *pGridLayout_;
    CPanelLabel *pPanelLabel_;
    QColor colorBorder_;

    void paintEvent(QPaintEvent *event) override;
};

#endif // CPANELWIDGET_H
