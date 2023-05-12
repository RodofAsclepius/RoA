#ifndef GRAPHLIST_H
#define GRAPHLIST_H

#include "graph.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

class CGraphList : public QList<CGraph>, public CPropertiesInterface
{
public:
    CGraphList();
    ~CGraphList();
    CGraphList(const CGraphList& lstGroups);
    CGraphList& operator=(const CGraphList& lstGroups);
    virtual bool operator==(const CGraphList& lstGroups);
    virtual bool operator<(const CGraphList& lstGroups) const;
    friend QDataStream& operator>>(QDataStream& in, CGraphList& lstGroups);

    void append(const CGraph& graph);
    void append(const CGraphList& lstGraphs);

    void replace(int iIndex, const CGraph& graph);
    void replace(const CGraph& graph);
    void replace(const CGraphList& lstGraphs);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    QJsonArray toJsonArray(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObj);
protected:
};

Q_DECLARE_METATYPE(CGraphList)

QDataStream& operator<<(QDataStream& out, const CGraphList& lstGraphs);
QDataStream& operator>>(QDataStream& in, CGraphList& lstGraphs);

#endif // GRAPHLIST_H
