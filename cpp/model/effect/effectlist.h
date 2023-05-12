#ifndef EFFECTLIST_H
#define EFFECTLIST_H

#include "variableeffect.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

class CEffectList : public QList<CVariableEffect>, public CPropertiesInterface
{
public:
    CEffectList();
    ~CEffectList();
    CEffectList(const CEffectList& lstEffects);
    CEffectList& operator=(const CEffectList& lstEffects);
    virtual bool operator==(const CEffectList& lstEffects);
    virtual bool operator<(const CEffectList& lstEffects) const;
    friend QDataStream& operator>>(QDataStream& in, CEffectList& lstEffects);

    void append(const CVariableEffect& variable);
    void append(const CEffectList& lstEffects);

    void replace(int iIndex, const CVariableEffect& variable);
    void replace(const CVariableEffect& variable);
    void replace(const CEffectList& lstEffects);

    void update(const CEffectList& lstEffects, bool bAppendNewVariable = true);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObject);

    QStringList variablesNames() const;

    CVariableEffect byName(const QString &strName) const;
    int indexByName(const QString &strName) const;

protected:

};

Q_DECLARE_METATYPE(CEffectList)

QDataStream& operator<<(QDataStream& out, const CEffectList& lstEffects);
QDataStream& operator>>(QDataStream& in, CEffectList& lstEffects);

#endif // EFFECTLIST_H
