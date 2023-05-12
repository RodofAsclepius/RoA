#ifndef ADJUSTMENTSET_H
#define ADJUSTMENTSET_H

#include "propertiesinterface.h"
#include "group.h"
#include "treatment.h"
#include "effect.h"
#include "variableslist.h"

class CAdjustmentSet : public CPropertiesInterface
{
    Q_OBJECT
public:
    CAdjustmentSet();
    CAdjustmentSet(const CAdjustmentSet& adjustmentSet);
    virtual ~CAdjustmentSet();

    CAdjustmentSet& operator=(const CAdjustmentSet& adjustmentSet);
    virtual bool operator==(const CAdjustmentSet& adjustmentSet) const;
    virtual bool operator<(const CAdjustmentSet& adjustmentSet) const;
    friend QDataStream& operator>>(QDataStream& in, CAdjustmentSet& adjustmentSet);

    void fromJsonObject(const QJsonObject& jsonObject);
    QJsonObject toJsonObject(bool bIncludeProperties = true) const;

    CGroup group() const;
    void setGroup(const CGroup &group);

    CTreatment treatment() const;
    void setTreatment(const CTreatment &treatment);

    CEffect effect() const;
    void setEffect(const CEffect &effect);

    CVariablesList variables() const;
    void setVariables(const CVariablesList &variables);

    QStringList nodesNames() const;
    void setNodesNames(const QStringList &nodesNames);

protected:
    CGroup group_;
    CTreatment treatment_;
    CEffect effect_;
    QStringList lstNodesNames_;
    CVariablesList lstVariables_;
};

Q_DECLARE_METATYPE(CAdjustmentSet)

QDataStream& operator<<(QDataStream& out, const CAdjustmentSet& adjustmentSet);
QDataStream& operator>>(QDataStream& in, CAdjustmentSet& adjustmentSet);
QDebug operator<<(QDebug dbg, const CAdjustmentSet& adjustmentSet);

#endif // ADJUSTMENTSET_H

