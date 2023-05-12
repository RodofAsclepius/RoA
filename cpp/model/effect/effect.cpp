#include "effect.h"
#include "utils.h"

using namespace utils;

CEffect::CEffect()
    : CPropertiesInterface()
{
    qRegisterMetaType<CEffect>("CEffect");
//    qRegisterMetaTypeStreamOperators<CEffect>("CEffect");
    setIsValid(false);
}

CEffect::CEffect(const CEffect &effect)
    : CEffect()
{
    this->operator =(effect);
}

CEffect::CEffect(const CVariable &variable)
    : CEffect()
{
    setVariable(variable);
}

CEffect::~CEffect()
{

}

CEffect& CEffect::operator=(const CEffect& effect)
{
    if (this != &effect)
    {
        setID(effect.getID());
        setProperties(effect.getProperties());
        setVariable(effect.variable());
        setIsValid(effect.isValid());
    }
    return *this;
}

bool CEffect::operator<(const CEffect& effect) const
{
    return (getID() < effect.getID());
}

bool CEffect::operator==(const CEffect& effect) const
{
    return (getID() == effect.getID());
}

QJsonObject CEffect::toJsonObject(bool bIncludeProperties) const
{
//    DEBUG_OBJ_INFO

    QJsonObject jsonObject;

    jsonObject["id"] = QString::number(getID());
    jsonObject["variable"] = variable().toJsonObject();
    jsonObject["valid"] = isValid();

    if(bIncludeProperties)
    {
        jsonObject["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObject;
}

void CEffect::fromJsonObject(const QJsonObject &jsonObject)
{
//    DEBUG_OBJ_INFO

    QString strID = jsonObject["id"].toString();
    QString strEstimationMethod = jsonObject["estimationMethod"].toString();
    bool ok;
    quint32 lID = strID.toUInt(&ok, 10);
    if(lID != 0)
    {
        setID(lID);
    }

    /* Variable */
    CVariable variable;
    QJsonValue valVariable = jsonObject["variable"];
    if(valVariable.isObject())
    {
        QJsonObject objVariable = valVariable.toObject();
        variable.fromJsonObject(objVariable);
        setVariable(variable);
    }

    /* Valid */
    bool bIsValid = jsonObject["valid"].toBool();
    setIsValid(bIsValid);

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

CVariable CEffect::variable() const
{
    return variable_;
}

void CEffect::setVariable(const CVariable &variable)
{
    variable_ = variable;
    setIsValid(true);
}

bool CEffect::isValid() const
{
    return bIsValid_;
}

void CEffect::setIsValid(bool bIsValid)
{
    bIsValid_ = bIsValid;
}


QDataStream& operator<<(QDataStream& out, const CEffect& effect)
{
    out << effect.getID()
        << effect.getProperties()
        << effect.variable()
        << effect.isValid();

    return out;
}

QDataStream& operator>>(QDataStream& in, CEffect& effect)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;

    QString strName;
    CVariable variable;
    bool bIsValid;

    in >> lID >> mapProperties >> variable >> bIsValid;

    effect.setID(lID);
    effect.setProperties(mapProperties);
    effect.setVariable(variable);
    effect.setIsValid(bIsValid);

    return in;
}

QDebug operator<<(QDebug dbg, const CEffect& effect)
{
    dbg.nospace()   << g_spacer
                    << "Ceffect(" << effect.getID()
                    << ", " << effect.variable()
                    << ", valid: " << effect.isValid()
                    << ")" << "\n";



    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(effect.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}

