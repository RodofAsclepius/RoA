#include "transfergraphdialog.h"
#include "ui_transfergraphdialog.h"

CTransferGraphDialog::CTransferGraphDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTransferGraphDialog)
{
    ui->setupUi(this);
}

CTransferGraphDialog::~CTransferGraphDialog()
{
    delete ui;
}
