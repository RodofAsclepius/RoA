#ifndef CGROUPSPANELEDITGROUPDIALOG_H
#define CGROUPSPANELEDITGROUPDIALOG_H

#include <QDialog>
#include "group.h"

namespace Ui {
class CGroupsPanelEditGroupDialog;
}

class QAbstractButton;

class CGroupsPanelEditGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CGroupsPanelEditGroupDialog(QWidget *parent = nullptr);
    ~CGroupsPanelEditGroupDialog();

    CGroup group() const;
    void setGroup(const CGroup &group);

    bool removeGroup() const;

private:
    Ui::CGroupsPanelEditGroupDialog *ui;
    CGroup group_;
    QColor color_;
    QColor color2_;
    bool bRemoveGroup_ = false;

    void updateDialog();

protected slots:
    void onAccepted();
    void onRejected();
private slots:
    void on_pushButtonColor_clicked();
    void on_pushButtonRemove_clicked();
};

#endif // CGROUPSPANELEDITGROUPDIALOG_H
