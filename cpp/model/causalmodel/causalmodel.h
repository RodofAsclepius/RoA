#ifndef CAUSALMODEL_H
#define CAUSALMODEL_H

#include "propertiesinterface.h"
#include "group.h"
#include "treatment.h"
#include "effect.h"
#include "variableslist.h"
#include "variableeffectlist.h"
#include <QVector>
#include <QLineF>
#include "graph.h"

class CCausalModel : public CPropertiesInterface
{
    Q_OBJECT
public:
    CCausalModel();
    CCausalModel(const CTreatment &treatment, const CEffect &effect);
    CCausalModel(const CCausalModel& causalModel);
    virtual ~CCausalModel();

    CCausalModel& operator=(const CCausalModel& causalModel);
    virtual bool operator==(const CCausalModel& causalModel) const;
    virtual bool operator!=(const CCausalModel& causalModel) const;
    virtual bool operator<(const CCausalModel& causalModel) const;
    friend QDataStream& operator>>(QDataStream& in, CCausalModel& causalModel);

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

    CGraph graphUser() const;
    void setGraphUser(const CGraph &graphUser);

    CGraph graphAlg() const;
    void setGraphAlg(const CGraph &graphAlg);

    void initGraphs();

protected:
    CGroup group_;
    CTreatment treatment_;
    CEffect effect_;
    CVariablesList lstVariables_;

    CGraph graphUser_;
    CGraph graphAlg_;

    void initGraphUser();
    void initGraphAlg();
};

Q_DECLARE_METATYPE(CCausalModel)

QDataStream& operator<<(QDataStream& out, const CCausalModel& causalModel);
QDataStream& operator>>(QDataStream& in, CCausalModel& causalModel);
QDebug operator<<(QDebug dbg, const CCausalModel& causalModel);

#endif // CAUSALMODEL_H


