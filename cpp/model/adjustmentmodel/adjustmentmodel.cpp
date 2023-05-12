#include "adjustmentmodel.h"
#include "utils.h"
#include "groupslist.h"

using namespace utils;

CAdjustmentModel::CAdjustmentModel()
    : CPropertiesInterface()
{
    qRegisterMetaType<CAdjustmentModel>("CAdjustmentModel");
//    qRegisterMetaTypeStreamOperators<CAdjustmentModel>("CadjustmentModel");
}

CAdjustmentModel::CAdjustmentModel(const CAdjustmentModel& adjustmentModel)
    : CAdjustmentModel()
{
    this->operator =(adjustmentModel);
}

CAdjustmentModel::~CAdjustmentModel()
{
}

CAdjustmentModel& CAdjustmentModel::operator=(const CAdjustmentModel& adjustmentModel)
{
    if (this != &adjustmentModel)
    {
        setID(adjustmentModel.getID());
        setProperties(adjustmentModel.getProperties());
        setCausalModel(adjustmentModel.causalModel());
        setAdjustmentSets(adjustmentModel.adjustmentSets());
        setPaths(adjustmentModel.paths());
        setOpenPaths(adjustmentModel.openPaths());
    }

    return *this;
}

bool CAdjustmentModel::operator==(const CAdjustmentModel& adjustmentModel) const
{
    return (getID() == adjustmentModel.getID());
}

bool CAdjustmentModel::operator<(const CAdjustmentModel& adjustmentModel) const
{
    return (getID() < adjustmentModel.getID());
}

void CAdjustmentModel::fromJsonObject(const QJsonObject &jsonObject)
{
//    DEBUG_OBJ_INFO

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
        causalModel.fromJsonObject(valCausalModel.toObject());
    }
    setCausalModel(causalModel);

    /* Adjustment sets */
    CAdjustmentSetList lstAdjustmentSets;
    QJsonValue valAdjustmentSetList = jsonObject["adjustmentSets"];

    //    qDebug() << valAdjustmentSetList;
    if(valAdjustmentSetList.isArray())
    {
        lstAdjustmentSets.fromJsonArray(valAdjustmentSetList.toArray());
    }
    setAdjustmentSets(lstAdjustmentSets);

    /* Paths */
    QStringList lstPaths;
    QJsonValue valPathsInfo = jsonObject["pathsInfo"].toObject();
    QJsonValue valPaths = valPathsInfo["paths"];
    if(valPaths.isArray())
    {
        foreach(QJsonValue valPath, valPaths.toArray())
        {
            lstPaths << valPath.toString();
        }
    }
    setPaths(lstPaths);

    /* Open paths */
    QVariantList lstOpenPaths;
    QJsonValue valOpenPaths = valPathsInfo["open"];
    if(valOpenPaths.isArray())
    {
        foreach(QJsonValue valOpenPath, valOpenPaths.toArray())
        {
            lstOpenPaths << valOpenPath.toBool();
        }
    }
    setOpenPaths(lstOpenPaths);

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

QJsonObject CAdjustmentModel::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObject;

    jsonObject["id"] = QString::number(getID());
    jsonObject["causalModel"] = causalModel().toJsonObject();
    jsonObject["adjustmentSets"] = lstAdjustmentSets_.toJsonObject();

    if(bIncludeProperties)
    {
        jsonObject["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObject;
}

CCausalModel CAdjustmentModel::causalModel() const
{
    return causalModel_;
}

void CAdjustmentModel::setCausalModel(const CCausalModel &causalModel)
{
    causalModel_ = causalModel;
}

CAdjustmentSetList CAdjustmentModel::adjustmentSets() const
{
    return lstAdjustmentSets_;
}

void CAdjustmentModel::setAdjustmentSets(const CAdjustmentSetList &lstAdjustmentSets)
{
    lstAdjustmentSets_ = lstAdjustmentSets;
}

QStringList CAdjustmentModel::paths() const
{
    return lstPaths_;
}

void CAdjustmentModel::setPaths(const QStringList &lstPaths)
{
    lstPaths_ = lstPaths;
}

QVariantList CAdjustmentModel::openPaths() const
{
    return lstOpenPaths_;
}

void CAdjustmentModel::setOpenPaths(const QVariantList &lstPathIsOpen)
{
    lstOpenPaths_ = lstPathIsOpen;
}

QDataStream& operator<<(QDataStream& out, const CAdjustmentModel& adjustmentModel)
{
    out << adjustmentModel.getID()
        << adjustmentModel.getProperties()
        << adjustmentModel.causalModel()
        << adjustmentModel.adjustmentSets()
        << adjustmentModel.paths()
        << adjustmentModel.openPaths();

    return out;
}

QDataStream& operator>>(QDataStream& in, CAdjustmentModel& adjustmentModel)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;
    CCausalModel causalModel;
    CAdjustmentSetList lstAdjustmentSets;
    QStringList lstPaths;
    QVariantList lstOpenPaths;

    in >> lID >> mapProperties
            >> causalModel
            >> lstAdjustmentSets
            >> lstPaths
            >> lstOpenPaths;

    CAdjustmentModel adjustmentModelNew;

    adjustmentModelNew.setID(lID);
    adjustmentModelNew.setProperties(mapProperties);
    adjustmentModelNew.setCausalModel(causalModel);
    adjustmentModelNew.setAdjustmentSets(lstAdjustmentSets);
    adjustmentModelNew.setPaths(lstPaths);
    adjustmentModelNew.setOpenPaths(lstOpenPaths);

    adjustmentModel = adjustmentModelNew;

    return in;
}

QDebug operator<<(QDebug dbg, const CAdjustmentModel& adjustmentModel)
{
    dbg.nospace()   << g_spacer
                    << "CadjustmentModel(" << adjustmentModel.getID()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Causal mdoel:" << adjustmentModel.causalModel() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Adjustment sets:" << adjustmentModel.adjustmentSets() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Paths:" << adjustmentModel.paths() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Open paths:" << adjustmentModel.openPaths() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(adjustmentModel.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}
