#include "sortlayoutdialog.h"
#include "ui_sortlayoutdialog.h"

CSortLayoutDialog::CSortLayoutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSortLayoutDialog)
{
    ui->setupUi(this);

    setDialogValues();
    ui->tabWidget->setCurrentIndex(0);
}

CSortLayoutDialog::~CSortLayoutDialog()
{
    delete ui;
}

QString CSortLayoutDialog::layoutMethod() const
{
    QString strLayoutMethod = ui->tabWidget->tabText(ui->tabWidget->currentIndex());

    return strLayoutMethod;
}

QVariantMap CSortLayoutDialog::parameters() const
{
    QVariantMap mapParameters;

    if(strLayoutMethod_ == "Orthogonally")
    {
        mapParameters.insert("runs", QVariant::fromValue(iRuns_ortho));
        mapParameters.insert("seperation", QVariant::fromValue(rSeperation_ortho));
        mapParameters.insert("coverhang", QVariant::fromValue(coverhang_ortho));
        mapParameters.insert("nodecenter2bends", QVariant::fromValue(iNodecenter2bendsCount_ortho));
    }
    if(strLayoutMethod_ == "Sugiyama")
    {
        mapParameters.insert("layerdistance", QVariant::fromValue(rLayerdistance_Sugiyama));
        mapParameters.insert("nodedistance", QVariant::fromValue(rNodedistance_Sugiyama));
        mapParameters.insert("weightbalancing", QVariant::fromValue(rWeightBalancing_Sugiyama));
    }
    if(strLayoutMethod_ == "Energy")
    {
        mapParameters.insert("unitedgelength", QVariant::fromValue(rUnitedgelength_energy));
        mapParameters.insert("usehighleveloptions", QVariant::fromValue(bUsehighleveloptions_energy));
        mapParameters.insert("newinitialplacement", QVariant::fromValue(bInitialPlacement_energy));
    }

    return mapParameters;
}

void CSortLayoutDialog::setDialogValues()
{
    /* Ortho */
    ui->spinBoxRuns->setValue(iRuns_ortho);
    ui->spinBoxRuns->setMaximum(9999);

    ui->doubleSpinBoxSeperation->setValue(rSeperation_ortho);
    ui->doubleSpinBoxSeperation->setMaximum(9999);

    ui->doubleSpinBoxOverhang->setValue(coverhang_ortho);
    ui->doubleSpinBoxOverhang->setMaximum(9999);

    ui->spinBoxNodeCenterBends->setValue(iNodecenter2bendsCount_ortho);

    /* Sugiyama */
    ui->doubleSpinBoxLayerDistance->setValue(rLayerdistance_Sugiyama);
    ui->doubleSpinBoxLayerDistance->setMaximum(9999);

    ui->doubleSpinBoxNodeDistance->setValue(rNodedistance_Sugiyama);
    ui->doubleSpinBoxNodeDistance->setMaximum(9999);

    ui->doubleSpinBoxWeightBalancing->setValue(rWeightBalancing_Sugiyama);
    ui->doubleSpinBoxWeightBalancing->setMaximum(9999);

    /* Energy */
    ui->spinBoxUnitEdgeLength->setValue(rUnitedgelength_energy);
    ui->spinBoxUnitEdgeLength->setMaximum(9999);

    ui->checkBoxNewIntitialPlacement->setChecked(bInitialPlacement_energy);
    ui->checkBoxUseHighlevelOptions->setChecked(bUsehighleveloptions_energy);
}

void CSortLayoutDialog::retrieveDialogValues()
{
    strLayoutMethod_ = ui->tabWidget->tabText(ui->tabWidget->currentIndex());

    /* Ortho */
    iRuns_ortho = ui->spinBoxRuns->value();
    rSeperation_ortho = ui->doubleSpinBoxSeperation->value();
    coverhang_ortho = ui->doubleSpinBoxOverhang->value();
    iNodecenter2bendsCount_ortho = ui->spinBoxNodeCenterBends->value();

    /* Sugiyama */
    rLayerdistance_Sugiyama = ui->doubleSpinBoxLayerDistance->value();
    rNodedistance_Sugiyama = ui->doubleSpinBoxNodeDistance->value();
    rWeightBalancing_Sugiyama = ui->doubleSpinBoxWeightBalancing->value();

    /* Energy */
    rUnitedgelength_energy = ui->spinBoxUnitEdgeLength->value();
    bInitialPlacement_energy = ui->checkBoxNewIntitialPlacement->isChecked();
    bUsehighleveloptions_energy = ui->checkBoxUseHighlevelOptions->isChecked();
}
void CSortLayoutDialog::on_buttonBox_accepted()
{
    retrieveDialogValues();
}
