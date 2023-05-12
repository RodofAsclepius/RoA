#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"

CNewProjectDialog::CNewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CNewProjectDialog)
{
    ui->setupUi(this);
}

CNewProjectDialog::~CNewProjectDialog()
{
    delete ui;
}

CDatabaseConnection CNewProjectDialog::databaseConnection() const
{
    CDatabaseConnection dbConnection;

    dbConnection.setHost(ui->lineEditHost->text());
    dbConnection.setDatabaseName(ui->lineEditDatabaseName->text());
    dbConnection.setDatabaseUserName(ui->lineEditUser->text());
    dbConnection.setDatabasePassword(ui->lineEditPassword->text());
    dbConnection.setDatabaseMainTableName(ui->lineEditMainTableName->text());
    dbConnection.setDatabaseVariablesTypesTableName(ui->lineEditVariablesTypesTableName->text());

    return dbConnection;
}
