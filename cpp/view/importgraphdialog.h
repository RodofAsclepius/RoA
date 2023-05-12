#ifndef IMPORTGRAPHDIALOG_H
#define IMPORTGRAPHDIALOG_H

#include <QDialog>
#include "groupslist.h"

namespace Ui {
class CImportGraphDialog;
}

class CImportGraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CImportGraphDialog(QWidget *parent = nullptr);
    ~CImportGraphDialog();

    CGroup group() const;
    void setGroup(const CGroup &group);

    bool ImportUserGraph() const;
    void setImportUserGraph(bool ImportUserGraph);

    bool clearCurrentGraph() const;
    void setClearCurrentGraph(bool clearCurrentGraph);

    CGroupsList groups() const;
    void setGroups(const CGroupsList &groups);

    QString filename() const;
    void setFilename(const QString &filename);

    void updateGroupsCombo();

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

private:
    Ui::CImportGraphDialog *ui;
    QString strFilename_;
    CGroupsList lstGroups_;
    CGroup group_;
    bool bImportUserGraph_ = true;
    bool bClearCurrentGraph_ = false;
};

#endif // IMPORTGRAPHDIALOG_H
