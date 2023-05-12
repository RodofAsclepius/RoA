#ifndef CVARIABLE_H
#define CVARIABLE_H

#include "propertiesinterface.h"

#include <QString>
#include <QMutex>
#include <QMetaType>
#include <QList>
#include <QDataStream>
#include <QMap>
#include <QVariant>

static inline QString variablesMimeType() { return QStringLiteral("application/x-variable"); }

class CVariable : public CPropertiesInterface
{
    Q_OBJECT
public:
    CVariable(const QString& strName = QString(), const QString& strType = QString());
    CVariable(const CVariable& variable);
    virtual ~CVariable();

    CVariable& operator=(const CVariable& variable);
    virtual bool operator==(const CVariable& variable) const;
    virtual bool operator<(const CVariable& Variable) const;
    friend QDataStream& operator>>(QDataStream& in, CVariable& variable);

    QString name() const;
    void setName(const QString& strName);
    QString type() const;
    void setType(const QString& strType);

    void update(const CVariable& variable);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonObject(const QJsonObject& jsonObject);
    QStringList levels() const;
    void setLevels(const QStringList &levels);

protected:
    QString strName_;
    QString strType_;
    QStringList lstLevels_;

    friend uint qHash(const CVariable &key, uint seed);
};

Q_DECLARE_METATYPE(CVariable)

QDataStream& operator<<(QDataStream& out, const CVariable& variable);
QDataStream& operator>>(QDataStream& in, CVariable& variable);
QDebug operator<<(QDebug dbg, const CVariable& variable);

inline uint qHash(const CVariable &key, uint seed = 0)
{
    Q_UNUSED(seed)

    return qHash(key.getID());// * key.getName().length();
}

#endif // CVARIABLE_H
