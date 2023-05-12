#include "causalmodel.h"
#include "utils.h"
#include "groupslist.h"

using namespace utils;

CCausalModel::CCausalModel()
    : CPropertiesInterface()
{
    qRegisterMetaType<CCausalModel>("CCausalModel");
//    qRegisterMetaTypeStreamOperators<CCausalModel>("CCausalModel");
}

CCausalModel::CCausalModel(const CTreatment &treatment, const CEffect &effect)
    : CCausalModel()
{
    setTreatment(treatment);
    setEffect(effect);
//    add
}

CCausalModel::CCausalModel(const CCausalModel& causalModel)
    : CCausalModel()
{
    this->operator =(causalModel);
}

CCausalModel::~CCausalModel()
{
}

CCausalModel& CCausalModel::operator=(const CCausalModel& causalModel)
{
    if (this != &causalModel)
    {
        setID(causalModel.getID());
        setProperties(causalModel.getProperties());
        setGroup(causalModel.group());
        setTreatment(causalModel.treatment());
        setEffect(causalModel.effect());
        setVariables(causalModel.variables());
        setGraphUser(causalModel.graphUser());
        setGraphAlg(causalModel.graphAlg());
    }

    return *this;
}

bool CCausalModel::operator==(const CCausalModel& causalModel) const
{
    return (getID() == causalModel.getID());
}

bool CCausalModel::operator!=(const CCausalModel &causalModel) const
{
    return (getID() != causalModel.getID());
}

bool CCausalModel::operator<(const CCausalModel& causalModel) const
{
    return (getID() < causalModel.getID());
}

void CCausalModel::fromJsonObject(const QJsonObject &jsonObject)
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

    /* User graph */
    CGraph graphUser;
    QJsonValue valGraphUser = jsonObject["graphUser"];
    if(valGraphUser.isObject())
    {
        graphUser.fromJsonObject(valGraphUser.toObject());
        setGraphUser(graphUser);
    }

    /* Mined graph */
    CGraph graphAlg;
    QJsonValue valGraphAlg = jsonObject["graphAlg"];
    if(valGraphAlg.isObject())
    {
        graphAlg.fromJsonObject(valGraphAlg.toObject());
        setGraphAlg(graphAlg);
    }

    /* Store mined edges, if present override mined graph */
    QJsonValue valMinedEdges = jsonObject["minedEdges"];
    if(valMinedEdges.isObject())
    {
        initGraphAlg();
        CGraph graphAlg = graphAlg_;

        QJsonObject objMinedEdges = valMinedEdges.toObject();
        foreach(QString strKeySourceNodeName, objMinedEdges.keys())
        {
            QJsonArray arTargetNodes = objMinedEdges.value(strKeySourceNodeName).toArray();
            foreach(QJsonValue valTargetNodeName, arTargetNodes)
            {
                QString strTargetNodeName = valTargetNodeName.toString();

                /* Add edge */
                CNode nodeSource;
                CNode nodeTarget;
                if( graphAlg.nodeByLabel(strKeySourceNodeName, nodeSource) &&
                    graphAlg.nodeByLabel(strTargetNodeName, nodeTarget))
                {
                    CEdge edge(nodeSource, nodeTarget, "");
                    graphAlg.addEdge(edge);
                }
            }
        }
        setGraphAlg(graphAlg);
    }

    /* Properties */
    if(jsonObject["properties"].isObject())
    {
        QJsonObject jsonObjectProperties = jsonObject["properties"].toObject();
        foreach(QString strPropertyName, jsonObjectProperties.keys())
        {
            /* Object */
            if(jsonObjectProperties[strPropertyName].isObject())
            {
                QJsonObject jsonObjectProperty = jsonObjectProperties[strPropertyName].toObject();
                QJsonDocument jsonDoc(jsonObjectProperty);
                QString strJsonData(jsonDoc.toJson(QJsonDocument::Compact));
                addProperty(strPropertyName, QVariant::fromValue(strJsonData));
            }

            /* Array */
            if(jsonObjectProperties[strPropertyName].isArray())
            {
                QJsonArray jsonArrayProperty = jsonObjectProperties[strPropertyName].toArray();
                QJsonDocument jsonDoc(jsonArrayProperty);
                QString strJsonData(jsonDoc.toJson(QJsonDocument::Compact));
                addProperty(strPropertyName, QVariant::fromValue(strJsonData));
            }

//            qDebug() << jsonObjectProperties[strPropertyName];
            /* String */
            if(jsonObjectProperties[strPropertyName].isString())
            {
                QJsonValue jsonValue = jsonObjectProperties[strPropertyName];
//                QJsonDocument jsonDoc(jsonArrayProperty);
//                QString strJsonData(jsonDoc.toJson(QJsonDocument::Compact));

                addProperty(strPropertyName, jsonValue.toString());
            }

            /* Double */
            if(jsonObjectProperties[strPropertyName].isDouble())
            {
                QJsonValue jsonValue = jsonObjectProperties[strPropertyName];
                addProperty(strPropertyName, jsonValue.toDouble());
            }
        }
    }
}

