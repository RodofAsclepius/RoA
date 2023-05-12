#ifndef EXPORTGRAPHDIALOG_H
#define EXPORTGRAPHDIALOG_H

#include <QDialog>
#include "causalmodellist.h"

namespace Ui {
class CExportGraphDialog;
}

class CExportGraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CExportGraphDialog(QWidget *parent = nullptr);
    ~CExportGraphDialog();

    QString filename() const;
    void setFilename(const QString &filename);

    bool exportUserGraph() const;
    void setExportUserGraph(bool exportUserGraph);

    CCausalModelList causalModels() const;
    void setCausalModels(const CCausalModelList &causalModels);

    void updatePanel();

    bool openExportedFile() const;
    void setOpenExportedFile(bool openExportedFile);

    CCausalModel causalModelSelected() const;
    void setCausalModelSelected(const CCausalModel &causalModelSelected);

private slots:
    void on_buttonBox_accepted();
    void on_pushButton_clicked();

private:
    Ui::CExportGraphDialog *ui;
    QString strFilename_;
    CCausalModelList lstCausalModels_;
    CCausalModel causalModelSelected_;
    bool bExportUserGraph_ = true;
    bool bOpenExportedFile_ = true;
};

#endif // EXPORTGRAPHDIALOG_H
