#include "populationcontrolwidget.h"
#include "maincontroller.h"
#include "utils.h"

#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QTableWidget>

CPopulationControlWidget::CPopulationControlWidget(QWidget *parent)
    : QWidget(parent),
      _mainController(CMainController::getInstance()),
      _pTableSubPopulations(new QTableWidget())
{
    QGridLayout* pGridLayout = new QGridLayout();
    pGridLayout->setContentsMargins(6, 0, 0, 0);

//    QLabel* _pLabelEvent = new QLabel("Event");
//    pGridLayout->addWidget(_pLabelEvent);
//    pGridLayout->addWidget(_pComboBoxMainEvent);

//    QLabel* _pLabelPopulationSplit = new QLabel("Population split");
//    pGridLayout->addWidget(_pLabelPopulationSplit);
//    pGridLayout->addWidget(_pComboBoxPopulationSplit);

    _pTableSubPopulations->setColumnCount(2);
    _pTableSubPopulations->setHorizontalHeaderLabels(QString("Population;Color").split(";"));
    pGridLayout->addWidget(_pTableSubPopulations);

    setLayout(pGridLayout);

    updateTable();
}

void CPopulationControlWidget::updateTable()
{
//    DEBUG_OBJ_INFO
    _pTableSubPopulations->clear();
    _pTableSubPopulations->setRowCount(0);

    CPopulationsList lstPopulations = _applicationState.getPopulations();

    foreach(CPopulation population, lstPopulations)
    {
        _pTableSubPopulations->setRowCount(_pTableSubPopulations->rowCount() + 1);
        QTableWidgetItem* pNewTableWidgetItem = new QTableWidgetItem(population.getDataframeLabel());
        _pTableSubPopulations->setItem(_pTableSubPopulations->rowCount()-1, 0, pNewTableWidgetItem);
    }
}

void CPopulationControlWidget::onMainControllerActiveApplicationStateUpdated(int iActiveApplicationStateIndex, const CApplicationState &applicationState)
{
    _applicationState = applicationState;
    updateTable();
}