QJsonObject CCausalModel::toJsonObject(bool bIncludeProperties) const
{
//    DEBUG_OBJ_INFO

    QJsonObject jsonObject;

    jsonObject["id"] = QString::number(getID());
    jsonObject["group"] = group().toJsonObject();
    jsonObject["treatment"] = treatment().toJsonObject();
    jsonObject["effect"] = effect().toJsonObject();
    jsonObject["variables"] = variables().toJsonObject();

    jsonObject["graphUser"] = graphUser().toJsonObject();
    jsonObject["graphAlg"] = graphAlg().toJsonObject();

    if(bIncludeProperties)
    {
        jsonObject["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObject;
}

CGroup CCausalModel::group() const
{
    return group_;
}

void CCausalModel::setGroup(const CGroup &group)
{
    group_ = group;
}

CTreatment CCausalModel::treatment() const
{
    return treatment_;
}

void CCausalModel::setTreatment(const CTreatment &treatmeant)
{
//    DEBUG_OBJ_INFO

    treatment_ = treatmeant;
}

CEffect CCausalModel::effect() const
{
    return effect_;
}

CVariablesList CCausalModel::variables() const
{
    return lstVariables_;
}

void CCausalModel::setVariables(const CVariablesList &lstVariables)
{
    lstVariables_ = lstVariables;
}

void CCausalModel::initGraphs()
{
    initGraphUser();
    initGraphAlg();
}

void CCausalModel::initGraphUser()
{
    graphUser_.clear();

    foreach(CVariable variable, lstVariables_)
    {
        CNode nodeNew(variable.name(), QVariant::fromValue(variable));
        graphUser_.addNode(nodeNew);
    }

    QString strTreatmentName = treatment().variable().name();
    QString strEffectName = effect().variable().name();

    CNode nodeTreatment;
    CNode nodeEffect;

    if(graphUser_.nodeByLabel(strTreatmentName, nodeTreatment) &&
            graphUser_.nodeByLabel(strEffectName, nodeEffect))
    {
        CEdge edgeMain(nodeTreatment, nodeEffect);
        graphUser_.addEdge(edgeMain);
    }
}

void CCausalModel::initGraphAlg()
{
    graphAlg_.clear();

    foreach(CVariable variable, lstVariables_)
    {
        CNode nodeNew(variable.name(), QVariant::fromValue(variable));
        graphAlg_.addNode(nodeNew);
    }

//    QString strTreatmentName = treatment().variable().name();
//    QString strEffectName = effect().variable().name();

//    CNode nodeTreatment;
//    CNode nodeEffect;

//    if(graphAlg_.nodeByLabel(strTreatmentName, nodeTreatment) &&
//            graphAlg_.nodeByLabel(strEffectName, nodeEffect))
//    {
//        CEdge edgeMain(nodeTreatment, nodeEffect);
//        graphAlg_.addEdge(edgeMain);
//    }
}

CGraph CCausalModel::graphAlg() const
{
    return graphAlg_;
}

void CCausalModel::setGraphAlg(const CGraph &graphAlg)
{
    graphAlg_ = graphAlg;
}

CGraph CCausalModel::graphUser() const
{
    return graphUser_;
}

void CCausalModel::setGraphUser(const CGraph &graphUser)
{
    graphUser_ = graphUser;
}

void CCausalModel::setEffect(const CEffect &effect)
{
    effect_ = effect;
}

QDataStream& operator<<(QDataStream& out, const CCausalModel& causalModel)
{
    out << causalModel.getID()
        << causalModel.getProperties()
        << causalModel.group()
        << causalModel.treatment()
        << causalModel.effect()
        << causalModel.variables()
        << causalModel.graphUser()
        << causalModel.graphAlg();

    return out;
}

QDataStream& operator>>(QDataStream& in, CCausalModel& causalModel)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;
    CGroup group;
    CTreatment treatment;
    CEffect effect;
    CVariablesList lstVariables;
    CGraph graphUser;
    CGraph graphAlg;

    in >> lID >> mapProperties
            >> group
            >> treatment
            >> effect
            >> lstVariables
            >> graphUser
            >> graphAlg;

    CCausalModel causalModelNew;

    causalModelNew.setID(lID);
    causalModelNew.setProperties(mapProperties);
    causalModelNew.setGroup(group);
    causalModelNew.setTreatment(treatment);
    causalModelNew.setEffect(effect);
    causalModelNew.setVariables(lstVariables);
    causalModelNew.setGraphUser(graphUser);
    causalModelNew.setGraphAlg(graphAlg);

    causalModel = causalModelNew;

    return in;
}

QDebug operator<<(QDebug dbg, const CCausalModel& causalModel)
{
    dbg.nospace()   << g_spacer
                    << "CcausalModel(" << causalModel.getID()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Group:" << causalModel.group() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Treatment:" << causalModel.treatment() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Effect:" << causalModel.effect() << "\n";
    g_spacer.unindent();

//    g_spacer.indent();
//    dbg.nospace() << g_spacer << "Variables:" << causalModel.variables() << "\n";
//    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Graph user:" << causalModel.graphUser() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Graph alg:" << causalModel.graphAlg() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(causalModel.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}
