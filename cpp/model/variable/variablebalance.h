#ifndef VARIABLEBALANCE_H
#define VARIABLEBALANCE_H

#include "variable.h"
#include "variablebalanceinfo.h"

class CVariableBalance : public CVariable
{
public:
    CVariableBalance();
    CVariableBalance(const CVariableBalance& variableBalance);
    virtual ~CVariableBalance();

    CVariableBalance& operator=(const CVariableBalance& variableBalance);
    virtual bool operator==(const CVariableBalance& variableBalance) const;
    virtual bool operator<(const CVariableBalance& variableBalance) const;
    friend QDataStream& operator>>(QDataStream& in, CVariableBalance& variableBalance);

    CVariableBalanceInfo balanceInfo() const;
    void setBalanceInfo(const CVariableBalanceInfo &balanceInfo);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonObject(const QJsonObject& jsonObject);

    friend uint qHash(const CVariable &key, uint seed);

protected:
    CVariableBalanceInfo balanceInfo_;
};

Q_DECLARE_METATYPE(CVariableBalance)

QDataStream& operator<<(QDataStream& out, const CVariableBalance& variableBalance);
QDataStream& operator>>(QDataStream& in, CVariableBalance& variableBalance);
QDebug operator<<(QDebug dbg, const CVariableBalance& variableBalance);

inline uint qHash(const CVariableBalance &key, uint seed = 0)
{
    Q_UNUSED(seed);

    return qHash(key.getID());
}

#endif // VARIABLEBALANCE_H
