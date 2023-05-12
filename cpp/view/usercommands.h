#ifndef USERCOMMANDS_H
#define USERCOMMANDS_H

#include "maincontroller.h"
#include "variable.h"
#include "populationsselectortreenode.h"

#include <QUndoCommand>

class CUpdateVariableCommand : public QUndoCommand
{
public:
    CUpdateVariableCommand(const CVariable& variable, QUndoCommand *parent = 0);
    ~CUpdateVariableCommand();

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    CMainController& _mainController;
    CVariable _originalVariable;
    CVariable _updatedVariable;
};

/* Populations */
class CUpdateMainPopulationsSelectorTreeCommand : public QUndoCommand
{
public:
    CUpdateMainPopulationsSelectorTreeCommand(const CPopulationsSelectorTreeNode& mainPopulationsSelectorTreeNode, QUndoCommand *parent = 0);
    ~CUpdateMainPopulationsSelectorTreeCommand();

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    CMainController& _mainController;
    CPopulationsSelectorTreeNode _originalPopulationsSelectorTreeNode;
    CPopulationsSelectorTreeNode _updatedPopulationsSelectorTreeNode;
};


#endif // USERCOMMANDS_H
