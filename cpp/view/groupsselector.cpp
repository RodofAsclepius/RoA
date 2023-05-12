#include "groupsselector.h"
#include "ui_groupsselector.h"

CGroupsSelector::CGroupsSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CGroupsSelector)
{
    ui->setupUi(this);
}

CGroupsSelector::~CGroupsSelector()
{
    delete ui;
}
