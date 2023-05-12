#ifndef MARKOVMODEL_H
#define MARKOVMODEL_H

#include "propertiesinterface.h"
#include "causalmodel.h"

class CMarkovModel : public CPropertiesInterface
{
    Q_OBJECT
public:
    CMarkovModel();
    CMarkovModel(const CCausalModel &causalModel, const CVariablesList &blanket);
    CMarkovModel(const CMarkovModel&  markovModel);
    virtual ~CMarkovModel();

    CMarkovModel& operator=(const CMarkovModel&  markovModel);
    virtual bool operator==(const CMarkovModel&  markovModel) const;
    virtual bool operator<(const CMarkovModel&  markovModel) const;
    friend QDataStream& operator>>(QDataStream& in, CMarkovModel&  markovModel);

    void fromJsonObject(const QJsonObject& jsonObject);
    QJsonObject toJsonObject(bool bIncludeProperties = true) const;

    CCausalModel causalModel() const;
    void setCausalModel(const CCausalModel &causalModel);

    CVariablesList markovBlanket() const;
    void setMarkovBlanket(const CVariablesList &markovBlanket);

    CVariablesList markovBlanketPlus(int iDepth = 1) const;

protected:
    CCausalModel causalModel_;
    CVariablesList lstMarkovBlanket_;
};

Q_DECLARE_METATYPE(CMarkovModel)

QDataStream& operator<<(QDataStream& out, const CMarkovModel&  markovModel);
QDataStream& operator>>(QDataStream& in, CMarkovModel&  markovModel);
QDebug operator<<(QDebug dbg, const CMarkovModel&  markovModel);

#endif // MARKOVMODEL_H


