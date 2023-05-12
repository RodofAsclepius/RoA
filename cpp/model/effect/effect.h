#ifndef EFFECT_H
#define EFFECT_H

#include "propertiesinterface.h"
#include "variable.h"

class CEffect : public CPropertiesInterface
{
    Q_OBJECT
public:
    CEffect();
    CEffect(const CEffect& effect);
    CEffect(const CVariable &variable);
    virtual ~CEffect();

    CEffect& operator=(const CEffect& effect);
    virtual bool operator==(const CEffect& effect) const;
    virtual bool operator<(const CEffect& effect) const;
    friend QDataStream& operator>>(QDataStream& in, CEffect& effect);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonObject(const QJsonObject& jsonObject);

    CVariable variable() const;
    void setVariable(const CVariable &variable);

    bool isValid() const;
    void setIsValid(bool isValid);

protected:
    CVariable variable_;
    bool bIsValid_;

    friend uint qHash(const CVariable &key, uint seed);
};

Q_DECLARE_METATYPE(CEffect)

QDataStream& operator<<(QDataStream& out, const CEffect& effect);
QDataStream& operator>>(QDataStream& in, CEffect& effect);
QDebug operator<<(QDebug dbg, const CEffect& effect);

inline uint qHash(const CEffect &key, uint seed = 0)
{
    Q_UNUSED(seed)

    return qHash(key.getID());// * key.getName().length();
}

#endif // EFFECT_H
