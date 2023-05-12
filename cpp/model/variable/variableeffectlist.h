#ifndef VARIABLEEFFECTLIST_H
#define VARIABLEEFFECTLIST_H

#include "variableeffect.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

class CVariableEffectList : public QList<CVariableEffect>, public CPropertiesInterface
{
public:
    CVariableEffectList();
    ~CVariableEffectList();
    CVariableEffectList(const CVariableEffectList& lstVariables);
    CVariableEffectList& operator=(const CVariableEffectList& lstVariables);
    virtual bool operator==(const CVariableEffectList& lstVariables);
    virtual bool operator<(const CVariableEffectList& lstVariables) const;
    friend QDataStream& operator>>(QDataStream& in, CVariableEffectList& lstVariables);

    void append(const CVariableEffect& variable);
    void append(const CVariableEffectList& lstVariables);

    void replace(int iIndex, const CVariableEffect& variable);
    void replace(const CVariableEffect& variable);
    void replace(const CVariableEffectList& lstVariables);

    void update(const CVariableEffectList& lstVariables, bool bAppendNewVariable = true);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObject);

    QStringList variablesNames() const;

    CVariableEffect byName(const QString &strName) const;
    int indexByName(const QString &strName) const;

protected:

};

Q_DECLARE_METATYPE(CVariableEffectList)

QDataStream& operator<<(QDataStream& out, const CVariableEffectList& lstVariables);
QDataStream& operator>>(QDataStream& in, CVariableEffectList& lstVariables);

#endif // VARIABLEEFFECTLIST_H
