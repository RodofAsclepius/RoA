#include "loadprojectdialog.h"
#include "ui_loadprojectdialog.h"
#include "utils.h"
#include "maincontroller.h"

#include <QSettings>
#include <QDir>


CLoadProjectDialog::CLoadProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLoadProjectDialog),
    _mainController(CMainController::instance())
{
//    DEBUG_OBJ_INFO

    ui->setupUi(this);
    setModal(true);

    setWindowTitle("Load previous project");

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TU/e", "Asclepius");
    settings.beginGroup("Environment");
        settings.beginGroup("System");
            _strProjectFolder = settings.value("ProjectsDirectory", "error").toString();
        settings.endGroup();
    settings.endGroup();

    connect(ui->listWidgetProjects, &QListWidget::doubleClicked, this, &CLoadProjectDialog::onListWidgedDoubleClicked);

    refresh();
}


CLoadProjectDialog::~CLoadProjectDialog()
{
    delete ui;
}


QString CLoadProjectDialog::getProjectFile() const
{
    return _strProjectFile;
}

void CLoadProjectDialog::refresh()
{
    QDir directory;
    directory.setCurrent(_strProjectFolder);
    QStringList filters;
    filters << "*.cvis";
    QFileInfoList fileInfoList = directory.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);

    ui->listWidgetProjects->clear();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        ui->listWidgetProjects->addItem(fileInfo.baseName());
    }

    if(ui->listWidgetProjects->count() > 0)
    {
        ui->listWidgetProjects->setCurrentRow(0);
    }
}

void CLoadProjectDialog::on_buttonBox_accepted()
{
    QList<QListWidgetItem *> lstSelectedItems = ui->listWidgetProjects->selectedItems();

    if(!lstSelectedItems.isEmpty())
    {
        _strProjectFile = _strProjectFolder + "/" + lstSelectedItems.first()->text() + ".cvis";
        accept();
    }
}


void CLoadProjectDialog::onListWidgedDoubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    on_buttonBox_accepted();
}



void CLoadProjectDialog::on_buttonBox_rejected()
{
    close();
}

void CLoadProjectDialog::on_listWidgetProjects_itemSelectionChanged()
{
    QList<QListWidgetItem*> lstItems = ui->listWidgetProjects->selectedItems();

    ui->pushButtonDelete->setEnabled(!lstItems.isEmpty());
}

void CLoadProjectDialog::on_pushButtonDelete_clicked()
{
    QList<QListWidgetItem*> lstSelectedItems = ui->listWidgetProjects->selectedItems();
    QString strProjectFile = _strProjectFolder + "/" + lstSelectedItems.first()->text() + ".cvis";
    _mainController.deleteProject(strProjectFile);

    refresh();
}
