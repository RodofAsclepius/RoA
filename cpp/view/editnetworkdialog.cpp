#include "editnetworkdialog.h"
#include "ui_editnetworkdialog.h"
#include "maincontroller.h"

CEditNetworkDialog::CEditNetworkDialog(QWidget *parent) :
    QDialog(parent),
    mainController_(CMainController::instance()),
    ui(new Ui::CEditNetworkDialog)
{
    ui->setupUi(this);

    updateWidget();
}

CEditNetworkDialog::~CEditNetworkDialog()
{
    delete ui;
}

void CEditNetworkDialog::on_pushButtonAddRelation_clicked()
{
    CGraph graphUser = causalModelActive_.graphUser();

    QString strLabel1 = ui->comboBoxArrowOut->currentText();
    QString strLabel2 = ui->comboBoxArrowIn->currentText();

    CNode node1;
    CNode node2;
    if(graphUser.nodeByLabel(strLabel1, node1) && graphUser.nodeByLabel(strLabel2, node2) )
    {
        CEdge edge(node1, node2);
        graphUser.addEdge(edge);
    }

    causalModelActive_.setGraphUser(graphUser);
    updateWidget();
}

void CEditNetworkDialog::on_pushButtonRemoveRelation_clicked()
{
    CGraph graphUser = causalModelActive_.graphUser();

    QString strLabel1 = ui->comboBoxArrowOut->currentText();
    QString strLabel2 = ui->comboBoxArrowIn->currentText();

    CNode node1;
    CNode node2;
    if(graphUser.nodeByLabel(strLabel1, node1) && graphUser.nodeByLabel(strLabel2, node2) )
    {
        graphUser.removeEdge(node1, node2);
    }

    causalModelActive_.setGraphUser(graphUser);
    updateWidget();
}

CCausalModelList CEditNetworkDialog::causalModels() const
{
    return lstCausalModels_;
}

void CEditNetworkDialog::setCausalModels(const CCausalModelList &lstCausalModels)
{
    lstCausalModels_ = lstCausalModels;
    updateWidget();
}

void CEditNetworkDialog::updateWidget()
{
    lstVariables_ = mainController_.project().applicationData().variables();

    /* Fill groups combos */
    disconnect(ui->comboBoxGroups, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboGroupIndexChanged(int)));
    ui->comboBoxGroups->clear();
    foreach(CCausalModel causalModel, lstCausalModels_)
    {
        CGroup group = causalModel.group();
        ui->comboBoxGroups->addItem(group.name(), QVariant::fromValue(causalModel));
    }

    /* Active model? */
    if(!lstCausalModels_.isEmpty())
    {
        if(!lstCausalModels_.contains(causalModelActive_))
        {
            causalModelActive_ = lstCausalModels_.first();
        }
        else
        {
            int iIndex = lstCausalModels_.indexOf(causalModelActive_);
            ui->comboBoxGroups->setCurrentIndex(iIndex);
        }

        /* Fill in relations */
        ui->listWidget->clear();
        CGraph graph = causalModelActive_.graphUser();
        CEdgesList lstEdges = graph.edges();
        foreach(CEdge edge, lstEdges)
        {
            CNode nodeSource = edge.sourceNode();
            CNode nodeTarget = edge.targetNode();
            QListWidgetItem * pItem = new QListWidgetItem(nodeSource.label() + " -> " + nodeTarget.label());
            pItem->setData(Qt::UserRole, QVariant::fromValue(edge));
            ui->listWidget->addItem(pItem);
        }
    }

    connect(ui->comboBoxGroups, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboGroupIndexChanged(int)));

    /* Fill variables */
    ui->comboBoxArrowOut->clear();
    ui->comboBoxArrowIn->clear();
    foreach(CVariable variable, lstVariables_)
    {
        ui->comboBoxArrowOut->addItem(variable.name(), QVariant::fromValue(variable));
        ui->comboBoxArrowIn->addItem(variable.name(), QVariant::fromValue(variable));
    }
}

void CEditNetworkDialog::updateCausalModels()
{
    lstCausalModels_.replace(causalModelActive_);
}

void CEditNetworkDialog::setActiveModel(const CCausalModel &causalModel)
{
    causalModelActive_ = causalModel;
    updateWidget();
}

void CEditNetworkDialog::on_buttonBox_accepted()
{
    updateCausalModels();
    accept();
}

void CEditNetworkDialog::on_buttonBox_rejected()
{
    reject();
}

void CEditNetworkDialog::onComboGroupIndexChanged(int iIndex)
{
    lstCausalModels_.replace(causalModelActive_);

    QVariant varData = ui->comboBoxGroups->currentData();
    CCausalModel causalModel = varData.value<CCausalModel>();

    setActiveModel(causalModel);
}

void CEditNetworkDialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QListWidgetItem *pItem = ui->listWidget->currentItem();
    CEdge edge = pItem->data(Qt::UserRole).value<CEdge>();

    CNode nodeSource = edge.sourceNode();
    CNode nodeTarget = edge.targetNode();

    int iIndex1 = ui->comboBoxArrowOut->findText(nodeSource.label());
    if(iIndex1 != -1)
    {
        ui->comboBoxArrowOut->setCurrentIndex(iIndex1);
    }

    int iIndex2 = ui->comboBoxArrowIn->findText(nodeTarget.label());
    if(iIndex2 != -1)
    {
        ui->comboBoxArrowIn->setCurrentIndex(iIndex2);
    }
}
