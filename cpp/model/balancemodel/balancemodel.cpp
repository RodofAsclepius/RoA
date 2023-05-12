#include "balancemodel.h"
#include "utils.h"

using namespace utils;

CBalanceModel::CBalanceModel()
    : CPropertiesInterface()
{
    qRegisterMetaType<CBalanceModel>("CBalanceModel");
//    qRegisterMetaTypeStreamOperators<CBalanceModel>("CBalanceModel");
}

CBalanceModel::CBalanceModel(const CGroup &group, const CTreatment &treatment)
{
    setGroup(group);
    setTreatment(treatment);
}

CBalanceModel::CBalanceModel(const CBalanceModel& balanceModel)
    : CBalanceModel()
{
    this->operator =(balanceModel);
}

CBalanceModel::~CBalanceModel()
{
}

CBalanceModel& CBalanceModel::operator=(const CBalanceModel& balanceModel)
{
    if (this != &balanceModel)
    {
        setID(balanceModel.getID());
        setProperties(balanceModel.getProperties());
        setGroup(balanceModel.group());
        setTreatment(balanceModel.treatment());
        setVariables(balanceModel.variables());
        setVariablesBalanced(balanceModel.variablesBalanced());
        setVariablesBaseline(balanceModel.variablesBaseline());
        setDistributionsTreatment(balanceModel.distributionsTreatment());
        setDistributionsControl(balanceModel.distributionsControl());
        setNeedsRefresh(balanceModel.needsRefresh());
    }
    return *this;
}

bool CBalanceModel::operator==(const CBalanceModel& balanceModel) const
{
    return (getID() == balanceModel.getID());
}

bool CBalanceModel::operator<(const CBalanceModel& balanceModel) const
{
    return (getID() < balanceModel.getID());
}

void CBalanceModel::fromJsonObject(const QJsonObject &jsonObject)
{
//    DEBUG_OBJ_INFO

//    qDebug() << jsonObject;

    QString strID = jsonObject["id"].toString();
    bool ok;
    quint32 lID = strID.toUInt(&ok, 10);
    if(lID != 0)
    {
        setID(lID);
    }

    /* Group */
    CGroup group;
    QJsonValue valGroup = jsonObject["group"];
    if(valGroup.isObject())
    {
        QJsonObject objGroup = valGroup.toObject();
        group.fromJsonObject(objGroup);
        setGroup(group);
    }

    /* Treatment */
    CTreatment treatment;
    QJsonValue valTreatment = jsonObject["treatment"];
    if(valTreatment.isObject())
    {
        QJsonObject objTreatment = valTreatment.toObject();
        treatment.fromJsonObject(objTreatment);
        setTreatment(treatment);
    }

    /* Variables */
    CVariablesList lstVariables;
    QJsonValue valVariables = jsonObject["variables"];
    if(valVariables.isObject())
    {
        lstVariables.fromJsonObject(valVariables.toObject());
        setVariables(lstVariables);
    }

    /* Variables balance */
    CVariableBalanceList lstVariablesBalanced;
    QJsonValue valVariablesBalanced = jsonObject["variablesBalanced"];
    if(valVariablesBalanced.isArray())
    {
        lstVariablesBalanced.fromJsonArray(valVariablesBalanced.toArray());
        setVariablesBalanced(lstVariablesBalanced);
    }

    /* Variables baseline */
    CVariablesList lstVariablesBaseline;
    QJsonValue valVariablesBaseline = jsonObject["variablesBaseline"];
    if(valVariablesBaseline.isObject())
    {
        lstVariablesBaseline.fromJsonObject(valVariablesBaseline.toObject());
        setVariablesBaseline(lstVariablesBaseline);
    }

    /* Weighted distributions */
    CDistributionsList lstDistributionsWeighted;
    QJsonValue valDistributionsWeighted = jsonObject["distributionsTreated"];
    if(valDistributionsWeighted.isArray())
    {
        lstDistributionsWeighted.fromJsonArray(valDistributionsWeighted.toArray());
        setDistributionsTreatment(lstDistributionsWeighted);
    }

    /* Weighted distributions */
    CDistributionsList lstDistributions;
    QJsonValue valDistributions = jsonObject["distributionsControl"];

    if(valDistributions.isArray())
    {
        lstDistributions.fromJsonArray(valDistributions.toArray());
        setDistributionsControl(lstDistributions);
    }

    /* Needs refresh */
    bool bNeedsRefresh;
    QJsonValue valNeedsRefresh = jsonObject["needsRefresh"];
    bNeedsRefresh = valNeedsRefresh.toBool();
    setNeedsRefresh(bNeedsRefresh);
}

