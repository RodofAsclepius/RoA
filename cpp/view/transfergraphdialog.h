#ifndef TRANSFERGRAPHDIALOG_H
#define TRANSFERGRAPHDIALOG_H

#include <QDialog>

namespace Ui {
class CTransferGraphDialog;
}

class CTransferGraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CTransferGraphDialog(QWidget *parent = nullptr);
    ~CTransferGraphDialog();

private:
    Ui::CTransferGraphDialog *ui;
};

#endif // TRANSFERGRAPHDIALOG_H
