#ifndef VARIABLEEFFECT_H
#define VARIABLEEFFECT_H

#include "variable.h"
#include "variableeffectinfo.h"

class CVariableEffect : public CVariable
{
public:
    CVariableEffect();
    CVariableEffect(const CVariableEffect& variableEffect);
    virtual ~CVariableEffect();

    CVariableEffect& operator=(const CVariableEffect& variableEffect);
    virtual bool operator==(const CVariableEffect& variableEffect) const;
    virtual bool operator<(const CVariableEffect& variableEffect) const;
    friend QDataStream& operator>>(QDataStream& in, CVariableEffect& variableEffect);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonObject(const QJsonObject& jsonObject);

    friend uint qHash(const CVariable &key, uint seed);

    CVariableEffectInfo effectInfo() const;
    void setEffectInfo(const CVariableEffectInfo &effectInfo);

protected:
    CVariableEffectInfo effectInfo_;
};

Q_DECLARE_METATYPE(CVariableEffect)

QDataStream& operator<<(QDataStream& out, const CVariableEffect& variableEffect);
QDataStream& operator>>(QDataStream& in, CVariableEffect& variableEffect);
QDebug operator<<(QDebug dbg, const CVariableEffect& variableEffect);

inline uint qHash(const CVariableEffect &key, uint seed = 0)
{
    Q_UNUSED(seed)

    return qHash(key.getID());
}

#endif // VARIABLEEFFECT_H
