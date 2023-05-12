#ifndef MINEGRAPHDIALOG_H
#define MINEGRAPHDIALOG_H

#include <QDialog>
#include <causalmodellist.h>

namespace Ui {
class CMineGraphDialog;
}

class CMineGraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMineGraphDialog(QWidget *parent = nullptr);
    ~CMineGraphDialog();

    CCausalModelList causalModels() const;
    void setCausalModels(const CCausalModelList &lstCausalModels);

    qreal alpha() const;
    void setAlpha(const qreal &rAlpha);

    CCausalModel causalModelSelected() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CMineGraphDialog *ui;
    CCausalModelList lstCausalModels_;
    qreal rAlpha_ = 0.001;
    CCausalModel causalModelSelected_;

    void initValues();
    void retrieveValues();
};

#endif // MINEGRAPHDIALOG_H
