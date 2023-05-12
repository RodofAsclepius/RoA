#ifndef CLOADPROJECTDIALOG_H
#define CLOADPROJECTDIALOG_H

#include <QDialog>


namespace Ui {
class CLoadProjectDialog;
}

class CMainController;

class CLoadProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CLoadProjectDialog(QWidget *parent = 0);
    ~CLoadProjectDialog();

    QString getProjectFile() const;
    void refresh();

private slots:
    void on_buttonBox_accepted();
    void onListWidgedDoubleClicked(const QModelIndex &index);

    void on_buttonBox_rejected();

    void on_listWidgetProjects_itemSelectionChanged();

    void on_pushButtonDelete_clicked();

private:
    Ui::CLoadProjectDialog *ui;
    CMainController& _mainController;
    QString _strProjectFolder;
    QString _strProjectFile;
};


#endif // CLOADPROJECTDIALOG_H
