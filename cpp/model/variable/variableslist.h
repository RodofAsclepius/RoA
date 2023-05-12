#ifndef CVARIABLELIST_H
#define CVARIABLELIST_H


#include "variable.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

//static inline QString variableListMimeType() { return QStringLiteral("application/x-variablelist"); }

class CVariablesList : public QList<CVariable>, public CPropertiesInterface
{
public:
    CVariablesList();
    ~CVariablesList();
    CVariablesList(const CVariablesList& lstVariables);
    CVariablesList& operator=(const CVariablesList& lstVariables);
    virtual bool operator==(const CVariablesList& lstVariables);
    virtual bool operator<(const CVariablesList& lstVariables) const;
    friend QDataStream& operator>>(QDataStream& in, CVariablesList& lstVariables);

    void append(const CVariable& variable);
    void append(const CVariablesList& lstVariables);

    void replace(int iIndex, const CVariable& variable);
    void replace(const CVariable& variable);
    void replace(const CVariablesList& lstVariables);

    void update(const CVariablesList& lstVariables, bool bAppendNewVariable = true);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObject);

    QStringList variablesNames() const;

    CVariable byName(const QString &strName) const;
    int indexByName(const QString &strName) const;

protected:

};

Q_DECLARE_METATYPE(CVariablesList)

QDataStream& operator<<(QDataStream& out, const CVariablesList& lstVariables);
QDataStream& operator>>(QDataStream& in, CVariablesList& lstVariables);

bool VariableNameLessThan(const CVariable &v1, const CVariable &v2);

#endif // CVARIABLELIST_H
