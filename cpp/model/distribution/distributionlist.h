#ifndef CDISTRIBUTIONLIST_H
#define CDISTRIBUTIONLIST_H

#include "distribution.h"
#include "propertiesinterface.h"

#include <QList>
#include <QMutex>
#include <QMetaType>

class CDistributionsList : public QList<CDistribution>, public CPropertiesInterface
{
public:
    CDistributionsList();
    ~CDistributionsList();
    CDistributionsList(const CDistributionsList& lstDistributions);
    CDistributionsList& operator=(const CDistributionsList& lstDistributions);
    virtual bool operator==(const CDistributionsList& lstDistributions);
    virtual bool operator<(const CDistributionsList& lstDistributions) const;
    friend QDataStream& operator>>(QDataStream& in, CDistributionsList& lstDistributions);

    void append(const CDistribution& distribution);
    void append(const CDistributionsList& lstDistributions);

    void replace(int iIndex, const CDistribution& distribution);
    void replace(const CDistribution& variable);
    void replace(const CDistributionsList& lstDistributions);

    void update(const CDistributionsList& lstDistributions, bool bAppendNewDistribution = true);

    QJsonObject toJsonObject(bool bIncludeProperties = true) const;
    void fromJsonArray(const QJsonArray &jsonArray);
    void fromJsonObject(const QJsonObject &jsonObject);

protected:

};

Q_DECLARE_METATYPE(CDistributionsList)

QDataStream& operator<<(QDataStream& out, const CDistributionsList& lstDistributions);
QDataStream& operator>>(QDataStream& in, CDistributionsList& lstDistributions);

#endif // CDISTRIBUTIONLIST_H
