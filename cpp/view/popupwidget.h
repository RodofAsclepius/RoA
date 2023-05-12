#ifndef POPUPWIDGET_H
#define POPUPWIDGET_H

#include <QWidget>

class QGridLayout;



class CPopupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPopupWidget(QWidget* parent, QPoint pos, const QRect& rect);
    ~CPopupWidget();

    void setChildWidget(QWidget* pChildWidget);
    QWidget* getChildWidget() const;
protected:
    QWidget* _pChildWidget;
    QGridLayout* _pGridLayout;
    QRect _rectActivated;

    void onMouseLeftRect();

    void mouseMoveEvent(QMouseEvent *event) override;
signals:

public slots:
};


#endif // POPUPWIDGET_H
