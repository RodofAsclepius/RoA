
#include "aboutdialog.h"
#include "ui_aboutdialog.h"


AboutDialog::AboutDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowTitle("About PopulationExplorer");
}


AboutDialog::~AboutDialog()
{
    delete ui;
}


void AboutDialog::on_pushButtonClose_clicked()
{
    close();
}

