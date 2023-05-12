#ifndef EFFECTSMODEL_H
#define EFFECTSMODEL_H

#include "propertiesinterface.h"
#include "balancemodel.h"
#include "effect.h"

class CEffectsModel : public CPropertiesInterface
{
    Q_OBJECT
public:
    CEffectsModel();
    CEffectsModel(const CEffectsModel &effectsModel);
    virtual ~CEffectsModel();

    CEffectsModel& operator=(const CEffectsModel &effectsModel);
    virtual bool operator==(const CEffectsModel &effectsModel) const;
    virtual bool operator<(const CEffectsModel &effectsModel) const;
    friend QDataStream& operator>>(QDataStream& in, CEffectsModel &effectsModel);

    void fromJsonObject(const QJsonObject& jsonObject);
    QJsonObject toJsonObject(bool bIncludeProperties = true) const;

    CBalanceModel balanceModel() const;
    void setBalanceModel(const CBalanceModel &balanceModel);

    CTreatment treatment() const;
    CGroup group() const;

    CDistribution distributionU() const;
    void setDistributionU(const CDistribution &distributionU);

    CDistribution distributionT() const;
    void setDistributionT(const CDistribution &distributionT);

    CEffect effect() const;
    void setEffect(const CEffect &effect);

protected:
    CEffect effect_;
    CBalanceModel balanceModel_;
    CDistribution distributionU_;
    CDistribution distributionT_;
};

Q_DECLARE_METATYPE(CEffectsModel)

QDataStream& operator<<(QDataStream& out, const CEffectsModel& applicationData);
QDataStream& operator>>(QDataStream& in, CEffectsModel& applicationData);
QDebug operator<<(QDebug dbg, const CEffectsModel& applicationData);

#endif // EFFECTSMODEL_H
