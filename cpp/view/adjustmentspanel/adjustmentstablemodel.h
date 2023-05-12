#ifndef ADJUSTMENTSTABLEMODEL_H
#define ADJUSTMENTSTABLEMODEL_H

#include <QDate>
#include <QSortFilterProxyModel>
#include "causalmodellist.h"
#include "groupslist.h"
#include "causalpaneladjacencypanelmultiedge.h"

class CAdjustmentsTableModel : public QAbstractTableModel
{
public:
    CAdjustmentsTableModel(QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void clear();

    CCausalModelList causalModels() const;
    void setCausalModels(const CCausalModelList &causalModels);

    enum {
        NODE1,
        LEFTARROWS,
        RIGHTARROWS,
        NODE2
    } Columns;

protected:
    void updateModel();
private:
    /* Original data */
    CCausalModelList lstCausalModels_;

    /* Extracted data */
    CGroupsList lstGroups_;
    QVector<QPair<QString, QString>> dataNodes_;
    QMap<QPair<QString, QString>, CCausalPanelAdjacencyPanelMultiEdge> dataEdges_;

    bool bBidirectionMode_ = true;
};



#endif // ADJUSTMENTSTABLEMODEL_H

