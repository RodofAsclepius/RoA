#ifndef EFFECTSMODELSLIST_H
#define EFFECTSMODELSLIST_H

#include "effectsmodel.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

class CEffectsModelsList : public QList<CEffectsModel>, public CPropertiesInterface
{
public:
    CEffectsModelsList();
    ~CEffectsModelsList();
    CEffectsModelsList(const CEffectsModelsList& lstEffectsmodels);
    CEffectsModelsList& operator=(const CEffectsModelsList& lstEffectsModels);
    virtual bool operator==(const CEffectsModelsList& lstEffectsModels);
    virtual bool operator<(const CEffectsModelsList& lstEffectsModels) const;
    friend QDataStream& operator>>(QDataStream& in, CEffectsModelsList& lstEffectsModels);

    void append(const CEffectsModel& effectsModel);
    void append(const CEffectsModelsList& lstEffectsModels);

    void replace(int iIndex, const CEffectsModel& effectsModel);
    void replace(const CEffectsModel& effectsModel);
    void replace(const CEffectsModelsList& lstEffectsModels);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObject);

protected:

};

Q_DECLARE_METATYPE(CEffectsModelsList)

QDataStream& operator<<(QDataStream& out, const CEffectsModelsList& lstEffectsModels);
QDataStream& operator>>(QDataStream& in, CEffectsModelsList& lstEffectsModels);

#endif // EFFECTSMODELSLIST_H
