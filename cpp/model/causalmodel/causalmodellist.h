#ifndef CAUSALMODELLIST_H
#define CAUSALMODELLIST_H


#include "causalmodel.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

class CCausalModelList : public QList<CCausalModel>, public CPropertiesInterface
{
public:
    CCausalModelList();
    ~CCausalModelList();
    CCausalModelList(const CCausalModelList& lstCausalModels);
    CCausalModelList& operator=(const CCausalModelList& lstCausalModels);
    virtual bool operator==(const CCausalModelList& lstCausalModels);
    virtual bool operator<(const CCausalModelList& lstCausalModels) const;
    friend QDataStream& operator>>(QDataStream& in, CCausalModelList& lstCausalModels);

    void append(const CCausalModel& causalModel);
    void append(const CCausalModelList& lstCausalModels);

    void replace(int iIndex, const CCausalModel& causalModel);
    void replace(const CCausalModel& causalModel);
    void replace(const CCausalModelList& lstCausalModels);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObject);

protected:

};

Q_DECLARE_METATYPE(CCausalModelList)

QDataStream& operator<<(QDataStream& out, const CCausalModelList& lstCausalModels);
QDataStream& operator>>(QDataStream& in, CCausalModelList& lstCausalModels);

#endif // CAUSALMODELLIST_H
