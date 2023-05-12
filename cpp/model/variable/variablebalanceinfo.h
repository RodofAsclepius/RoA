#ifndef CVARIABLEBALANCEINFO_H
#define CVARIABLEBALANCEINFO_H

#include "propertiesinterface.h"

class CVariableBalanceInfo : public CPropertiesInterface
{
    Q_OBJECT
public:
    CVariableBalanceInfo();
    CVariableBalanceInfo(const CVariableBalanceInfo& balanceInfo);
    virtual ~CVariableBalanceInfo();

    CVariableBalanceInfo& operator=(const CVariableBalanceInfo& balanceInfo);
    virtual bool operator==(const CVariableBalanceInfo& balanceInfo) const;
    virtual bool operator<(const CVariableBalanceInfo& balanceInfo) const;
    friend QDataStream& operator>>(QDataStream& in, CVariableBalanceInfo& balanceInfo);

    void fromJsonObject(QJsonObject objBalanceInfo);

    QMap<QString, QMap<QString, qreal> > levelValues() const;
    void setLevelValues(const QMap<QString, QMap<QString, qreal> > &value);

protected:
    QMap<QString, QMap<QString, qreal>> mapLevelValues;
};

Q_DECLARE_METATYPE(CVariableBalanceInfo)

QDataStream& operator<<(QDataStream& out, const CVariableBalanceInfo& variableBalanceInfo);
QDataStream& operator>>(QDataStream& in, CVariableBalanceInfo& variableBalanceInfo);
QDebug operator<<(QDebug dbg, const CVariableBalanceInfo& variableBalanceInfo);

inline uint qHash(const CVariableBalanceInfo &key, uint seed = 0)
{
    Q_UNUSED(seed)

    return qHash(key.getID());
}

#endif // CVARIABLEBALANCEINFO_H
