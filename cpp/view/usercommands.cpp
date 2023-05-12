
#include "usercommands.h"
#include "utils.h"

CUpdateVariableCommand::CUpdateVariableCommand(const CVariable& variable, QUndoCommand *parent)
    : QUndoCommand(parent), _mainController(CMainController::getInstance())
{
//    CVariableList _lstOriginalVariables = _mainController.getVariables();

//    _originalVariable = _lstOriginalVariables.at(_lstOriginalVariables.indexOf(variable));
    _updatedVariable = variable;
}


CUpdateVariableCommand::~CUpdateVariableCommand()
{
}


void CUpdateVariableCommand::undo()
{
//    _mainController.updateVariable(_originalVariable);
    setText("update of variable");
}


void CUpdateVariableCommand::redo()
{
//    _mainController.updateVariable(_updatedVariable);
    setText("update of variable");
}


/* Populations */

CUpdateMainPopulationsSelectorTreeCommand::CUpdateMainPopulationsSelectorTreeCommand(
                                                    const CPopulationsSelectorTreeNode& populationsSelectorTreeNode,
                                                    QUndoCommand *parent)
    : QUndoCommand(parent), _mainController(CMainController::getInstance())
{
//    _originalPopulationsSelectorTreeNode = _mainController.getPopulationsSelectorTree();
    _updatedPopulationsSelectorTreeNode = populationsSelectorTreeNode;
}


CUpdateMainPopulationsSelectorTreeCommand::~CUpdateMainPopulationsSelectorTreeCommand()
{
}


void CUpdateMainPopulationsSelectorTreeCommand::undo()
{
//    _mainController.updatePopulationsSelectorTree(_originalPopulationsSelectorTreeNode);
    setText("update of the main populations selector tree");
}


void CUpdateMainPopulationsSelectorTreeCommand::redo()
{
//    _mainController.updatePopulationsSelectorTree(_updatedPopulationsSelectorTreeNode);
    setText("update of the main populations selector tree");
}
