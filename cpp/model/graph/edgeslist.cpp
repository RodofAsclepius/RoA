#include "edgeslist.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>

CEdgesList::CEdgesList()
    :QList<CEdge>()
{
    qRegisterMetaType<CEdgesList>("CEdgesList");
//    qRegisterMetaTypeStreamOperators<CEdgesList>("CEdgesList");
}

CEdgesList::CEdgesList(const CEdgesList& lstEdges)
    : QList<CEdge>(), CPropertiesInterface()
{
    this->operator =(lstEdges);
}

CEdgesList::~CEdgesList()
{

}

CEdgesList& CEdgesList::operator=(const CEdgesList& lstEdges)
{
    if (this != &lstEdges)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CEdge>::operator =(lstEdges);
        mutLocker.unlock();
        setID(lstEdges.getID());
    }
    return* this;
}


bool CEdgesList::operator==(const CEdgesList& lstEdges)
{
    return (_lID == lstEdges.getID());
}


bool CEdgesList::operator<(const CEdgesList& lstEdges) const
{
    return (getID() < lstEdges.getID());
}

void CEdgesList::append(const CEdge &Edge)
{
    QMutexLocker mutLocker(&_mutex);

    if(!contains(Edge))
    {
        QList<CEdge>::append(Edge);
    }
    else
    {
        replace(Edge);
    }
}


void CEdgesList::append(const CEdgesList &lstEdges)
{
    foreach(CEdge edge, lstEdges)
    {
        append(edge);
    }
}


void CEdgesList::replace(int iIndex, const CEdge& edge)
{
    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, edge);
}


void CEdgesList::replace(const CEdge& edge)
{
    int iIndex =indexOf(edge);
    if(iIndex != -1)
    {
        replace(iIndex, edge);
    }
    else
    {
        append(edge);
    }
}


void CEdgesList::replace(const CEdgesList& lstEdges)
{
    foreach(CEdge edge, lstEdges)
    {
        replace(edge);
    }
}


void CEdgesList::update(const CEdgesList &lstEdges, bool bAppendNewEdge)
{
    /* Add or update items */
    foreach(CEdge edge, lstEdges)
    {
        /* Get current one based on ID */
        int iIndex = indexOf(edge);

        if(iIndex == -1)
        {
            if(bAppendNewEdge)
            {
                append(edge);
            }
        }
        else
        {
            /* Merge all properties */
            CEdge currentEdge = at(iIndex);
            currentEdge.update(edge);
            replace(iIndex, currentEdge);
        }
    }
}


QJsonObject CEdgesList::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CEdge edge = at(iIndex);
        jsonObjectResult[QString::number(edge.getID())] = edge.toJsonObject(bIncludeProperties);
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonObjectResult;
}

void CEdgesList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObjectEdge = jsonValue.toObject();

        CEdge edge;
        edge.fromJsonObject(jsonObjectEdge);

        /* Combine result */
        append(edge);
    }
}

void CEdgesList::fromJsonObject(const QJsonObject &jsonObject)
{
    clear();

    QStringList lstKeys = jsonObject.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObject[strKey];
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CEdge edge;
        edge.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(edge);
    }
}

QStringList CEdgesList::edgesLabels() const
{
    QStringList lstNames;

    for(int i=0; i<count(); ++i)
    {
        lstNames << at(i).label();
    }

    return lstNames;
}

CEdge CEdgesList::byLabel(const QString &strName) const
{
    for(int i=0; i<count(); ++i)
    {
        CEdge edge = at(i);
        if(edge.label() == strName)
        {
            return edge;
        }
    }

    return CEdge();
}

int CEdgesList::indexByName(const QString &strName) const
{
    for(int i=0; i<count(); ++i)
    {
        CEdge edge = at(i);
        if(edge.label() == strName)
        {
            return indexOf(edge);
        }
    }

    return -1;
}

QDataStream& operator<<(QDataStream& out, const CEdgesList& lstEdges)
{
    out << static_cast <const QList<CEdge>& >( lstEdges );
    out << lstEdges.getID();
    out << lstEdges.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CEdgesList& lstEdges)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CEdge>& >( lstEdges );
    in >> lID;
    lstEdges.setID(lID);
    in >> mapProperties;

    lstEdges.setProperties(mapProperties);

    return in;
}