QJsonObject CBalanceModel::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObject;

    jsonObject["id"] = QString::number(getID());
    jsonObject["group"] = group().toJsonObject();
    jsonObject["treatment"] = treatment().toJsonObject();
    jsonObject["variables"] = variables().toJsonObject();
    jsonObject["variablesBalanced"] = variablesBalanced().toJsonObject();
    jsonObject["variablesBaseline"] = variablesBaseline().toJsonObject();
    jsonObject["distributionsTreated"] = distributionsTreatment().toJsonObject();
    jsonObject["distributionsControl"] = distributionsControl().toJsonObject();
    jsonObject["needsRefresh"] = needsRefresh();

    if(bIncludeProperties)
    {
        jsonObject["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObject;
}

CVariablesList CBalanceModel::variables() const
{
    return lstVariables_;
}

void CBalanceModel::setVariables(const CVariablesList &lstVariables)
{
    lstVariables_ = lstVariables;
}

CVariableBalanceList CBalanceModel::variablesBalanced() const
{
    return lstVariablesBalanced_;
}

void CBalanceModel::setVariablesBalanced(const CVariableBalanceList &lstVariablesBalance)
{
    lstVariablesBalanced_ = lstVariablesBalance;
}

CGroup CBalanceModel::group() const
{
    return group_;
}

void CBalanceModel::setGroup(const CGroup &group)
{
    group_ = group;
}

CTreatment CBalanceModel::treatment() const
{
    return treatment_;
}

void CBalanceModel::setTreatment(const CTreatment &treatment)
{
    treatment_ = treatment;
}

CVariablesList CBalanceModel::variablesBaseline() const
{
    return lstVariablesBaseline_;
}

void CBalanceModel::setVariablesBaseline(const CVariablesList &lstVariablesBaseline)
{
    lstVariablesBaseline_ = lstVariablesBaseline;
}

CDistributionsList CBalanceModel::distributionsTreatment() const
{
    return lstDistributionsTreatment_;
}

void CBalanceModel::setDistributionsTreatment(const CDistributionsList &lstDistributionsWeighted)
{
    lstDistributionsTreatment_ = lstDistributionsWeighted;
}

CDistributionsList CBalanceModel::distributionsControl() const
{
    return lstDistributionsControl_;
}

void CBalanceModel::setDistributionsControl(const CDistributionsList &lstDistributions)
{
    lstDistributionsControl_ = lstDistributions;
}

void CBalanceModel::initDistributions()
{
    /* Control */
    lstDistributionsControl_.clear();
    lstDistributionsTreatment_.clear();

    foreach(CVariable variable, lstVariables_)
    {
        CDistribution distribution(group(), variable);
        lstDistributionsControl_.append(distribution);
        lstDistributionsTreatment_.append(distribution);
    }
}

bool CBalanceModel::needsRefresh() const
{
    return bNeedsRefresh_;
}

void CBalanceModel::setNeedsRefresh(bool bNeedRefresh)
{
    bNeedsRefresh_ = bNeedRefresh;
}

QDataStream& operator<<(QDataStream& out, const CBalanceModel& balanceModel)
{
    out << balanceModel.getID()
        << balanceModel.getProperties()
        << balanceModel.group()
        << balanceModel.treatment()
        << balanceModel.variables()
        << balanceModel.variablesBalanced()
        << balanceModel.variablesBaseline()
        << balanceModel.distributionsTreatment()
        << balanceModel.distributionsControl()
        << balanceModel.needsRefresh();
    
    return out;
}

QDataStream& operator>>(QDataStream& in, CBalanceModel& balanceModel)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;
    CGroup group;
    CTreatment treatment;
    CVariablesList variables;
    CVariableBalanceList variablesBalance;
    CVariablesList variablesBaseline;
    CDistributionsList lstDistributionsTreatment;
    CDistributionsList lstDistributionsControl;
    bool bNeedsRefresh;

    in  >> lID >> mapProperties
        >> group
        >> treatment
        >> variables
        >> variablesBalance
        >> variablesBaseline
        >> lstDistributionsTreatment
        >> lstDistributionsControl
        >> bNeedsRefresh;

    CBalanceModel balanceModelNew;
    balanceModelNew.setID(lID);
    balanceModelNew.setProperties(mapProperties);
    balanceModelNew.setGroup(group);
    balanceModelNew.setTreatment(treatment);
    balanceModelNew.setVariables(variables);
    balanceModelNew.setVariablesBalanced(variablesBalance);
    balanceModelNew.setVariablesBaseline(variablesBaseline);
    balanceModelNew.setDistributionsTreatment(lstDistributionsTreatment);
    balanceModelNew.setDistributionsControl(lstDistributionsControl);
    balanceModelNew.setNeedsRefresh(bNeedsRefresh);

    balanceModel = balanceModelNew;

    return in;
}

QDebug operator<<(QDebug dbg, const CBalanceModel& balanceModel)
{
    dbg.nospace()   << g_spacer
                    << "balanceModel(" << balanceModel.getID()
                    << ")" << "\n";

    g_spacer.indent();
        dbg.nospace() << g_spacer << "Group:" << balanceModel.group() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
        dbg.nospace() << g_spacer << "Treatment:" << balanceModel.treatment() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
        dbg.nospace() << g_spacer << "Variables:" << balanceModel.variables() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
        dbg.nospace() << g_spacer << "Variables balanced:" << balanceModel.variablesBalanced() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
        dbg.nospace() << g_spacer << "Variables baseline:" << balanceModel.variablesBaseline() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
        dbg.nospace() << g_spacer << "Distributions treatment:" << balanceModel.distributionsTreatment() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
        dbg.nospace() << g_spacer << "Distributions control:" << balanceModel.distributionsControl() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(balanceModel.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}
