#include "markovmodel.h"
#include "utils.h"
#include <QSet>

using namespace utils;

CMarkovModel::CMarkovModel()
    : CPropertiesInterface()
{
    qRegisterMetaType<CMarkovModel>("CMarkovModel");
//    qRegisterMetaTypeStreamOperators<CMarkovModel>("CMarkovModel");
}

CMarkovModel::CMarkovModel(const CCausalModel &causalModel, const CVariablesList &blanket)
    : CMarkovModel()
{
    setCausalModel(causalModel);
    setMarkovBlanket(blanket);
}

CMarkovModel::CMarkovModel(const CMarkovModel& markovModel)
    : CMarkovModel()
{
    this->operator =(markovModel);
}

CMarkovModel::~CMarkovModel()
{
}

CMarkovModel& CMarkovModel::operator=(const CMarkovModel& markovModel)
{
    if (this != &markovModel)
    {
        setID(markovModel.getID());
        setProperties(markovModel.getProperties());
        setCausalModel(markovModel.causalModel());
        setMarkovBlanket(markovModel.markovBlanket());
    }

    return *this;
}

bool CMarkovModel::operator==(const CMarkovModel& markovModel) const
{
    return (getID() == markovModel.getID());
}

bool CMarkovModel::operator<(const CMarkovModel& markovModel) const
{
    return (getID() < markovModel.getID());
}

void CMarkovModel::fromJsonObject(const QJsonObject &jsonObject)
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

    /* Causal model */
    CCausalModel causalModel;
    QJsonValue valCausalModel = jsonObject["causalModel"];

    if(valCausalModel.isObject())
    {
        QJsonObject objCausalModel = valCausalModel.toObject();
        causalModel.fromJsonObject(objCausalModel);
        setCausalModel(causalModel);
    }

    /* Markov blanket */
    CVariablesList lstMarkovBlanket;
    QJsonValue valVariablesMB = jsonObject["markovBlanket"];
    if(valVariablesMB.isArray())
    {
        QJsonArray arrMarkovBB = valVariablesMB.toArray();
        foreach(QJsonValue valVariable, arrMarkovBB)
        {
            QJsonObject objVariable = valVariable.toObject();
            QString strVarName = objVariable.keys().first();

            CVariable variable;
            variable.fromJsonObject(objVariable.value(strVarName).toObject());
            lstMarkovBlanket.append(variable);
        }
        setMarkovBlanket(lstMarkovBlanket);
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

QJsonObject CMarkovModel::toJsonObject(bool bIncludeProperties) const
{
//    DEBUG_OBJ_INFO

    QJsonObject jsonObject;

    jsonObject["id"] = QString::number(getID());
    jsonObject["causalModel"] = causalModel().toJsonObject();
    jsonObject["markovBlanket"] = markovBlanket().toJsonObject();

    if(bIncludeProperties)
    {
        jsonObject["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObject;
}

CCausalModel CMarkovModel::causalModel() const
{
    return causalModel_;
}

void CMarkovModel::setCausalModel(const CCausalModel &causalModel)
{
    causalModel_ = causalModel;
}

CVariablesList CMarkovModel::markovBlanket() const
{
    return lstMarkovBlanket_;
}

void CMarkovModel::setMarkovBlanket(const CVariablesList &lstBlanket)
{
    lstMarkovBlanket_ = lstBlanket;
}

CVariablesList CMarkovModel::markovBlanketPlus(int iDepth) const
{
//    DEBUG_OBJ_INFO

    CVariablesList lstVariablesResult;
    if(iDepth < 1)
    {
        return lstVariablesResult;
    }

    CVariablesList lstVariablesMB = markovBlanket();
    CGraph graphUser = causalModel_.graphUser();
    CNodesList lstNodesMB = graphUser.nodesByLabel(lstVariablesMB.variablesNames());
    CNodesList lstNodesMBPlus = graphUser.connectedNodes(lstNodesMB, iDepth);

    CVariablesList lstAllVariables = causalModel_.variables();
    foreach(CNode nodeResult, lstNodesMBPlus)
    {
        CVariable variable = lstAllVariables.byName(nodeResult.label());
        lstVariablesResult.append(variable);
    }

    return lstVariablesResult;
}

QDataStream& operator<<(QDataStream& out, const CMarkovModel& markovModel)
{
    out << markovModel.getID()
        << markovModel.getProperties()
        << markovModel.causalModel()
        << markovModel.markovBlanket();

    return out;
}

QDataStream& operator>>(QDataStream& in, CMarkovModel& markovModel)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;
    CCausalModel causalModel;
    CVariablesList lstMarkovBlanket;

    in >> lID >> mapProperties
            >> causalModel
            >> lstMarkovBlanket;

    CMarkovModel markovModelNew;

    markovModelNew.setID(lID);
    markovModelNew.setProperties(mapProperties);
    markovModelNew.setCausalModel(causalModel);
    markovModelNew.setMarkovBlanket(lstMarkovBlanket);

    markovModel = markovModelNew;

    return in;
}

QDebug operator<<(QDebug dbg, const CMarkovModel& markovModel)
{
    dbg.nospace()   << g_spacer
                    << "CMarkovModel(" << markovModel.getID()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Causal model:" << markovModel.causalModel() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Markov blanket:" << markovModel.markovBlanket() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(markovModel.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}
