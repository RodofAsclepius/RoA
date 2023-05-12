#include "treatment.h"
#include "utils.h"

using namespace utils;

CTreatment::CTreatment()
    : CPropertiesInterface()
{
    qRegisterMetaType<CTreatment>("CTreatment");
//    qRegisterMetaTypeStreamOperators<CTreatment>("CTreatment");
    setIsValid(false);
}

CTreatment::CTreatment(const CTreatment &treatment)
    : CTreatment()
{
    this->operator =(treatment);
}

CTreatment::CTreatment(const CVariable &variable, QString strMethod)
    : CTreatment()
{
    setVariable(variable);
    setEstimationMethod(strMethod);
}

CTreatment::~CTreatment()
{

}

CTreatment& CTreatment::operator=(const CTreatment& treatment)
{
    if (this != &treatment)
    {
        setID(treatment.getID());
        setProperties(treatment.getProperties());
        setVariable(treatment.variable());
        setEstimationMethod(treatment.estimationMethod());
        setLevelTreatment(treatment.levelTreatment());
        setLevelControl(treatment.levelControl());
        setIsValid(treatment.isValid());
    }
    return *this;
}


bool CTreatment::operator<(const CTreatment& treatment) const
{
    return (getID() < treatment.getID());
}

bool CTreatment::operator==(const CTreatment& treatment) const
{
    return (getID() == treatment.getID());
}

QJsonObject CTreatment::toJsonObject(bool bIncludeProperties) const
{
//    DEBUG_OBJ_INFO

    QJsonObject jsonObject;

    jsonObject["id"] = QString::number(getID());
    jsonObject["variable"] = variable().toJsonObject();
    jsonObject["estimationMethod"] = estimationMethod();
    jsonObject["levelTreatment"] = levelTreatment();
    jsonObject["levelControl"] = levelControl();
    jsonObject["valid"] = isValid();

    if(bIncludeProperties)
    {
        jsonObject["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObject;
}

void CTreatment::fromJsonObject(const QJsonObject &jsonObject)
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

    /* Method */
    setEstimationMethod(strEstimationMethod);

    /* Level treatment */
    QString strLevelTreatment = jsonObject["levelTreatment"].toString();
    setLevelTreatment(strLevelTreatment);

    /* Level treatment */
    QString strLevelControl = jsonObject["levelControl"].toString();
    setLevelControl(strLevelControl);

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

CVariable CTreatment::variable() const
{
    return variable_;
}

void CTreatment::setVariable(const CVariable &variable)
{
    variable_ = variable;
    variable_.levels();

    QStringList lstLevels = variable_.levels();
    if(variable_.type() == "factor" && lstLevels.count() == 2)
    {
        setLevelControl(lstLevels.at(0));
        setLevelTreatment(lstLevels.at(1));
        setIsValid(true);
    }
}

QString CTreatment::estimationMethod() const
{
    return strEstimationMethod_;
}

void CTreatment::setEstimationMethod(const QString &strEstimationMethod)
{
    strEstimationMethod_ = strEstimationMethod;
}

QStringList CTreatment::estimationMethods()
{
    QStringList lstMethods;

    lstMethods << "ATE" << "ATT";

    return lstMethods;
}

QString CTreatment::levelTreatment() const
{
    return strLevelTreatment;
}

void CTreatment::setLevelTreatment(const QString &value)
{
    strLevelTreatment = value;
}

QString CTreatment::levelControl() const
{
    return strLevelControl;
}

void CTreatment::setLevelControl(const QString &value)
{
    strLevelControl = value;
}

bool CTreatment::isValid() const
{
    return bIsValid_;
}

void CTreatment::setIsValid(bool bIsValid)
{
    bIsValid_ = bIsValid;
}

QDataStream& operator<<(QDataStream& out, const CTreatment& treatment)
{
    out << treatment.getID()
        << treatment.getProperties()
        << treatment.variable()
        << treatment.estimationMethod()
        << treatment.levelTreatment()
        << treatment.levelControl()
        << treatment.isValid();

    return out;
}

QDataStream& operator>>(QDataStream& in, CTreatment& treatment)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;

    QString strName;
    CVariable variable;
    QString strMethod;
    QString strLevelTreatment;
    QString strLevelControl;
    bool bIsValid;

    in >> lID
        >> mapProperties
        >> variable
        >> strMethod
        >> strLevelTreatment
        >> strLevelControl
        >> bIsValid;

    treatment.setID(lID);
    treatment.setProperties(mapProperties);
    treatment.setVariable(variable);
    treatment.setEstimationMethod(strMethod);
    treatment.setLevelTreatment(strLevelTreatment);
    treatment.setLevelControl(strLevelControl);
    treatment.setIsValid(bIsValid);

    return in;
}

QDebug operator<<(QDebug dbg, const CTreatment& treatment)
{
    dbg.nospace()   << g_spacer
                    << "CTreatment(" << treatment.getID()
                    << ", " << treatment.variable()
                    << ", " << treatment.estimationMethod()
                    << ", valid " << treatment.isValid()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(treatment.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}

