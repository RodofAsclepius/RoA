#ifndef ADJUSTMENTSETLIST_H
#define ADJUSTMENTSETLIST_H

#include "adjustmentset.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

class CAdjustmentSetList : public QList<CAdjustmentSet>, public CPropertiesInterface
{
public:
    CAdjustmentSetList();
    ~CAdjustmentSetList();
    CAdjustmentSetList(const CAdjustmentSetList& lstAdjustmentSets);
    CAdjustmentSetList& operator=(const CAdjustmentSetList& lstAdjustmentSets);
    virtual bool operator==(const CAdjustmentSetList& lstAdjustmentSets);
    virtual bool operator<(const CAdjustmentSetList& lstAdjustmentSets) const;
    friend QDataStream& operator>>(QDataStream& in, CAdjustmentSetList& lstAdjustmentSets);

    void append(const CAdjustmentSet& adjustmentSet);
    void append(const CAdjustmentSetList& lstAdjustmentSets);

    void replace(int iIndex, const CAdjustmentSet& adjustmentSet);
    void replace(const CAdjustmentSet& adjustmentSet);
    void replace(const CAdjustmentSetList& lstAdjustmentSets);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObject);

protected:

};

Q_DECLARE_METATYPE(CAdjustmentSetList)

QDataStream& operator<<(QDataStream& out, const CAdjustmentSetList& lstAdjustmentSets);
QDataStream& operator>>(QDataStream& in, CAdjustmentSetList& lstAdjustmentSets);

#endif // ADJUSTMENTSETLIST_H
