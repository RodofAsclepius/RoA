#include "minegraphdialog.h"
#include "ui_minegraphdialog.h"

CMineGraphDialog::CMineGraphDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMineGraphDialog)
{
    ui->setupUi(this);

    initValues();
}

CMineGraphDialog::~CMineGraphDialog()
{
    delete ui;
}

void CMineGraphDialog::on_buttonBox_accepted()
{
    retrieveValues();
}

CCausalModel CMineGraphDialog::causalModelSelected() const
{
    return causalModelSelected_;
}

void CMineGraphDialog::initValues()
{
    ui->comboBoxGroups->clear();
    foreach(CCausalModel causalModel, lstCausalModels_)
    {
        ui->comboBoxGroups->addItem(causalModel.group().name(), QVariant::fromValue(causalModel));
    }

    ui->doubleSpinBoxAlpha->setValue(rAlpha_);
}

void CMineGraphDialog::retrieveValues()
{
    causalModelSelected_ = ui->comboBoxGroups->currentData().value<CCausalModel>();
    rAlpha_ = ui->doubleSpinBoxAlpha->value();
}

qreal CMineGraphDialog::alpha() const
{
    return rAlpha_;
}

void CMineGraphDialog::setAlpha(const qreal &rAlpha)
{
    rAlpha_ = rAlpha;
    initValues();
}

CCausalModelList CMineGraphDialog::causalModels() const
{
    return lstCausalModels_;
}

void CMineGraphDialog::setCausalModels(const CCausalModelList &lstCausalModels)
{
    lstCausalModels_ = lstCausalModels;
    initValues();
}
