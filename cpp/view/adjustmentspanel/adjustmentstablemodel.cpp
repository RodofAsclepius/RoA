#include "adjustmentstablemodel.h"

#include "utils.h"
#include <QSize>

bool compareNodeNames(const CNode& nodeA, const CNode& nodeB)
{
    return nodeA.label() < nodeB.label();
}

int CAdjustmentsTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 4;
//    return 6;
}

CAdjustmentsTableModel::CAdjustmentsTableModel(QObject *parent)
    :   QAbstractTableModel(parent)
{

}

int CAdjustmentsTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return dataNodes_.size();
}

QVariant CAdjustmentsTableModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= dataNodes_.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        QPair<QString, QString> pairNodes =  dataNodes_.at(index.row());

        switch (index.column())
        {
        case 0:
            return pairNodes.first;
        break;
        case 1:
        {
            const CCausalPanelAdjacencyPanelMultiEdge dataEdge = dataEdges_.value(pairNodes);
            return QVariant::fromValue(dataEdge);
        }
        break;
        case 2:
        {
            const CCausalPanelAdjacencyPanelMultiEdge dataEdge = dataEdges_.value(pairNodes);
            return QVariant::fromValue(dataEdge);
        }
        break;
        case 3:
            return pairNodes.second;
        break;
        default:
            return QVariant();
        }
    }
    else if (role == Qt::TextAlignmentRole){
        if (index.column() == 0)
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        else
            return Qt::AlignCenter;
    }

    if (role == Qt::UserRole || role == Qt::EditRole)
    {
//        switch (index.column()) {
//        case 0:
//        {
//            QVariant parentsData = QVariant::fromValue(incomingEdges_.at(index.row()));
//            return parentsData;
//        }
//        case 1:
//        {
//            QVariant nodeData = QVariant::fromValue(nodes_.at(index.row()));
//            return nodeData;
//        }
//        case 2:
//        {
//            QVariant childrenData = QVariant::fromValue(outgoingEdges_.at(index.row()));
//            return childrenData;
//        }
//        default:
//            return QVariant();
//        }
    }

    return QVariant();
}

QVariant CAdjustmentsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    /* Display role */
    if(role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return tr("Node");
            case 1:
            {
                return QVariant::fromValue(lstGroups_);
            }
            case 2:
                return QVariant::fromValue(lstGroups_);
            case 3:
                return tr("Node");
            default:
                return section + 1;
            }
        }
        if (orientation == Qt::Vertical)
        {
            switch (section)
            {
            default:
                return section + 1;
            }
        }
    }

    /* Sizehint */
    if(role == Qt::SizeHintRole)
    {
        qreal rScaleFactor = 1;
        int iHeaderHeight = rScaleFactor * 20;
        int iRowHeight = rScaleFactor * 14;
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return QSize(rScaleFactor*200, iHeaderHeight);
            case 1:
                return QSize(rScaleFactor*50, iHeaderHeight);
            case 2:
                return QSize(rScaleFactor*50, iHeaderHeight);
            case 3:
                return QSize(rScaleFactor*200, iHeaderHeight);
            default:
                return QVariant();
            }
        }
        if (orientation == Qt::Vertical)
        {
           return QSize(0, rScaleFactor * iRowHeight);
        }
    }

    return QVariant();
}

void CAdjustmentsTableModel::clear()
{
    lstCausalModels_.clear();

    updateModel();
}

void CAdjustmentsTableModel::updateModel()
{
    DEBUG_OBJ_INFO

    beginResetModel();

    lstGroups_.clear();
    dataNodes_.clear();
    dataEdges_.clear();

    /* Store groups and edges */
    foreach(CCausalModel causalModel, lstCausalModels_)
    {
        CGroup group = causalModel.group();
        lstGroups_.append(group);

        CGraph graphAlg = causalModel.graphAlg();
        CGraph graphUser = causalModel.graphUser();

        /* Extract alg arrows */
        QList<CEdge> lstEdgesAlg = graphAlg.edges();
        foreach(CEdge edge, lstEdgesAlg)
        {
            QPair<QString, QString> pairNodes = qMakePair<QString, QString>(edge.sourceNode().label(), edge.targetNode().label());

            if(!dataNodes_.contains(pairNodes))
            {
                dataNodes_.append(pairNodes);
            }

            CCausalPanelAdjacencyPanelMultiEdge multiEdge = dataEdges_.value(pairNodes);
            multiEdge.addGroupArrowAlg(pairNodes.first, pairNodes.second, group);
            dataEdges_.insert(pairNodes, multiEdge);
        }

        /* Extract user arrows */
        QList<CEdge> lstEdgesUser = graphUser.edges();
        foreach(CEdge edge, lstEdgesUser)
        {
            QPair<QString, QString> pairNodes = qMakePair<QString, QString>(edge.sourceNode().label(), edge.targetNode().label());

            if(!dataNodes_.contains(pairNodes))
            {
                dataNodes_.append(pairNodes);
            }

            CCausalPanelAdjacencyPanelMultiEdge multiEdge = dataEdges_.value(pairNodes);
            multiEdge.addGroupArrowUser(pairNodes.first, pairNodes.second, group);
            dataEdges_.insert(pairNodes, multiEdge);
        }
    }

    endResetModel();

    qDebug() << lstCausalModels_;
    qDebug() << dataNodes_.count();
    qDebug() << dataEdges_.count();
}

CCausalModelList CAdjustmentsTableModel::causalModels() const
{
    return lstCausalModels_;
}

void CAdjustmentsTableModel::setCausalModels(const CCausalModelList &lstCausalModels)
{
    lstCausalModels_ = lstCausalModels;

    updateModel();
}
