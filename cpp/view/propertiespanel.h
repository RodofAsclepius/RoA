#ifndef PROPERTIESPANEL_H
#define PROPERTIESPANEL_H

#include <QWidget>

class QGridLayout;

class CPropertiesPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CPropertiesPanel(QWidget *parent = nullptr);
    virtual ~CPropertiesPanel();

    void showWindow(QWidget* pWidget);
protected:
    QGridLayout* _pGridLayoutMain;

//    update();
signals:

public slots:
};

#endif // PROPERTIESPANEL_H
