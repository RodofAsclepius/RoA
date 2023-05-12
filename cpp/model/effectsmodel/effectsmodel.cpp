#include "effectsmodel.h"
#include "utils.h"

using namespace utils;

CEffectsModel::CEffectsModel()
    : CPropertiesInterface()
{
    qRegisterMetaType<CEffectsModel>("CEffectsModel");
//    qRegisterMetaTypeStreamOperators<CEffectsModel>("CEffectsModel");
}

CEffectsModel::CEffectsModel(const CEffectsModel &effectsModel)
    : CEffectsModel()
{
    this->operator =(effectsModel);
}

CEffectsModel::~CEffectsModel()
{
}

CEffectsModel& CEffectsModel::operator=(const CEffectsModel &effectsModel)
{
    if (this != &effectsModel)
    {
        setID(effectsModel.getID());
        setProperties(effectsModel.getProperties());
        setBalanceModel(effectsModel.balanceModel());
        setEffect(effectsModel.effect());
        setDistributionT(effectsModel.distributionT());
        setDistributionU(effectsModel.distributionU());
    }
    return *this;
}

bool CEffectsModel::operator==(const CEffectsModel &effectsModel) const
{
    return (getID() == effectsModel.getID());
}

bool CEffectsModel::operator<(const CEffectsModel &effectsModel) const
{
    return (getID() < effectsModel.getID());
}

void CEffectsModel::fromJsonObject(const QJsonObject &jsonObject)
{
//    DEBUG_OBJ_INFO

    QString strID = jsonObject["id"].toString();
    bool ok;
    quint32 lID = strID.toUInt(&ok, 10);
    if(lID != 0)
    {
        setID(lID);
    }

    /* BalanceModel */
    CBalanceModel balanceModel;

    QJsonValue valBalanceModel = jsonObject["balanceModel"];
    if(valBalanceModel.isObject())
    {
        QJsonObject objBalanceModel = valBalanceModel.toObject();
        balanceModel.fromJsonObject(objBalanceModel);
        setBalanceModel(balanceModel);
    }

    /* Variable effect */
    CEffect effect;
    QJsonValue valEffect = jsonObject["effect"];
    if(valEffect.isObject())
    {
        effect.fromJsonObject(valEffect.toObject());
        setEffect(effect);
    }

    /* DistributionU */
    CDistribution distributionU;
    QJsonValue valDistributionU = jsonObject["distributionU"];
    if(valDistributionU.isObject())
    {
        distributionU.fromJsonObject(valDistributionU.toObject());
        setDistributionU(distributionU);
    }

    /* DistributionT */
    CDistribution distributionT;
    QJsonValue valdistributionT = jsonObject["distributionT"];
    if(valdistributionT.isObject())
    {
        distributionT.fromJsonObject(valdistributionT.toObject());
        setDistributionT(distributionT);
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

QJsonObject CEffectsModel::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObject;

    jsonObject["id"] = QString::number(getID());

    jsonObject["balanceModel"] = balanceModel_.toJsonObject();
    jsonObject["effect"] = effect_.toJsonObject();

    if(bIncludeProperties)
    {
        jsonObject["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObject;
}

CBalanceModel CEffectsModel::balanceModel() const
{
    return balanceModel_;
}

void CEffectsModel::setBalanceModel(const CBalanceModel &balanceModel)
{
    balanceModel_ = balanceModel;
}

CTreatment CEffectsModel::treatment() const
{
    return balanceModel_.treatment();
}

CGroup CEffectsModel::group() const
{
    return balanceModel().group();
}

CDistribution CEffectsModel::distributionU() const
{
    return distributionU_;
}

void CEffectsModel::setDistributionU(const CDistribution &distributionU)
{
    distributionU_ = distributionU;
}

CDistribution CEffectsModel::distributionT() const
{
    return distributionT_;
}

void CEffectsModel::setDistributionT(const CDistribution &distributionT)
{
    distributionT_ = distributionT;
}

CEffect CEffectsModel::effect() const
{
    return effect_;
}

void CEffectsModel::setEffect(const CEffect &effect)
{
    effect_ = effect;
}

QDataStream& operator<<(QDataStream& out, const CEffectsModel &effectsModel)
{
    out << effectsModel.getID()
        << effectsModel.getProperties()
        << effectsModel.balanceModel()
        << effectsModel.effect()
        << effectsModel.distributionU()
        << effectsModel.distributionT();
    
    return out;
}

QDataStream& operator>>(QDataStream& in, CEffectsModel &effectsModel)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;
    CEffect effect;
    CBalanceModel balanceModel;
    CDistribution distributionU;
    CDistribution distributionT;

    in  >> lID >> mapProperties
        >> balanceModel
        >> effect
        >> distributionU
        >> distributionT;

    CEffectsModel effectsModelNew;
    effectsModelNew.setID(lID);
    effectsModelNew.setProperties(mapProperties);
    effectsModelNew.setBalanceModel(balanceModel);
    effectsModelNew.setEffect(effect);
    effectsModelNew.setDistributionU(distributionU);
    effectsModelNew.setDistributionT(distributionT);

    effectsModel = effectsModelNew;

    return in;
}

QDebug operator<<(QDebug dbg, const CEffectsModel &effectsModel)
{
    dbg.nospace()   << g_spacer
                    << "CEffectsModel(" << effectsModel.getID()
                    << ")" << "\n";

    g_spacer.indent();
        dbg.nospace() << g_spacer << "Balance model:" << effectsModel.balanceModel() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
        dbg.nospace() << g_spacer << "Effect:" << effectsModel.effect() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
        dbg.nospace() << g_spacer << "DistributionU:" << effectsModel.distributionU() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
        dbg.nospace() << g_spacer << "DistributionT:" << effectsModel.distributionT() << "\n";
    g_spacer.unindent();


    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(effectsModel.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}
