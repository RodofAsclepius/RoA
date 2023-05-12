#include "balancepaneltreemodel.h"
#include <QSize>
#include <QTreeWidgetItem>
#include <QDebug>

CBalancePanelTreeModel::CBalancePanelTreeModel(const QStringList &headers, QObject *parent)
    : CTreeModel(headers, parent)
{
}

CBalancePanelTreeModel::~CBalancePanelTreeModel()
{

}

int CBalancePanelTreeModel::columnCount(const QModelIndex &parent) const
{
    int iGroupCount = lstGroups_.count();
    return COLUMN_COUNT_DEFAULT + iGroupCount;
}

QVariant CBalancePanelTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int iGroupCount = lstGroups_.count();
    if (role == Qt::SizeHintRole)
    {
        if(index.column() == COLUMN_VARIABLE)
        {
            return QVariant(QSize(COLUMN_VARIABLE_WIDTH, ROW_HEIGHT));
        }

        if(index.column() == COLUMN_DISTRIBUTION)
        {
            return QVariant(QSize(COLUMN_DISTRIBUTION_WIDTH, ROW_HEIGHT));
        }

        if(index.column() == COLUMN_CURVES)
        {
            return QVariant(QSize(0, ROW_HEIGHT));
        }

        if(index.column() >= COLUMN_COUNT_DEFAULT && index.column() < (COLUMN_COUNT_DEFAULT + iGroupCount))
        {
            return QVariant(QSize(COLUMN_GROUP_WIDTH, ROW_HEIGHT));
        }

        return QVariant();
    }

    /* Check state */
    if(role == Qt::CheckStateRole)
    {
        if(index.column() >= COLUMN_COUNT_DEFAULT && index.column() < (COLUMN_COUNT_DEFAULT + iGroupCount))
        {
            QVariant varData = CTreeModel::data(index, role);

            int iCheckState = varData.toInt();
            if(iCheckState == Qt::CheckState::Checked)
            {
                return Qt::CheckState::Checked;
            }
            if(iCheckState == Qt::CheckState::Unchecked)
            {
                return Qt::CheckState::Unchecked;
            }
            if(iCheckState == Qt::CheckState::PartiallyChecked)
            {
                return Qt::CheckState::PartiallyChecked;
            }

            return QVariant();
        }
    }

    if(role == Qt::DisplayRole || role == Qt::EditRole || role >= Qt::UserRole)
    {
        return CTreeModel::data(index, role);
    }

    return CTreeModel::data(index, role);

//    return QVariant();
}

QVariant CBalancePanelTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    int iGroupCount = lstGroups_.count();

    if (orientation == Qt::Horizontal && role == Qt::SizeHintRole)
    {
        /* Curves */
        if(section == COLUMN_CURVES)
        {
            return QVariant(QSize(0, HEADER_HEIGHT));
        }

        /* Variable name */
        if(section == COLUMN_VARIABLE)
        {
            return QVariant(QSize(COLUMN_VARIABLE_WIDTH, HEADER_HEIGHT));
        }

        /* Variable distribution */
        if(section == COLUMN_DISTRIBUTION)
        {
            return QVariant(QSize(COLUMN_DISTRIBUTION_WIDTH, HEADER_HEIGHT));
        }

        /* Groups */
        if(section >= COLUMN_GROUPWEIGHTS_OFFSET)
        {
            return QVariant(QSize(COLUMN_GROUP_WIDTH, HEADER_HEIGHT));
        }
    }

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        /* Variable */
        if(section == COLUMN_VARIABLE)
        {
            return QString("Variable");
        }

        /* Distribution */
        if(section == COLUMN_DISTRIBUTION)
        {
            return QString("Distribution");
        }

        /* Curves */
        if(section == COLUMN_CURVES)
        {
            return QString("");
        }

        /* Groups */
        if(section >= COLUMN_GROUPWEIGHTS_OFFSET)
        {
            int iGroupIndex = section - COLUMN_GROUPWEIGHTS_OFFSET;
            if(iGroupIndex < lstGroups_.count())
            {
                return QVariant::fromValue(lstGroups_.at(iGroupIndex));
            }
            else
            {

            }

            return QVariant();
        }
    }

    return CTreeModel::headerData(section, orientation, role);
}

bool CBalancePanelTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int iGroupCount = lstGroups_.count();

    /* Check state */
    if(role == Qt::CheckStateRole)
    {
        if(index.column() >= COLUMN_COUNT_DEFAULT && index.column() < (COLUMN_COUNT_DEFAULT + iGroupCount))// && !index.parent().isValid())
        {
//            qDebug() << index << index.parent() << index.parent().isValid();

            int iCheckState = value.toInt();
            if(iCheckState == Qt::CheckState::Checked ||
               iCheckState == Qt::CheckState::Unchecked ||
               iCheckState == Qt::CheckState::PartiallyChecked)
            {
                return CTreeModel::setData(index, QVariant::fromValue(iCheckState), role);
            }
        }
    }

    if(role == Qt::DisplayRole || role == Qt::EditRole || role >= Qt::UserRole)
    {
        return CTreeModel::setData(index, value, role);
    }

    return CTreeModel::setData(index, value, role);
}

CVariablesList CBalancePanelTreeModel::variables() const
{
    return lstVariables_;
}

void CBalancePanelTreeModel::setVariables(const CVariablesList &lstVariables)
{
    beginResetModel();
    lstVariables_ = lstVariables;
    endResetModel();
}

CGroupsList CBalancePanelTreeModel::groups() const
{
    return lstGroups_;
}

void CBalancePanelTreeModel::setGroups(const CGroupsList &lstGroups)
{
    beginResetModel();
    lstGroups_ = lstGroups;
    endResetModel();
}

Qt::ItemFlags CBalancePanelTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = CTreeModel::flags(index);
    if (index.isValid() && index.column() >= COLUMN_GROUPWEIGHTS_OFFSET)
    {
        return defaultFlags | Qt::ItemIsUserCheckable;
    }
    return defaultFlags;
}
