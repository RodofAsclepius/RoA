#ifndef CGROUP_H
#define CGROUP_H

#include "propertiesinterface.h"

#include <QString>
#include <QMutex>
#include <QMetaType>
#include <QDataStream>
#include <QColor>

class CGroup : public CPropertiesInterface
{
    Q_OBJECT
public:
    CGroup();
    CGroup(const QString& strName, const QString& strQuery);
    CGroup(const CGroup& Group);
    virtual ~CGroup();

    CGroup& operator=(const CGroup& Group);
    virtual bool operator==(const CGroup& Group) const;
    virtual bool operator<(const CGroup& Group) const;
    friend QDataStream& operator>>(QDataStream& in, CGroup& group);

    QString name() const;
    void setName(const QString& strName);
    int size() const;
    void setSize(int size);
    QString filterQuery() const;
    void setFilterQuery(const QString& strQuery);
    QColor colorPrimary() const;
    void setColorPrimary(const QColor &colorPrimary);
    QColor colorSecondary() const;
    void setColorSecondary(const QColor &colorSecondary);

    bool enabled() const;
    void setEnabled(bool enabled);

    QJsonObject toJsonObject(bool bIncludeProperties = false) const;
    void fromJsonObject(const QJsonObject& jsonObject);

protected:
    QString _strName;
    int _iSize = -1;
    QString _strLabel;
    QString _strFilterQuery;
    QColor clrPrimary_ = Qt::gray;
    QColor colorSecondary_ = Qt::darkGray;
    bool bEnabled_ = false;

    friend uint qHash(const CGroup &key, uint seed);
};

Q_DECLARE_METATYPE(CGroup)

QDataStream& operator<<(QDataStream& out, const CGroup& group);
QDataStream& operator>>(QDataStream& in, CGroup& group);
QDebug operator<<(QDebug dbg, const CGroup& group);

inline uint qHash(const CGroup &key, uint seed = 0)
{
    Q_UNUSED(seed)

    return qHash(key.getID());
}

#endif // CGROUP_H
