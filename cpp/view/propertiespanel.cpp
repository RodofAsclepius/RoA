#include "propertiespanel.h"

#include "panellabel.h"
#include <QGridLayout>

CPropertiesPanel::CPropertiesPanel(QWidget *parent) : QWidget(parent)
{
    setContentsMargins(0, 0 ,0 ,0);
    /* Main layout */
    _pGridLayoutMain = new QGridLayout();
    _pGridLayoutMain->setContentsMargins(0, 0 ,0 ,0);
    setLayout(_pGridLayoutMain);

    /* Label */
    CPanelLabel* pLabelGroups = new CPanelLabel("Properties");
    _pGridLayoutMain->addWidget(pLabelGroups, 0, 0, 1, 1);

    _pGridLayoutMain->setRowStretch(0, 1);
    _pGridLayoutMain->setRowStretch(1, 10);
}

CPropertiesPanel::~CPropertiesPanel()
{
    //    delete _pWidget;
}

void CPropertiesPanel::showWindow(QWidget *pWidget)
{
    _pGridLayoutMain->addWidget(pWidget, 1, 0, 1, 1);
}
