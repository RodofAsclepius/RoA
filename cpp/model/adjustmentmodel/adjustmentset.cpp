#include "adjustmentset.h"
#include "utils.h"
#include "groupslist.h"

using namespace utils;

CAdjustmentSet::CAdjustmentSet()
    : CPropertiesInterface()
{
    qRegisterMetaType<CAdjustmentSet>("CAdjustmentSet");
//    qRegisterMetaTypeStreamOperators<CAdjustmentSet>("CAdjustmentSet");
}

CAdjustmentSet::CAdjustmentSet(const CAdjustmentSet& adjustmentSet)
    : CAdjustmentSet()
{
    this->operator =(adjustmentSet);
}

CAdjustmentSet::~CAdjustmentSet()
{
}

CAdjustmentSet& CAdjustmentSet::operator=(const CAdjustmentSet& adjustmentSet)
{
    if (this != &adjustmentSet)
    {
        setID(adjustmentSet.getID());
        setProperties(adjustmentSet.getProperties());
        setGroup(adjustmentSet.group());
        setTreatment(adjustmentSet.treatment());
        setEffect(adjustmentSet.effect());
        setNodesNames(adjustmentSet.nodesNames());
    }

    return *this;
}

bool CAdjustmentSet::operator==(const CAdjustmentSet& adjustmentSet) const
{
    return (getID() == adjustmentSet.getID());
}

bool CAdjustmentSet::operator<(const CAdjustmentSet& adjustmentSet) const
{
    return (getID() < adjustmentSet.getID());
}

void CAdjustmentSet::fromJsonObject(const QJsonObject &jsonObject)
{
//    DEBUG_OBJ_INFO

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

    /* Effect */
    CEffect effect;
    QJsonValue valEffect = jsonObject["effect"];
    if(valEffect.isObject())
    {
        QJsonObject objEffect = valEffect.toObject();
        effect.fromJsonObject(objEffect);
        setEffect(effect);
    }

    /* Variables */
    CVariablesList lstVariables;
    QJsonValue valVariables = jsonObject["variables"];
    if(valVariables.isObject())
    {
        lstVariables.fromJsonObject(valVariables.toObject());
        setVariables(lstVariables);
    }

    /* Node names */
    QStringList lstNodeNames;
    QJsonValue valNodeNames = jsonObject["nodeNames"];
    if(valNodeNames.isArray())
    {
        QJsonArray arNodeNames = valNodeNames.toArray();
        foreach(QJsonValue vNodeName, arNodeNames)
        {
            lstNodeNames.append(vNodeName.toString());
        }
    }
    setNodesNames(lstNodeNames);
}

QJsonObject CAdjustmentSet::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObject;

    jsonObject["id"] = QString::number(getID());
    jsonObject["group"] = group().toJsonObject();
    jsonObject["treatment"] = treatment().toJsonObject();
    jsonObject["effect"] = effect().toJsonObject();
//    jsonObject["variables"] = variables().toJsonObject();

    if(bIncludeProperties)
    {
        jsonObject["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObject;
}

CGroup CAdjustmentSet::group() const
{
    return group_;
}

void CAdjustmentSet::setGroup(const CGroup &group)
{
    group_ = group;
}

CTreatment CAdjustmentSet::treatment() const
{
    return treatment_;
}

void CAdjustmentSet::setTreatment(const CTreatment &treatmeant)
{
    treatment_ = treatmeant;
}

CVariablesList CAdjustmentSet::variables() const
{
    return lstVariables_;
}

void CAdjustmentSet::setVariables(const CVariablesList &lstVariables)
{
    lstVariables_ = lstVariables;
}

QStringList CAdjustmentSet::nodesNames() const
{
    return lstNodesNames_;
}

void CAdjustmentSet::setNodesNames(const QStringList &lstNodesNames)
{
    lstNodesNames_ = lstNodesNames;
}

CEffect CAdjustmentSet::effect() const
{
    return effect_;
}

void CAdjustmentSet::setEffect(const CEffect &effect)
{
    effect_ = effect;
}

QDataStream& operator<<(QDataStream& out, const CAdjustmentSet& adjustmentSet)
{
    out << adjustmentSet.getID()
        << adjustmentSet.getProperties()
        << adjustmentSet.group()
        << adjustmentSet.treatment()
        << adjustmentSet.effect()
        << adjustmentSet.nodesNames();

    return out;
}

QDataStream& operator>>(QDataStream& in, CAdjustmentSet& adjustmentSet)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;
    CGroup group;
    CTreatment treatment;
    CEffect effect;
    QStringList lstNodeNames;

    in >> lID >> mapProperties
            >> group
            >> treatment
            >> effect
            >> lstNodeNames;

    CAdjustmentSet adjustmentSetNew;

    adjustmentSetNew.setID(lID);
    adjustmentSetNew.setProperties(mapProperties);
    adjustmentSetNew.setGroup(group);
    adjustmentSetNew.setTreatment(treatment);
    adjustmentSetNew.setEffect(effect);
    adjustmentSetNew.setNodesNames(lstNodeNames);

    adjustmentSet = adjustmentSetNew;

    return in;
}

QDebug operator<<(QDebug dbg, const CAdjustmentSet& adjustmentSet)
{
    dbg.nospace()   << g_spacer
                    << "CadjustmentSet(" << adjustmentSet.getID()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Group:" << adjustmentSet.group() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Treatment:" << adjustmentSet.treatment() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Effect:" << adjustmentSet.effect() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Node names:" << adjustmentSet.nodesNames() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(adjustmentSet.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}
