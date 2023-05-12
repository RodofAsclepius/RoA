#ifndef BALANCEMODEL_H
#define BALANCEMODEL_H

#include "propertiesinterface.h"
#include "group.h"
#include "treatment.h"
#include "variableslist.h"
#include "variablebalancelist.h"
#include "distributionlist.h"

class CBalanceModel : public CPropertiesInterface
{
    Q_OBJECT
public:
    CBalanceModel();
    CBalanceModel(const CGroup &group, const CTreatment &treatment);
    CBalanceModel(const CBalanceModel& balanceModel);
    virtual ~CBalanceModel();

    CBalanceModel& operator=(const CBalanceModel& balanceModel);
    virtual bool operator==(const CBalanceModel& balanceModel) const;
    virtual bool operator<(const CBalanceModel& balanceModel) const;
    friend QDataStream& operator>>(QDataStream& in, CBalanceModel& balanceModel);

    void fromJsonObject(const QJsonObject& jsonObject);
    QJsonObject toJsonObject(bool bIncludeProperties = true) const;

    CVariablesList variables() const;
    void setVariables(const CVariablesList &variables);

    CVariableBalanceList variablesBalanced() const;
    void setVariablesBalanced(const CVariableBalanceList &variablesBalanced);

    CGroup group() const;
    void setGroup(const CGroup &group);

    CTreatment treatment() const;
    void setTreatment(const CTreatment &treatment);

    CVariablesList variablesBaseline() const;
    void setVariablesBaseline(const CVariablesList &variablesBaseline);

    CDistributionsList distributionsTreatment() const;
    void setDistributionsTreatment(const CDistributionsList &distributionsTreatment);

    CDistributionsList distributionsControl() const;
    void setDistributionsControl(const CDistributionsList &distributionsControl);

    void initDistributions();

    bool needsRefresh() const;
    void setNeedsRefresh(bool needsRefresh);

protected:
    CGroup group_;
    CTreatment treatment_;
    CVariablesList lstVariables_;
    CVariableBalanceList lstVariablesBalanced_;
    CVariablesList lstVariablesBaseline_;
    CDistributionsList lstDistributionsControl_;
    CDistributionsList lstDistributionsTreatment_;

    bool bNeedsRefresh_;
};

Q_DECLARE_METATYPE(CBalanceModel)

QDataStream& operator<<(QDataStream& out, const CBalanceModel& applicationData);
QDataStream& operator>>(QDataStream& in, CBalanceModel& applicationData);
QDebug operator<<(QDebug dbg, const CBalanceModel& applicationData);

#endif // BALANCEMODEL_H
