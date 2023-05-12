#include "graphviewoptionsdialog.h"
#include "ui_graphviewoptionsdialog.h"

GraphViewOptionsDialog::GraphViewOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphViewOptionsDialog)
{
    ui->setupUi(this);
}

GraphViewOptionsDialog::~GraphViewOptionsDialog()
{
    delete ui;
}

int GraphViewOptionsDialog::markovBlanketDistance() const
{
    return iMarkovBlanketDistance_;
}

void GraphViewOptionsDialog::setMarkovBlanketDistance(int newIMarkovBlanketDistance)
{
    iMarkovBlanketDistance_ = newIMarkovBlanketDistance;
    updateSettings();
}

bool GraphViewOptionsDialog::showMarkovBlanketPlus() const
{
    return bShowMarkovBlanketPlus_;
}

void GraphViewOptionsDialog::setShowMarkovBlanketPlus(bool newBShowMarkovBlanketPlus)
{
    bShowMarkovBlanketPlus_ = newBShowMarkovBlanketPlus;
    updateSettings();
}

bool GraphViewOptionsDialog::showUnconnected() const
{
    return bShowUnconnected_;
}

void GraphViewOptionsDialog::setShowUnconnected(bool newBShowUnconnected)
{
    bShowUnconnected_ = newBShowUnconnected;
    updateSettings();
}

void GraphViewOptionsDialog::updateSettings()
{
    ui->radioButtonShowFullGraph->setChecked(!bShowMarkovBlanketPlus_);
    ui->radioButtonShowMB->setChecked(bShowMarkovBlanketPlus_);
    ui->spinBoxMBDistance->setValue(iMarkovBlanketDistance_);
    ui->spinBoxMBDistance->setRange(0, 100);
    ui->checkBoxShowUnconnectedNodes->setChecked(bShowUnconnected_);
}

void GraphViewOptionsDialog::on_buttonBox_accepted()
{
    iMarkovBlanketDistance_ = ui->spinBoxMBDistance->value();
    bShowMarkovBlanketPlus_ = !ui->radioButtonShowFullGraph->isChecked();
    bShowUnconnected_ = ui->checkBoxShowUnconnectedNodes->isChecked();
}

