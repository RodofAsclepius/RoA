#ifndef SORTLAYOUTDIALOG_H
#define SORTLAYOUTDIALOG_H

#include <QDialog>

namespace Ui {
class CSortLayoutDialog;
}

class CSortLayoutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSortLayoutDialog(QWidget *parent = nullptr);
    ~CSortLayoutDialog();

    QString layoutMethod() const;
    QVariantMap parameters() const;

    void setDialogValues();
    void retrieveDialogValues();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CSortLayoutDialog *ui;

    QString strLayoutMethod_;
    int iRuns_ortho = 100;
    qreal rSeperation_ortho = 60;
    qreal coverhang_ortho = 0.4;
    int iNodecenter2bendsCount_ortho = 3;

    qreal rLayerdistance_Sugiyama = 60;
    qreal rNodedistance_Sugiyama = 50;
    qreal rWeightBalancing_Sugiyama = 0.8;

    qreal rUnitedgelength_energy = 75;
    bool bInitialPlacement_energy = true;
    bool bUsehighleveloptions_energy = true;
};

#endif // SORTLAYOUTDIALOG_H
