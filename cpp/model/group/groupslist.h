#ifndef CGROUPSLIST_H
#define CGROUPSLIST_H

#include "group.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

class CGroupsList : public QList<CGroup>, public CPropertiesInterface
{
public:
    CGroupsList();
    ~CGroupsList();
    CGroupsList(const CGroupsList& lstGroups);
    CGroupsList& operator=(const CGroupsList& lstGroups);
    virtual bool operator==(const CGroupsList& lstGroups);
    virtual bool operator<(const CGroupsList& lstGroups) const;
    friend QDataStream& operator>>(QDataStream& in, CGroupsList& lstGroups);

    void append(const CGroup& group);
    void append(const CGroupsList& lstGroups);

    void replace(int iIndex, const CGroup& group);
    void replace(const CGroup& group);
    void replace(const CGroupsList& lstGroups);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    QJsonArray toJsonArray(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObj);

    CGroup byName(const QString &strName) const;
    CGroup byID(quint32 id) const;

    QStringList groupsNames() const;
    CGroupsList groupsEnabled() const;
    CGroupsList groupsDisabled() const;
protected:
};

Q_DECLARE_METATYPE(CGroupsList)

QDataStream& operator<<(QDataStream& out, const CGroupsList& lstGroups);
QDataStream& operator>>(QDataStream& in, CGroupsList& lstGroups);

#endif // CGROUPSLIST_H
