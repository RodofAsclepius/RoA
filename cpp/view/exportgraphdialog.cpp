#include "exportgraphdialog.h"
#include "ui_exportgraphdialog.h"
#include <QFileDialog>

CExportGraphDialog::CExportGraphDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CExportGraphDialog)
{
    ui->setupUi(this);

    setWindowTitle("Export graph dialog");
    updatePanel();
}

CExportGraphDialog::~CExportGraphDialog()
{
    delete ui;
}

void CExportGraphDialog::on_buttonBox_accepted()
{
    strFilename_ = ui->lineEditFilename->text();
    causalModelSelected_ = ui->comboBoxGroups->currentData().value<CCausalModel>();
    bExportUserGraph_ = ui->radioButtonUser->isChecked();
    bOpenExportedFile_ = ui->checkBoxOpenFile->isChecked();
}

void CExportGraphDialog::on_pushButton_clicked()
{
    QString strFileName = QFileDialog::getSaveFileName(this, tr("Export causal model"), "../data/", tr("CSV Files (*.csv)"));
    ui->lineEditFilename->setText(strFileName);
}

CCausalModel CExportGraphDialog::causalModelSelected() const
{
    return causalModelSelected_;
}

void CExportGraphDialog::setCausalModelSelected(const CCausalModel &causalModelSelected)
{
    causalModelSelected_ = causalModelSelected;
}

bool CExportGraphDialog::openExportedFile() const
{
    return bOpenExportedFile_;
}

void CExportGraphDialog::setOpenExportedFile(bool bOpenExportedFile)
{
    bOpenExportedFile_ = bOpenExportedFile;
}

CCausalModelList CExportGraphDialog::causalModels() const
{
    return lstCausalModels_;
}

void CExportGraphDialog::setCausalModels(const CCausalModelList &lstCausalModels)
{
    lstCausalModels_ = lstCausalModels;
    updatePanel();
}

void CExportGraphDialog::updatePanel()
{
    /* Groups combo */
    ui->comboBoxGroups->clear();
    foreach(CCausalModel causalModel, lstCausalModels_)
    {
        ui->comboBoxGroups->addItem(causalModel.group().name(), QVariant::fromValue(causalModel));
    }
}

bool CExportGraphDialog::exportUserGraph() const
{
    return bExportUserGraph_;
}

void CExportGraphDialog::setExportUserGraph(bool bExportUserGraph)
{
    bExportUserGraph_ = bExportUserGraph;
}

QString CExportGraphDialog::filename() const
{
    return strFilename_;
}

void CExportGraphDialog::setFilename(const QString &strFilename)
{
    strFilename_ = strFilename;
}
