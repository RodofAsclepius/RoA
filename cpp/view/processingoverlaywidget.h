#ifndef PROCESSINGOVERLAYWIDGET_H
#define PROCESSINGOVERLAYWIDGET_H


#include <QDialog>
#include <QSize>


class QMovie;

class CProcessingOverlayWidget : public QDialog
{
    Q_OBJECT
public:
    explicit CProcessingOverlayWidget(QWidget *parent);

    void paintEvent(QPaintEvent* event);

protected:
    QWidget* _pParentWidget;
    int _iNumberOfShows;
    int _iTimerIDRepaint;

    QMovie* _pSpinnerMovie;

    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
};

    
#endif // PROCESSINGOVERLAYWIDGET_H
