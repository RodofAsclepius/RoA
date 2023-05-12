#include "treeitem.h"
#include <QDebug>
#include <QMutexLocker>

CTreeItem::CTreeItem(CTreeItem *parent)
    : parentItem(parent)
{}

CTreeItem::~CTreeItem()
{
    qDeleteAll(childItems);
}

CTreeItem *CTreeItem::child(int number)
{
    if (number < 0 || number >= childItems.size())
        return nullptr;
    return childItems.at(number);
}

int CTreeItem::childCount() const
{
    return childItems.count();
}

int CTreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<CTreeItem*>(this));
    return 0;
}

void CTreeItem::clearChildren()
{
    qDeleteAll(childItems);
}

int CTreeItem::columnCount() const
{
    return itemData.count();
}

void CTreeItem::setColumnCount(int iCount)
{
    itemData.resize(iCount);
    itemData.squeeze();
}

QVariant CTreeItem::data(int column) const
{
    if (column < 0 || column >= itemData.size() || itemData.isEmpty())
        return QVariant();

    return itemData.at(column);
}

bool CTreeItem::insertChildren(int position, int count)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
        CTreeItem *item = new CTreeItem(this);
        item->setColumnCount(itemData.count());

        childItems.insert(position, item);
    }

    return true;
}

bool CTreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    for (CTreeItem *child : qAsConst(childItems))
        child->insertColumns(position, columns);

    return true;
}

CTreeItem *CTreeItem::parent()
{
    return parentItem;
}

bool CTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool CTreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    for (CTreeItem *child : qAsConst(childItems))
        child->removeColumns(position, columns);

    return true;
}

bool CTreeItem::setData(int column, const QVariant &value)
{
    if (column < 0)
        return false;

    if (column >= itemData.size() || itemData.isEmpty())
    {
        itemData.resize(column + 1);
    }

    itemData[column] = value;
    return true;
}

void CTreeItem::clearData()
{
    itemData.clear();
}
