#ifndef ADJUSTMENTMODELLIST_H
#define ADJUSTMENTMODELLIST_H

#include "adjustmentmodel.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

class CAdjustmentModelList : public QList<CAdjustmentModel>, public CPropertiesInterface
{
public:
    CAdjustmentModelList();
    ~CAdjustmentModelList();
    CAdjustmentModelList(const CAdjustmentModelList& lstAdjustmentModels);
    CAdjustmentModelList& operator=(const CAdjustmentModelList& lstAdjustmentModels);
    virtual bool operator==(const CAdjustmentModelList& lstAdjustmentModels);
    virtual bool operator<(const CAdjustmentModelList& lstAdjustmentModels) const;
    friend QDataStream& operator>>(QDataStream& in, CAdjustmentModelList& lstAdjustmentModels);

    void append(const CAdjustmentModel& adjustmentModel);
    void append(const CAdjustmentModelList& lstAdjustmentModels);

    void replace(int iIndex, const CAdjustmentModel& adjustmentModel);
    void replace(const CAdjustmentModel& adjustmentModel);
    void replace(const CAdjustmentModelList& lstAdjustmentModels);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObject);

protected:

};

Q_DECLARE_METATYPE(CAdjustmentModelList)

QDataStream& operator<<(QDataStream& out, const CAdjustmentModelList& lstAdjustmentModels);
QDataStream& operator>>(QDataStream& in, CAdjustmentModelList& lstAdjustmentModels);

#endif // ADJUSTMENTMODELLIST_H
