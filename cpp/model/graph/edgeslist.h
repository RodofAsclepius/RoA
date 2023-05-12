#ifndef CEDGESLIST_H
#define CEDGESLIST_H

#include "edge.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

//static inline QString edgeListMimeType() { return QStringLiteral("application/x-edgelist"); }

class CEdgesList : public QList<CEdge>, public CPropertiesInterface
{
public:
    CEdgesList();
    ~CEdgesList();
    CEdgesList(const CEdgesList& lstedges);
    CEdgesList& operator=(const CEdgesList& lstedges);
    virtual bool operator==(const CEdgesList& lstedges);
    virtual bool operator<(const CEdgesList& lstedges) const;
    friend QDataStream& operator>>(QDataStream& in, CEdgesList& lstedges);

    void append(const CEdge& edge);
    void append(const CEdgesList& lstedges);

    void replace(int iIndex, const CEdge& edge);
    void replace(const CEdge& edge);
    void replace(const CEdgesList& lstedges);

    void update(const CEdgesList& lstedges, bool bAppendNewedge = true);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObject);

    QStringList edgesLabels() const;

    CEdge byLabel(const QString &strName) const;
    int indexByName(const QString &strName) const;

protected:

};

Q_DECLARE_METATYPE(CEdgesList)

QDataStream& operator<<(QDataStream& out, const CEdgesList& lstedges);
QDataStream& operator>>(QDataStream& in, CEdgesList& lstedges);

bool edgeNameLessThan(const CEdge &v1, const CEdge &v2);

#endif // CEDGESLIST_H
