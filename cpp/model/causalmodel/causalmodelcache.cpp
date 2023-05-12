#include "causalmodelcache.h"
#include "utils.h"
#include "groupslist.h"

using namespace utils;

CCausalModelCache::CCausalModelCache()
    : CPropertiesInterface()
{
    qRegisterMetaType<CCausalModelCache>("CCausalModelCache");
//    qRegisterMetaTypeStreamOperators<CCausalModelCache>("CCausalModelCache");
}

CCausalModelCache::CCausalModelCache(const CGroup &group, const CTreatment &treatment, const CEffect &effect)
    : CCausalModelCache()
{
    setGroup(group);
    setTreatment(treatment);
    setEffect(effect);
}

CCausalModelCache::CCausalModelCache(const CCausalModelCache& causalModelCache)
    : CCausalModelCache()
{
    this->operator =(causalModelCache);
}

CCausalModelCache::~CCausalModelCache()
{
}

CCausalModelCache& CCausalModelCache::operator=(const CCausalModelCache& causalModelCache)
{
    if (this != &causalModelCache)
    {
        setID(causalModelCache.getID());
        setProperties(causalModelCache.getProperties());
        setGroup(causalModelCache.group());
        setTreatment(causalModelCache.treatment());
        setEffect(causalModelCache.effect());
        setNodePositionsUserGraph(causalModelCache.nodePositionsUserGraph());
        setNodePositionsMinedGraph(causalModelCache.nodePositionsMinedGraph());
    }

    return *this;
}

bool CCausalModelCache::operator==(const CCausalModelCache& causalModelCache) const
{
    return (getID() == causalModelCache.getID());
}

bool CCausalModelCache::operator<(const CCausalModelCache& causalModelCache) const
{
    return (getID() < causalModelCache.getID());
}

void CCausalModelCache::fromJsonObject(const QJsonObject &jsonObject)
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

QJsonObject CCausalModelCache::toJsonObject(bool bIncludeProperties) const
{
//    DEBUG_OBJ_INFO

    QJsonObject jsonObject;

    jsonObject["id"] = QString::number(getID());
    jsonObject["group"] = group().toJsonObject();
    jsonObject["treatment"] = treatment().toJsonObject();
    jsonObject["effect"] = effect().toJsonObject();

    if(bIncludeProperties)
    {
        jsonObject["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObject;
}

CGroup CCausalModelCache::group() const
{
    return group_;
}

void CCausalModelCache::setGroup(const CGroup &group)
{
    group_ = group;
}

CTreatment CCausalModelCache::treatment() const
{
    return treatment_;
}

void CCausalModelCache::setTreatment(const CTreatment &treatmeant)
{
//    DEBUG_OBJ_INFO

    treatment_ = treatmeant;
}

CEffect CCausalModelCache::effect() const
{
    return effect_;
}

QMap<QString, QPointF> CCausalModelCache::nodePositionsUserGraph() const
{
    return mapNodePositionsUserGraph_;
}

void CCausalModelCache::setNodePositionsUserGraph(const QMap<QString, QPointF> &mapNodePosition)
{
    mapNodePositionsUserGraph_ = mapNodePosition;
}

QMap<QString, QPointF> CCausalModelCache::nodePositionsMinedGraph() const
{
    return mapNodePositionMinedGraph_;
}

void CCausalModelCache::setNodePositionsMinedGraph(const QMap<QString, QPointF> &mapNodePositionMinedGraph)
{
    mapNodePositionMinedGraph_ = mapNodePositionMinedGraph;
}

void CCausalModelCache::setEffect(const CEffect &effect)
{
    effect_ = effect;
}

QDataStream& operator<<(QDataStream& out, const CCausalModelCache& causalModelCache)
{
    out << causalModelCache.getID()
        << causalModelCache.getProperties()
        << causalModelCache.group()
        << causalModelCache.treatment()
        << causalModelCache.effect()
        << causalModelCache.nodePositionsUserGraph()
        << causalModelCache.nodePositionsMinedGraph();

    return out;
}

QDataStream& operator>>(QDataStream& in, CCausalModelCache& causalModelCache)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;
    CGroup group;
    CTreatment treatment;
    CEffect effect;
    QMap<QString, QPointF> mapNodePositionUserGraph;
    QMap<QString, QPointF> mapNodePositionMinedGraph;

    in >> lID >> mapProperties
            >> group
            >> treatment
            >> effect
            >> mapNodePositionUserGraph
            >> mapNodePositionMinedGraph;

    CCausalModelCache causalModelCacheNew;

    causalModelCacheNew.setID(lID);
    causalModelCacheNew.setProperties(mapProperties);
    causalModelCacheNew.setGroup(group);
    causalModelCacheNew.setTreatment(treatment);
    causalModelCacheNew.setEffect(effect);
    causalModelCacheNew.setNodePositionsUserGraph(mapNodePositionUserGraph);
    causalModelCacheNew.setNodePositionsMinedGraph(mapNodePositionMinedGraph);

    causalModelCache = causalModelCacheNew;

    return in;
}

QDebug operator<<(QDebug dbg, const CCausalModelCache& causalModelCache)
{
    dbg.nospace()   << g_spacer
                    << "CCausalModelCache(" << causalModelCache.getID()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Group:" << causalModelCache.group() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Treatment:" << causalModelCache.treatment() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Effect:" << causalModelCache.effect() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Node positions user graph:" << causalModelCache.nodePositionsUserGraph() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Node positions mined graph:" << causalModelCache.nodePositionsMinedGraph() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(causalModelCache.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}
