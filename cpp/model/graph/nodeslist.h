#ifndef CNODESLIST_H
#define CNODESLIST_H


#include "node.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

//static inline QString nodeListMimeType() { return QStringLiteral("application/x-nodelist"); }

class CNodesList : public QList<CNode>, public CPropertiesInterface
{
public:
    CNodesList();
    ~CNodesList();
    CNodesList(const CNodesList& lstnodes);
    CNodesList& operator=(const CNodesList& lstnodes);
    virtual bool operator==(const CNodesList& lstnodes);
    virtual bool operator<(const CNodesList& lstnodes) const;
    friend QDataStream& operator>>(QDataStream& in, CNodesList& lstnodes);

    void append(const CNode& node);
    void append(const CNodesList& lstnodes);

    void replace(int iIndex, const CNode& node);
    void replace(const CNode& node);
    void replace(const CNodesList& lstnodes);

    void update(const CNodesList& lstnodes, bool bAppendNewnode = true);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObject);

    QStringList nodesLabels() const;

    CNode byName(const QString &strName) const;
    int indexByLabel(const QString &strName) const;

protected:

};

Q_DECLARE_METATYPE(CNodesList)

QDataStream& operator<<(QDataStream& out, const CNodesList& lstnodes);
QDataStream& operator>>(QDataStream& in, CNodesList& lstnodes);

bool nodeNameLessThan(const CNode &v1, const CNode &v2);

#endif // CNODESLIST_H
