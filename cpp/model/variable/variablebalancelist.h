#ifndef VARIABLEBALANCELIST_H
#define VARIABLEBALANCELIST_H


#include "variablebalance.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

//static inline QString variableListMimeType() { return QStringLiteral("application/x-variablelist"); }

class CVariableBalanceList : public QList<CVariableBalance>, public CPropertiesInterface
{
public:
    CVariableBalanceList();
    ~CVariableBalanceList();
    CVariableBalanceList(const CVariableBalanceList& lstVariables);
    CVariableBalanceList& operator=(const CVariableBalanceList& lstVariables);
    virtual bool operator==(const CVariableBalanceList& lstVariables);
    virtual bool operator<(const CVariableBalanceList& lstVariables) const;
    friend QDataStream& operator>>(QDataStream& in, CVariableBalanceList& lstVariables);

    void append(const CVariableBalance& variable);
    void append(const CVariableBalanceList& lstVariables);

    void replace(int iIndex, const CVariableBalance& variable);
    void replace(const CVariableBalance& variable);
    void replace(const CVariableBalanceList& lstVariables);

    void update(const CVariableBalanceList& lstVariables, bool bAppendNewVariable = true);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObject);

    QStringList variablesNames() const;

    CVariableBalance byName(const QString &strName) const;
    int indexByName(const QString &strName) const;

protected:

};

Q_DECLARE_METATYPE(CVariableBalanceList)

QDataStream& operator<<(QDataStream& out, const CVariableBalanceList& lstVariables);
QDataStream& operator>>(QDataStream& in, CVariableBalanceList& lstVariables);

#endif // VARIABLEBALANCELIST_H
