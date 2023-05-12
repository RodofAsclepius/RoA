#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>

class QHBoxLayout;
class QSpacerItem;

class CToolBar : public QWidget
{
    Q_OBJECT
public:
    CToolBar(QWidget *parent = 0);
    ~CToolBar();

    void addWidget(QWidget *pWidget);
    void addLayout(QLayout *pLayout);
    void addItem(QLayout *pItem);
    void addSpacer(QSpacerItem *pSpacerItem = nullptr);

    QColor colorBackground() const;
    void setColorBackground(const QColor &colorBackground);

protected:
    QHBoxLayout *pLayout_;
//    QColor colorBackground_ = QColor("#ababab");
//    QColor colorBackground_ = QColor("lightgray");
//    QColor colorBackground_ = QColor("lightblue");
//    QColor colorBackground_ = QColor("#96ded1"); // OK
//    QColor colorBackground_ = QColor("#afeeee"); // OK
    QColor colorBackground_ = QColor("#e0ffff"); // OK
//    QColor colorBackground_ = QColor("darkgray");

//    QColor colorBackground_ = QColor("#6bd8eb");
    void paintEvent(QPaintEvent *event) override;
};

#endif // TOOLBAR_H
