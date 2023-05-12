#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include <QList>
#include <QMutex>

class CTreeItem
{
public:
    explicit CTreeItem(CTreeItem *parent = nullptr);
    ~CTreeItem();

    CTreeItem *child(int number);
    int childCount() const;
    bool insertChildren(int position, int count);
    bool insertColumns(int position, int columns);
    CTreeItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    void clearChildren();

    int columnCount() const;
    void setColumnCount(int iCount);
    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);
    void clearData();
private:
    mutable QMutex mutex_;
    QVector<CTreeItem*> childItems;
    mutable QVector<QVariant> itemData;
    CTreeItem *parentItem;
};

#endif // TREEITEM_H
