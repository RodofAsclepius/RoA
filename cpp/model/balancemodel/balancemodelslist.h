#ifndef CBALANCEMODELSLIST_H
#define CBALANCEMODELSLIST_H


#include "balancemodel.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

class CBalanceModelsList : public QList<CBalanceModel>, public CPropertiesInterface
{
public:
    CBalanceModelsList();
    ~CBalanceModelsList();
    CBalanceModelsList(const CBalanceModelsList& lstBalanceModels);
    CBalanceModelsList& operator=(const CBalanceModelsList& lstBalanceModels);
    virtual bool operator==(const CBalanceModelsList& lstBalanceModels);
    virtual bool operator<(const CBalanceModelsList& lstBalanceModels) const;
    friend QDataStream& operator>>(QDataStream& in, CBalanceModelsList& lstBalanceModels);

    void append(const CBalanceModel& balanceModel);
    void append(const CBalanceModelsList& lstBalanceModels);

    void replace(int iIndex, const CBalanceModel& balanceModel);
    void replace(const CBalanceModel& balanceModel);
    void replace(const CBalanceModelsList& lstBalanceModels);

//    void update(const CBalanceModelsList& lstBalanceModels, bool bAppendNewVariable = true);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObject);

protected:

};

Q_DECLARE_METATYPE(CBalanceModelsList)

QDataStream& operator<<(QDataStream& out, const CBalanceModelsList& lstBalanceModels);
QDataStream& operator>>(QDataStream& in, CBalanceModelsList& lstBalanceModels);

#endif // CBALANCEMODELSLIST_H
