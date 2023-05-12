#ifndef MENUWIDGET_H
#define MENUWIDGET_H


#include <QWidget>
#include <QMap>


namespace Ui {
class CMenuWidget;
}

class QMenu;
class QPushButton;



class CMenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CMenuWidget(QWidget *parent = 0);
    ~CMenuWidget();

    void setMenuActions(const QList<QAction *>& actions);
    void setToolBarActions(const QList<QAction *>& actions);
    void addTab(const QString& strTabName);
private:
    Ui::CMenuWidget *ui;
protected:
    QMenu* _pMenu;
    QMap<QString, QPushButton*> _mapTabs;
    QString strActiveTabName;

    void setTabInactiveStyle(QPushButton* pPushButton);
    void setTabActiveStyle(QPushButton* pPushButton);
protected slots:
    void onPushButtonClicked();
signals:
    void tabClicked(const QString& strTabName);
};


#endif // MENUWIDGET_H
