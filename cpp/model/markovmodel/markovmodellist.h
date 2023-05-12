#ifndef MARKOVMODELLIST_H
#define MARKOVMODELLIST_H


#include "markovmodel.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

class CMarkovModelList : public QList<CMarkovModel>, public CPropertiesInterface
{
public:
    CMarkovModelList();
    ~CMarkovModelList();
    CMarkovModelList(const CMarkovModelList& lstMarkovModels);
    CMarkovModelList& operator=(const CMarkovModelList& lstMarkovModels);
    virtual bool operator==(const CMarkovModelList& lstMarkovModels);
    virtual bool operator<(const CMarkovModelList& lstMarkovModels) const;
    friend QDataStream& operator>>(QDataStream& in, CMarkovModelList& lstMarkovModels);

    void append(const CMarkovModel& markovModel);
    void append(const CMarkovModelList& lstMarkovModels);

    void replace(int iIndex, const CMarkovModel& markovModel);
    void replace(const CMarkovModel& markovModel);
    void replace(const CMarkovModelList& lstMarkovModels);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObject);

protected:

};

Q_DECLARE_METATYPE(CMarkovModelList)

QDataStream& operator<<(QDataStream& out, const CMarkovModelList& lstMarkovModels);
QDataStream& operator>>(QDataStream& in, CMarkovModelList& lstMarkovModels);

#endif // MARKOVMODELLIST_H
