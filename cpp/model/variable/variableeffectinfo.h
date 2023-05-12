#ifndef VARIABLEBEFFECTINFO_H
#define VARIABLEBEFFECTINFO_H

#include "propertiesinterface.h"
#include "distributionlist.h"

class CVariableEffectInfo : public CPropertiesInterface
{
    Q_OBJECT
public:
    CVariableEffectInfo();
    CVariableEffectInfo(const CVariableEffectInfo& effectInfo);
    virtual ~CVariableEffectInfo();

    CVariableEffectInfo& operator=(const CVariableEffectInfo& effectInfo);
    virtual bool operator==(const CVariableEffectInfo& effectInfo) const;
    virtual bool operator<(const CVariableEffectInfo& effectInfo) const;
    friend QDataStream& operator>>(QDataStream& in, CVariableEffectInfo& effectInfo);

    void fromJsonObject(QJsonObject objEffectInfo);

    QMap<QString, qreal> values() const;
    void setValues(const QMap<QString, qreal> &value);

    QString method() const;
    void setMethod(const QString &value);

protected:
    QMap<QString, qreal> mapValues_;
    QString strMethod = "Weighted mean difference";
//    qreal meanUn;
//    qreal meanAdj;
//    qreal stdErrorUn;
//    qreal stdErrorAdj;
};

Q_DECLARE_METATYPE(CVariableEffectInfo)

QDataStream& operator<<(QDataStream& out, const CVariableEffectInfo& variableEffectInfo);
QDataStream& operator>>(QDataStream& in, CVariableEffectInfo& variableEffectInfo);
QDebug operator<<(QDebug dbg, const CVariableEffectInfo& variableEffectInfo);

inline uint qHash(const CVariableEffectInfo &key, uint seed = 0)
{
    Q_UNUSED(seed)

    return qHash(key.getID());
}

#endif // VARIABLEBEFFECTINFO_H
