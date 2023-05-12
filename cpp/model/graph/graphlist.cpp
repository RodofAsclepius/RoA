#include "graphlist.h"
#include "utils.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>

CGraphList::CGraphList()
    :QList<CGraph>()
{
    qRegisterMetaType<CGraphList>("CGraphList");
//    qRegisterMetaTypeStreamOperators<CGraphList>("CGraphList");
}


CGraphList::CGraphList(const CGraphList& lstgraphs)
    : QList<CGraph>(), CPropertiesInterface()
{
    this->operator =(lstgraphs);
}


CGraphList::~CGraphList()
{

}


CGraphList& CGraphList::operator=(const CGraphList& lstgraphs)
{
    if (this != &lstgraphs)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CGraph>::operator =(lstgraphs);
        mutLocker.unlock();
        setID(lstgraphs.getID());
    }
    return* this;
}


bool CGraphList::operator==(const CGraphList& lstGraphs)
{
    return (_lID == lstGraphs.getID());
}


bool CGraphList::operator<(const CGraphList& lstGraphs) const
{
    return (getID() < lstGraphs.getID());
}

void CGraphList::append(const CGraph &graph)
{
    QMutexLocker mutLocker(&_mutex);

    if(!contains(graph))
    {
        QList<CGraph>::append(graph);
    }
    else
    {
        replace(graph);
    }
}


void CGraphList::append(const CGraphList &lstGraphs)
{
    foreach(CGraph graph, lstGraphs)
    {
        append(graph);
    }
}


void CGraphList::replace(int iIndex, const CGraph& graph)
{
    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, graph);
}


void CGraphList::replace(const CGraph& graph)
{
    int iIndex = indexOf(graph);
    if(iIndex != -1)
    {
        replace(iIndex, graph);
    }
}


void CGraphList::replace(const CGraphList& lstgraphs)
{
    foreach(CGraph graph, lstgraphs)
    {
        replace(graph);
    }
}

QJsonObject CGraphList::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CGraph graph = at(iIndex);
//        jsonObjectResult[graph.name()] = graph.toJsonObject(bIncludeProperties);
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonObjectResult;
}

QJsonArray CGraphList::toJsonArray(bool bIncludeProperties) const
{
    QJsonArray jsonArrayResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CGraph graph = at(iIndex);
//        jsonArrayResult.append(graph.toJsonObject(bIncludeProperties));
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonArrayResult;
}

void CGraphList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObjectgraph = jsonValue.toObject();

        CGraph graph;
//        graph.fromJsonObject(jsonObjectgraph);

        /* Combine result */
        append(graph);
    }
}

void CGraphList::fromJsonObject(const QJsonObject &jsonObj)
{
    clear();

    QStringList lstKeys = jsonObj.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObj[strKey];
        QJsonObject jsonObjectgraph = jsonValue.toObject();

        CGraph graph;
//        graph.fromJsonObject(jsonObjectgraph);

        /* Combine result */
        append(graph);
    }
}

QDataStream& operator<<(QDataStream& out, const CGraphList& lstgraphs)
{
    out << static_cast <const QList<CGraph>& >( lstgraphs );
    out << lstgraphs.getID();
    out << lstgraphs.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CGraphList& lstgraphs)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CGraph>& >( lstgraphs );
    in >> lID;
    in >> mapProperties;

    lstgraphs.setID(lID);
    lstgraphs.setProperties(mapProperties);

    return in;
}

