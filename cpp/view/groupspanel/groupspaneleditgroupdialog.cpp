#include "groupspaneleditgroupdialog.h"
#include "ui_groupspaneleditgroupdialog.h"

#include <QColorDialog>
#include "utils.h"

using namespace utils;

CGroupsPanelEditGroupDialog::CGroupsPanelEditGroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CGroupsPanelEditGroupDialog)
{
    ui->setupUi(this);

    QWidget::setWindowTitle("Group properties");

    connect(this, &CGroupsPanelEditGroupDialog::accepted, this, &CGroupsPanelEditGroupDialog::onAccepted);
    connect(this, &CGroupsPanelEditGroupDialog::rejected, this, &CGroupsPanelEditGroupDialog::onRejected);
}

CGroupsPanelEditGroupDialog::~CGroupsPanelEditGroupDialog()
{
    delete ui;
}

CGroup CGroupsPanelEditGroupDialog::group() const
{
    return group_;
}

void CGroupsPanelEditGroupDialog::setGroup(const CGroup &group)
{
    group_ = group;
    color_ = group_.colorPrimary().name();
    color2_ = group_.colorSecondary().name();

    updateDialog();
}

bool CGroupsPanelEditGroupDialog::removeGroup() const
{
    return bRemoveGroup_;
}

void CGroupsPanelEditGroupDialog::updateDialog()
{
    ui->lineEditName->setText(group_.name());
    ui->lineEditQuery->setText(group_.filterQuery());

    ui->lineEditColor1->setStyleSheet("background-color:" + color_.name());
    ui->lineEditColor1->setText(color_.name());
    ui->lineEditColor2->setStyleSheet("background-color:" + color2_.name());
    ui->lineEditColor2->setText(color2_.name());
}

void CGroupsPanelEditGroupDialog::onAccepted()
{
    group_.setName(ui->lineEditName->text());
    group_.setFilterQuery(ui->lineEditQuery->text());
    group_.setColorPrimary(color_);
    group_.setColorSecondary(color2_);
}

void CGroupsPanelEditGroupDialog::onRejected()
{

}

void CGroupsPanelEditGroupDialog::on_pushButtonColor_clicked()
{
    QColorDialog dlg;
    dlg.setCurrentColor(color_);

    if(dlg.exec() == QColorDialog::Accepted)
    {
        color_ = dlg.currentColor();
        color2_ = color_.darker();

        ui->lineEditColor1->setStyleSheet("background-color:" + color_.name());
        ui->lineEditColor1->setText(color_.name());
        ui->lineEditColor2->setStyleSheet("background-color:" + color2_.name());
        ui->lineEditColor2->setText(color2_.name());
    }
}

void CGroupsPanelEditGroupDialog::on_pushButtonRemove_clicked()
{
    bRemoveGroup_ = true;
    done(QDialog::Rejected);
}
