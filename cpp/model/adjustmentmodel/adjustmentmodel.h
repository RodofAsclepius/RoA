#ifndef ADJUSTMENTMODEL_H
#define ADJUSTMENTMODEL_H

#include "propertiesinterface.h"
#include "causalmodel.h"
#include "adjustmentsetlist.h"

class CAdjustmentModel : public CPropertiesInterface
{
    Q_OBJECT
public:
    CAdjustmentModel();
    CAdjustmentModel(const CAdjustmentModel& adjustmentModel);
    virtual ~CAdjustmentModel();

    CAdjustmentModel& operator=(const CAdjustmentModel& adjustmentModel);
    virtual bool operator==(const CAdjustmentModel& adjustmentModel) const;
    virtual bool operator<(const CAdjustmentModel& adjustmentModel) const;
    friend QDataStream& operator>>(QDataStream& in, CAdjustmentModel& adjustmentModel);

    void fromJsonObject(const QJsonObject& jsonObject);
    QJsonObject toJsonObject(bool bIncludeProperties = true) const;

    CCausalModel causalModel() const;
    void setCausalModel(const CCausalModel &causalModel);

    CAdjustmentSetList adjustmentSets() const;
    void setAdjustmentSets(const CAdjustmentSetList &adjustmentSets);

    QStringList paths() const;
    void setPaths(const QStringList &paths);

    QVariantList openPaths() const;
    void setOpenPaths(const QVariantList &openPaths);

protected:
    CCausalModel causalModel_;
    CAdjustmentSetList lstAdjustmentSets_;
    QStringList lstPaths_;
    QVariantList lstOpenPaths_;
};

Q_DECLARE_METATYPE(CAdjustmentModel)

QDataStream& operator<<(QDataStream& out, const CAdjustmentModel& adjustmentModel);
QDataStream& operator>>(QDataStream& in, CAdjustmentModel& adjustmentModel);
QDebug operator<<(QDebug dbg, const CAdjustmentModel& adjustmentModel);

#endif // ADJUSTMENTMODEL_H

