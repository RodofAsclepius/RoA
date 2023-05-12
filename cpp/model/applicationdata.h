#ifndef APPLICATIONDATA_H
#define APPLICATIONDATA_H

#include "propertiesinterface.h"
#include "variableslist.h"
#include "groupslist.h"
#include "treatment.h"
#include "balancemodelslist.h"
#include "variableeffectlist.h"
#include "effectsmodelslist.h"
#include "causalmodellist.h"
#include "causalmodelcachelist.h"
#include "adjustmentmodellist.h"

#include <QString>
#include <QMutex>
#include <QMetaType>
#include <QList>
#include <QDataStream>
#include <QMap>
#include <QVariant>

class CRController;

class CApplicationData : public CPropertiesInterface
{
    Q_OBJECT
public:
    CApplicationData();
    CApplicationData(const CApplicationData& applicationData);
    virtual ~CApplicationData();

    CApplicationData& operator=(const CApplicationData& applicationData);
    virtual bool operator==(const CApplicationData& applicationData) const;
    virtual bool operator<(const CApplicationData& applicationData) const;
    friend QDataStream& operator>>(QDataStream& in, CApplicationData& applicationData);

    CVariablesList variables() const;
    void setVariables(const CVariablesList& lstVariables);

    CGroupsList groups() const;
    void setGroups(const CGroupsList &groups);

    CTreatment treatment() const;
    void setTreatment(const CTreatment &treatment);

    CEffect effect() const;
    void setEffect(const CEffect &effect);

    CDistributionsList distributions() const;
    void setDistributions(const CDistributionsList &lstDistributions);

    CCausalModelList causalModels() const;
    void setCausalModels(const CCausalModelList &causalModels);
    void addBalanceModel(const CCausalModel &causalModel);
    void clearCausalModels();

    CCausalModelCacheList causalModelCaches() const;
    void setCausalModelCaches(const CCausalModelCacheList &lstCausalModelCaches);
    void addCausalModelCache(const CCausalModelCache &causalModelCache);
    void clearCausalModelCaches();

    CBalanceModelsList balanceModels() const;
    void setBalanceModels(const CBalanceModelsList &lstBalanceModels);
    void clearBalanceModels();
    void addBalanceModel(const CBalanceModel &balanceModel);

    CEffectsModelsList effectModels() const;
    void setEffectModels(const CEffectsModelsList &effectModels);
    void clearEffectModels();
    void addEffectModel(const CEffectsModel &model);

    CAdjustmentModelList adjustmentModels() const;
    void setAdjustmentModels(const CAdjustmentModelList &adjustmentModels);

    QStringList latestUpdates() const;
    void setLatestUpdates(const QStringList &latestUpdates);
    void clearLatestUpdates();
    void registerUpdate(QString strUpdate, bool bClearPrevious = false);
    void removeUpdate(QString strUpdate);
    bool checkUpdate(QString strUpdate);

    struct Updates {
        static const QString All;
        static const QString Variables;
        static const QString Groups;
        static const QString GroupsSizes;
        static const QString Treatment;
        static const QString Effect;
        static const QString Distributions;
        static const QString BalanceModels;
        static const QString EffectsModels;
        static const QString CausalModels;
        static const QString CausalModelCaches;
        static const QString AdjustmentModels;
    };

protected:
    /* Variables, groups, treatments */
    CVariablesList lstVariables_;
    CGroupsList lstGroups_;
    CTreatment treatment_;
    CEffect effect_;
    CDistributionsList lstDistributions_;

    /* Balance, effects and causal models */
    CCausalModelList lstCausalModels_;
    CCausalModelCacheList lstCausalModelCaches_;
    CBalanceModelsList lstBalanceModels_;
    CEffectsModelsList lstEffectModels_;
    CAdjustmentModelList lstAdjustmentModels_;

    /* State */
    QStringList lstLatestUpdates_;

    friend QDataStream& operator<<(QDataStream& out, const CApplicationData& applicationData);
    friend uint qHash(const CApplicationData &key, uint seed);
};

QDataStream& operator<<(QDataStream& out, const CApplicationData& applicationData);
QDataStream& operator>>(QDataStream& in, CApplicationData& applicationData);
QDebug operator<<(QDebug dbg, const CApplicationData& applicationData);

inline uint qHash(const CApplicationData &key, uint seed = 0)
{
    Q_UNUSED(seed)

    return qHash(key.getID());
}

#endif // APPLICATIONDATA_H
