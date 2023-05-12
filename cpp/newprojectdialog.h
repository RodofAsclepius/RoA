#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include "databaseconnection.h"
#include <QDialog>

namespace Ui {
class CNewProjectDialog;
}

class CNewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CNewProjectDialog(QWidget *parent = nullptr);
    ~CNewProjectDialog();

    CDatabaseConnection databaseConnection() const;

private:
    Ui::CNewProjectDialog *ui;
};

#endif // NEWPROJECTDIALOG_H
