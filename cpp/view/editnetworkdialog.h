#ifndef EDITNETWORKDIALOG_H
#define EDITNETWORKDIALOG_H

#include <QDialog>
#include "causalmodellist.h"

namespace Ui {
class CEditNetworkDialog;
}

class CMainController;
class QSortFilterProxyModel;
class QListWidgetItem;

class CEditNetworkDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CEditNetworkDialog(QWidget *parent = nullptr);
    ~CEditNetworkDialog();

    CCausalModelList causalModels() const;
    void setCausalModels(const CCausalModelList &causalModels);

    void updateWidget();
    void updateCausalModels();

    void setActiveModel(const CCausalModel &causalModel);

protected:
    CMainController &mainController_;
    CCausalModelList lstCausalModels_;
    CVariablesList lstVariables_;
    QSortFilterProxyModel *proxy1_, *proxy2_;
    CCausalModel causalModelActive_;

private:
    Ui::CEditNetworkDialog *ui;

private slots:
    void on_pushButtonAddRelation_clicked();
    void on_pushButtonRemoveRelation_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void onComboGroupIndexChanged(int iIndex);
    void on_listWidget_itemClicked(QListWidgetItem *item);
};

#endif // EDITNETWORKDIALOG_H
