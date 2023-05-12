#ifndef POPULATIONCONTROLWIDGET_H
#define POPULATIONCONTROLWIDGET_H

#include "applicationstate.h"
#include <QWidget>

class CMainController;
class QComboBox;
class QTableWidget;

class CPopulationControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPopulationControlWidget(QWidget *parent = nullptr);

    void updateTable();
protected:
    CMainController& _mainController;
//    QComboBox* _pComboBoxMainEvent;
//    QComboBox* _pComboBoxPopulationSplit;
    QTableWidget* _pTableSubPopulations;

    CApplicationState _applicationState;
signals:

public slots:
protected slots:
    void onMainControllerActiveApplicationStateUpdated(int iActiveApplicationStateIndex, const CApplicationState& applicationState);
};

#endif // POPULATIONCONTROLWIDGET_H
