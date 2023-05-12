#include "treemodel.h"
#include "treeitem.h"

#include <QtWidgets>

CTreeModel::CTreeModel(const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    pRootItem_ = new CTreeItem();
    setHorizontalHeaderLabels(headers);
}

CTreeModel::~CTreeModel()
{
    delete pRootItem_;
}

int CTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return pRootItem_->columnCount();
}

QVariant CTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::CheckStateRole)
        return QVariant();

    CTreeItem *item = getItem(index);

    item->data(index.column());
    return item->data(index.column());
}

Qt::ItemFlags CTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

CTreeItem *CTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        CTreeItem *item = static_cast<CTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return pRootItem_;
}

QVariant CTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return pRootItem_->data(section);

    return QVariant();
}

QModelIndex CTreeModel::index(int row, int column, const QModelIndex &parent) const
{
//    if (parent.isValid() && parent.column() != 0)
//        return QModelIndex();

    CTreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return QModelIndex();

    CTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

bool CTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    beginInsertColumns(parent, position, position + columns - 1);
    const bool success = pRootItem_->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool CTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    CTreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginInsertRows(parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position,
                                                    rows);
    endInsertRows();

    return success;
}

QModelIndex CTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    CTreeItem *childItem = getItem(index);
    CTreeItem *parentItem = childItem ? childItem->parent() : nullptr;

    if (parentItem == pRootItem_ || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool CTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    beginRemoveColumns(parent, position, position + columns - 1);
    const bool success = pRootItem_->removeColumns(position, columns);
    endRemoveColumns();

    if (pRootItem_->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool CTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    CTreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

void CTreeModel::setHorizontalHeaderLabels(const QStringList &lstLabels)
{
    for(int iCol=0; iCol<lstLabels.count(); ++iCol)
    {
        pRootItem_->setData(iCol, lstLabels.at(iCol));
    }
}

void CTreeModel::clearContents()
{
//    pRootItem_->clearData();
    beginResetModel();
    pRootItem_->clearChildren();
    endResetModel();
}

int CTreeModel::rowCount(const QModelIndex &parent) const
{
    const CTreeItem *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

bool CTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole && role != Qt::CheckStateRole)
        return false;

    CTreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole, Qt::CheckStateRole});

    return result;
}

bool CTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = pRootItem_->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void CTreeModel::setupModelData(const QStringList &lines, CTreeItem *parent)
{
//    QVector<CTreeItem*> parents;
//    QVector<int> indentations;
//    parents << parent;
//    indentations << 0;

//    int number = 0;

//    while (number < lines.count()) {
//        int position = 0;
//        while (position < lines[number].length()) {
//            if (lines[number].at(position) != ' ')
//                break;
//            ++position;
//        }

//        const QString lineData = lines[number].mid(position).trimmed();

//        if (!lineData.isEmpty()) {
//            // Read the column data from the rest of the line.
//            const QStringList columnStrings = lineData.split('\t', QString::SkipEmptyParts);
//            QVector<QVariant> columnData;
//            columnData.reserve(columnStrings.size());
//            for (const QString &columnString : columnStrings)
//                columnData << columnString;

//            if (position > indentations.last()) {
//                // The last child of the current parent is now the new parent
//                // unless the current parent has no children.

//                if (parents.last()->childCount() > 0) {
//                    parents << parents.last()->child(parents.last()->childCount()-1);
//                    indentations << position;
//                }
//            } else {
//                while (position < indentations.last() && parents.count() > 0) {
//                    parents.pop_back();
//                    indentations.pop_back();
//                }
//            }

//            // Append a new item to the current parent's list of children.
//            CTreeItem *parent = parents.last();
//            parent->insertChildren(parent->childCount(), 1);
//            for (int column = 0; column < columnData.size(); ++column)
//                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
//        }
//        ++number;
//    }
}
