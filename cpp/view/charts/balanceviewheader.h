#ifndef BALANCEVIEWHEADER_H
#define BALANCEVIEWHEADER_H

#include <QWidget>
#include "balancemodelslist.h"

class CColorAndFontMap;

class CBalanceViewHeader : public QWidget
{
    Q_OBJECT
public:
    CBalanceViewHeader(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~CBalanceViewHeader() override;

    QMargins marginsPlot() const;
    void setMarginsPlot(const QMargins &marginsPlot);

    virtual void paintEvent(QPaintEvent *event) override;

    QMap<QString, qreal> ticksLabels() const;
    void setTicksLabels(const QMap<QString, qreal> &ticksLabels);

protected:
    CColorAndFontMap& mapsColorAndFonts_;
    QMargins marPlot_;
    int iTickHeight_;
    int iAxisWidth_;
    QMap<QString, qreal> mapTicksLabels_;

    virtual void resizeEvent(QResizeEvent *event) override;

protected slots:
//    void onColorAndFontMapUpdated();
};

#endif // BALANCEVIEWHEADER_H
