#ifndef BALANCEPANELMODEL_H
#define BALANCEPANELMODEL_H

#include "treemodel.h"
#include "groupslist.h"
#include "variableslist.h"

//#define COLUMN_VARIABLE_SPACER 0
#define COLUMN_VARIABLE 0
#define COLUMN_DISTRIBUTION 1
#define COLUMN_CURVES 2
#define COLUMN_GROUPWEIGHTS_OFFSET 3
#define COLUMN_COUNT_DEFAULT 3

#define COLUMN_DISTRIBUTION_WIDTH 128
//#define COLUMN_VARIABLE_WIDTH 200
#define COLUMN_VARIABLE_WIDTH 0
#define COLUMN_CURVES_WIDTH 60
#define COLUMN_GROUP_WIDTH 30
#define MARGIN 0
#define HEADER_HEIGHT 24
#define ROW_HEIGHT 96
#define MINIMUM_WIDTH (COLUMN_DISTRIBUTION_WIDTH+3*COLUMN_GROUP_WIDTH + COLUMN_VARIABLE_WIDTH) + MARGIN

class CBalancePanelTreeModel : public CTreeModel
{
    Q_OBJECT

public:
    CBalancePanelTreeModel(const QStringList &headers = QStringList(), QObject *parent = nullptr);
    ~CBalancePanelTreeModel() override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    CVariablesList variables() const;
    void setVariables(const CVariablesList &variables);

    CGroupsList groups() const;
    void setGroups(const CGroupsList &groups);

    Qt::ItemFlags flags(const QModelIndex &index) const override;

protected:
    CGroupsList lstGroups_;
    CVariablesList lstVariables_;

    void processData();
};

#endif // TREEMODEL_H


