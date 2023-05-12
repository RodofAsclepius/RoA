#include "nodeslist.h"

#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>

CNodesList::CNodesList()
    :QList<CNode>()
{
    qRegisterMetaType<CNodesList>("CNodesList");
//    qRegisterMetaTypeStreamOperators<CNodesList>("CNodesList");
}

CNodesList::CNodesList(const CNodesList& lstNodes)
    : QList<CNode>(), CPropertiesInterface()
{
    this->operator =(lstNodes);
}

CNodesList::~CNodesList()
{

}

CNodesList& CNodesList::operator=(const CNodesList& lstNodes)
{
    if (this != &lstNodes)
    {
        QMutexLocker mutLocker(&_mutex);
        QList<CNode>::operator =(lstNodes);
        mutLocker.unlock();
        setID(lstNodes.getID());
    }
    return* this;
}


bool CNodesList::operator==(const CNodesList& lstNodes)
{
    return (_lID == lstNodes.getID());
}


bool CNodesList::operator<(const CNodesList& lstNodes) const
{
    return (getID() < lstNodes.getID());
}

void CNodesList::append(const CNode &node)
{
//    QMutexLocker mutLocker(&_mutex);

    if(!contains(node))
    {
        QList<CNode>::append(node);
    }
    else
    {
        replace(node);
    }
}


void CNodesList::append(const CNodesList &lstNodes)
{
    foreach(CNode node, lstNodes)
    {
        append(node);
    }
}


void CNodesList::replace(int iIndex, const CNode& node)
{
    QMutexLocker mutLocker(&_mutex);

    removeAt(iIndex);
    insert(iIndex, node);
}


void CNodesList::replace(const CNode& node)
{
    int iIndex =indexOf(node);
    if(iIndex != -1)
    {
        replace(iIndex, node);
    }
    else
    {
        append(node);
    }
}


void CNodesList::replace(const CNodesList& lstNodes)
{
    foreach(CNode node, lstNodes)
    {
        replace(node);
    }
}


void CNodesList::update(const CNodesList &lstNodes, bool bAppendNewnode)
{
    /* Add or update items */
    foreach(CNode node, lstNodes)
    {
        /* Get current one based on ID */
        int iIndex = indexOf(node);

        if(iIndex == -1)
        {
            if(bAppendNewnode)
            {
                append(node);
            }
        }
        else
        {
            /* Merge all properties */
            CNode currentnode = at(iIndex);
            currentnode.update(node);
            replace(iIndex, currentnode);
        }
    }
}


QJsonObject CNodesList::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObjectResult;

    for(int iIndex = 0; iIndex < count(); ++iIndex)
    {
        CNode node = at(iIndex);
        jsonObjectResult[node.label()] = node.toJsonObject(bIncludeProperties);
    }

//    if(bIncludeProperties)
//    {
//        jsonObjectResult["properties"] = VariantMaptoJsonObject(_mapProperties);
//    }

    return jsonObjectResult;
}

void CNodesList::fromJsonArray(const QJsonArray &jsonArray)
{
//    DEBUG_OBJ_INFO

    clear();

    for(int i=0; i < jsonArray.count(); ++i)
    {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObjectnode = jsonValue.toObject();

        CNode node;
        node.fromJsonObject(jsonObjectnode);

        /* Combine result */
        append(node);
    }
}

void CNodesList::fromJsonObject(const QJsonObject &jsonObject)
{
    clear();

    QStringList lstKeys = jsonObject.keys();
    foreach(QString strKey, lstKeys)
    {
        QJsonValue jsonValue = jsonObject[strKey];
        QJsonObject jsonObjectgroup = jsonValue.toObject();

        CNode node;
        node.fromJsonObject(jsonObjectgroup);

        /* Combine result */
        append(node);
    }
}

QStringList CNodesList::nodesLabels() const
{
    QStringList lstNames;

    for(int i=0; i<count(); ++i)
    {
        lstNames << at(i).label();
    }

    return lstNames;
}

CNode CNodesList::byName(const QString &strName) const
{
    for(int i=0; i<count(); ++i)
    {
        CNode node = at(i);
        if(node.label() == strName)
        {
            return node;
        }
    }

    return CNode();
}

int CNodesList::indexByLabel(const QString &strName) const
{
    for(int i=0; i<count(); ++i)
    {
        CNode node = at(i);
        if(node.label() == strName)
        {
            return indexOf(node);
        }
    }

    return -1;
}

QDataStream& operator<<(QDataStream& out, const CNodesList& lstNodes)
{
    out << static_cast <const QList<CNode>& >( lstNodes );
    out << lstNodes.getID();
    out << lstNodes.getProperties();

    return out;
}


QDataStream& operator>>(QDataStream& in, CNodesList& lstNodes)
{
    quint32 lID;
    QVariantMap mapProperties;

    in >> static_cast <QList<CNode>& >( lstNodes );
    in >> lID;
    lstNodes.setID(lID);
    in >> mapProperties;

    lstNodes.setProperties(mapProperties);

    return in;
}

