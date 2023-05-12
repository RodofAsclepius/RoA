#include "importgraphdialog.h"
#include "ui_importgraphdialog.h"
#include "maincontroller.h"
#include <QFileDialog>

CImportGraphDialog::CImportGraphDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CImportGraphDialog)
{
    ui->setupUi(this);
    setWindowTitle("Import graph dialog");
    updateGroupsCombo();
}

CImportGraphDialog::~CImportGraphDialog()
{
    delete ui;
}

void CImportGraphDialog::on_buttonBox_accepted()
{
    strFilename_ = ui->lineEditFilename->text();
    group_ = ui->comboBoxGroups->currentData().value<CGroup>();
    bImportUserGraph_ = ui->radioButtonUser->isChecked();
    bClearCurrentGraph_ = ui->checkBoxClearCurrentGraph->isChecked();
}

CGroupsList CImportGraphDialog::groups() const
{
    return lstGroups_;
}

void CImportGraphDialog::setGroups(const CGroupsList &lstGroups)
{
    lstGroups_ = lstGroups;
    updateGroupsCombo();
}

bool CImportGraphDialog::clearCurrentGraph() const
{
    return bClearCurrentGraph_;
}

void CImportGraphDialog::setClearCurrentGraph(bool bClearCurrentGraph)
{
    bClearCurrentGraph_ = bClearCurrentGraph;
}

bool CImportGraphDialog::ImportUserGraph() const
{
    return bImportUserGraph_;
}

void CImportGraphDialog::setImportUserGraph(bool bImportUserGraph)
{
    bImportUserGraph_ = bImportUserGraph;
}

CGroup CImportGraphDialog::group() const
{
    return group_;
}

void CImportGraphDialog::setGroup(const CGroup &group)
{
    group_ = group;
}

void CImportGraphDialog::on_pushButton_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this, tr("Import causal model"), "../data/", tr("CSV Files (*.csv)"));
    ui->lineEditFilename->setText(strFileName);
}

QString CImportGraphDialog::filename() const
{
    return strFilename_;
}

void CImportGraphDialog::setFilename(const QString &strFilename)
{
    strFilename_ = strFilename;
}

void CImportGraphDialog::updateGroupsCombo()
{
    ui->comboBoxGroups->clear();

    foreach(CGroup group, lstGroups_)
    {
        ui->comboBoxGroups->addItem(group.name(), QVariant::fromValue(group));
    }
}
