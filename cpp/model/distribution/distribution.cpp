#include "distribution.h"
#include "maincontroller.h"
#include "utils.h"

#include <QMutexLocker>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

using namespace utils;

CDistribution::CDistribution()
{
    qRegisterMetaType<CDistribution>("CDistribution");
//    qRegisterMetaTypeStreamOperators<CDistribution>("CDistribution");
}


CDistribution::CDistribution(const CGroup& group, const CVariable& variable)
    : CDistribution()
{
//    DEBUG_OBJ_INFO

    setGroup(group);
    setVariable(variable);
}


CDistribution::CDistribution(const CDistribution& distribution)
    : CDistribution()
{
    this->operator =(distribution);
}


CDistribution::~CDistribution()
{
}


CDistribution& CDistribution::operator=(const CDistribution& distribution)
{
    if (this != &distribution)
    {
        setID(distribution.getID());
        setGroup(distribution.group());
        setVariable(distribution.variable());
        setFrequencies(distribution.frequencies());
        setDensities(distribution.densities());
        setDensitiesBW(distribution.densitiesBW());
        setDensitiesN(distribution.densitiesN());
        setProperties(distribution.getProperties());
        setViolinValues(distribution.violinValues());
        setBoxPlotViolin(distribution.boxPlotViolin());
        setViolinN(distribution.violinN());
        setLevelLabelViolin(distribution.levelLabelViolin());
    }
    return *this;
}


bool CDistribution::operator<(const CDistribution& distribution) const
{
    return (getID() < distribution.getID());
}


bool CDistribution::operator==(const CDistribution& distribution) const
{
    return (getID() == distribution.getID());
}

void CDistribution::setVariable(const CVariable &variable)
{
    variable_ = variable;
}

CVariable CDistribution::variable() const
{
    return variable_;
}


CGroup CDistribution::group() const
{
    return group_;
}

void CDistribution::setGroup(const CGroup &group)
{
    group_ = group;
}

QJsonObject CDistribution::toJsonObject(bool bIncludeProperties) const
{
//    DEBUG_OBJ_INFO

    QJsonObject jsonObject;

    jsonObject["id"] = QString::number(getID());

    jsonObject["variable"] = variable_.toJsonObject();
    jsonObject["group"] = group_.toJsonObject();

    /* ToDO*/
//    QVector<QPair<QString, int>> _vecFrequencies;
//    QVector<QPair<qreal, qreal>> _vecDensities;
// BW AND N

    /* Remaining properties */
    jsonObject["properties"] = QJsonObject();

    if(bIncludeProperties)
    {
        jsonObject["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObject;
}


void CDistribution::fromJsonObject(const QJsonObject& jsonDoc)
{
//    DEBUG_OBJ_INFO

    /* ID */
    QString strID = jsonDoc["id"].toString();

    bool ok;
    quint32 lID = strID.toUInt(&ok, 10);
    if(lID != 0)
    {
        setID(lID);
    }

    /* Group */
    QJsonObject objGroup = jsonDoc["group"].toObject();
    CGroup group;
    group.fromJsonObject(objGroup);
    setGroup(group);

    /* Variable */
    QJsonObject objVariable = jsonDoc["variable"].toObject();
    CVariable variable;
    variable.fromJsonObject(objVariable);
    setVariable(variable);

    /* Frequencies */
    QMap<QString, int> mapFrequencies;
    QJsonObject objFrequencies = jsonDoc["frequencies"].toObject();
    QJsonArray arrLabels = objFrequencies["labels"].toArray();
    QJsonArray arrValues = objFrequencies["values"].toArray();

    for(int i=0; i<arrLabels.count(); ++i)
    {
        QString strLabel = QString::number(arrLabels.at(i).toDouble(-1));
        if(strLabel == "-1")
        {
            strLabel = arrLabels.at(i).toString();
        }
        int iValue = arrValues.at(i).toInt();

        mapFrequencies[strLabel] = iValue;
    }
    setFrequencies(mapFrequencies);

    /* Densities */
    QVector<QPair<qreal, qreal>> vecDensities;
    QJsonObject objDensities = jsonDoc["densities"].toObject();
    QJsonArray arrX = objDensities["x"].toArray();
    QJsonArray arrY = objDensities["y"].toArray();
    qreal rDensitiesBW = objDensities["bw"].toDouble();
    int iDensitiesN = objDensities["n"].toInt();


    for(int i=0; i<arrX.count(); ++i)
    {
        qreal rX = arrX.at(i).toDouble();
        qreal rY = arrY.at(i).toDouble();

        vecDensities.append(QPair<qreal, qreal>(rX, rY));
    }
    setDensities(vecDensities);
    setDensitiesBW(rDensitiesBW);
    setDensitiesN(iDensitiesN);

    /* Distribution */
//    qDebug() << jsonDoc["violin"];
    QJsonObject objViolin = jsonDoc["violin"].toObject();
    QJsonArray arrViolinY = objViolin["y"].toArray();
    QJsonArray arrViolinWidth = objViolin["violinwidth"].toArray();

    int violinN = objViolin["n"].toInt();
    setViolinN(violinN);

    QString strLevelLabelViolin = objViolin["levelLabel"].toString();
    setLevelLabelViolin(strLevelLabelViolin);

    QVector<QPair<qreal, qreal>> vecViolinValues;
    for(int i=0; i<arrViolinY.count(); ++i)
    {
        qreal rViolinY = arrViolinY.at(i).toDouble();
        qreal rViolinWidth = arrViolinWidth.at(i).toDouble();

        vecViolinValues.append(QPair<qreal, qreal>(rViolinY, rViolinWidth));
    }
    setViolinValues(vecViolinValues);

    /* Boxplot violin */
    QJsonArray arBoxPlotViolin = objViolin["boxPlot"].toArray();
    QJsonObject objViolinBoxPlot = arBoxPlotViolin.first().toArray().first().toObject();

    QMap<QString, qreal> mapBoxPlotViolin;
    mapBoxPlotViolin.insert("lower", objViolinBoxPlot["lower"].toDouble());
    mapBoxPlotViolin.insert("middle", objViolinBoxPlot["middle"].toDouble());
    mapBoxPlotViolin.insert("upper", objViolinBoxPlot["upper"].toDouble());
    mapBoxPlotViolin.insert("notchlower", objViolinBoxPlot["notchlower"].toDouble());
    mapBoxPlotViolin.insert("notchupper", objViolinBoxPlot["notchupper"].toDouble());
    mapBoxPlotViolin.insert("ymin_final", objViolinBoxPlot["ymin_final"].toDouble());
    mapBoxPlotViolin.insert("ymax_final", objViolinBoxPlot["ymax_final"].toDouble());

    setBoxPlotViolin(mapBoxPlotViolin);

    /* Properties */
    QJsonObject jsonObjectProperties = jsonDoc["properties"].toObject();
    propertiesFromJsonObject(jsonObjectProperties);
}

QVector<QPair<qreal, qreal> > CDistribution::densities() const
{
    return vecDensities_;
}

void CDistribution::setDensities(const QVector<QPair<qreal, qreal> > &vecDensities)
{
    vecDensities_ = vecDensities;
}

QPair<int, int> CDistribution::frequenciesRange(bool bRounded) const
{
    Q_UNUSED(bRounded)

    int iMin = 9999,
        iMax = -9999;

    QMapIterator<QString, int> mapIt(mapFrequencies_);
    while(mapIt.hasNext())
    {
        mapIt.next();

        QString strID = mapIt.key();
        int iFreq = mapIt.value();

        if(iFreq < iMin)
        {
            iMin = iFreq;
        }

        if(iFreq > iMax)
        {
            iMax = iFreq;
        }
    }

    return QPair<int, int>(iMin, iMax);
}

QPair<int, int> CDistribution::frequenciesDomain(bool bRounded) const
{
    Q_UNUSED(bRounded)

    int iMin = 9999,
        iMax = -9999;

    QMapIterator<QString, int> mapIt(mapFrequencies_);
    while(mapIt.hasNext())
    {
        mapIt.next();

        QString strID = mapIt.key();
        int iFreq = mapIt.value();

        if(iFreq < iMin)
        {
            iMin = iFreq;
        }

        if(iFreq > iMax)
        {
            iMax = iFreq;
        }
    }

    return QPair<int, int>(iMin, iMax);
}

QPair<qreal, qreal> CDistribution::densitiesRange(bool bRounded) const
{
    Q_UNUSED(bRounded)

    qreal rMinY = 9999,
            rMaxY = -9999;

    QVectorIterator<QPair<qreal, qreal>> vecIt(vecDensities_);
    while(vecIt.hasNext())
    {
        QPair<qreal, qreal> pairExtremes = vecIt.next();

        qreal rY = pairExtremes.second;

        if(rY < rMinY)
        {
            rMinY = rY;
        }

        if(rY > rMaxY)
        {
            rMaxY = rY;
        }
    }

    return QPair<qreal, qreal>(rMinY, rMaxY);
}

QPair<qreal, qreal> CDistribution::densitiesDomain(bool bRounded) const
{
    Q_UNUSED(bRounded)

    qreal rMinY = 9999,
            rMaxY = -9999;

    QVectorIterator<QPair<qreal, qreal>> vecIt(vecDensities_);
    while(vecIt.hasNext())
    {
        QPair<qreal, qreal> pairExtremes = vecIt.next();

        qreal rX = pairExtremes.first;

        if(rX < rMinY)
        {
            rMinY = rX;
        }

        if(rX > rMaxY)
        {
            rMaxY = rX;
        }
    }

    return QPair<qreal, qreal>(rMinY, rMaxY);
}

QPair<qreal, qreal> CDistribution::violinRange(bool bRounded) const
{
    Q_UNUSED(bRounded)
    qreal rMinY = 9999,
            rMaxY = -9999;

    QVectorIterator<QPair<qreal, qreal>> vecIt(vecViolinValues_);
    while(vecIt.hasNext())
    {
        QPair<qreal, qreal> pairExtremes = vecIt.next();

        qreal rY = pairExtremes.second;

        if(rY < rMinY)
        {
            rMinY = rY;
        }

        if(rY > rMaxY)
        {
            rMaxY = rY;
        }
    }

    return QPair<qreal, qreal>(rMinY, rMaxY);
}

QPair<qreal, qreal> CDistribution::violinDomain(bool bRounded) const
{
    Q_UNUSED(bRounded)

    qreal rMinY = 9999,
            rMaxY = -9999;

    QVectorIterator<QPair<qreal, qreal>> vecIt(vecViolinValues_);
    while(vecIt.hasNext())
    {
        QPair<qreal, qreal> pairExtremes = vecIt.next();

        qreal rX = pairExtremes.first;

        if(rX < rMinY)
        {
            rMinY = rX;
        }

        if(rX > rMaxY)
        {
            rMaxY = rX;
        }
    }

    return QPair<qreal, qreal>(rMinY, rMaxY);
}

qreal CDistribution::densitiesBW() const
{
    return qDensitiesBW;
}

void CDistribution::setDensitiesBW(const qreal &value)
{
    qDensitiesBW = value;
}

int CDistribution::densitiesN() const
{
    return iDensitiesN;
}

void CDistribution::setDensitiesN(int value)
{
    iDensitiesN = value;
}

QVector<QPair<qreal, qreal> > CDistribution::violinValues() const
{
    return vecViolinValues_;
}

void CDistribution::setViolinValues(const QVector<QPair<qreal, qreal> > &vecViolin)
{
    vecViolinValues_ = vecViolin;
}

QMap<QString, qreal> CDistribution::boxPlotViolin() const
{
    return mapBoxPlotViolin_;
}

void CDistribution::setBoxPlotViolin(const QMap<QString, qreal> &mapBoxPlot)
{
    mapBoxPlotViolin_ = mapBoxPlot;
}

int CDistribution::violinN() const
{
    return iViolinN_;
}

void CDistribution::setViolinN(int iViolinN)
{
    iViolinN_ = iViolinN;
}

QString CDistribution::levelLabelViolin() const
{
    return strLevelLabelViolin_;
}

void CDistribution::setLevelLabelViolin(const QString &strLevelLabelViolin)
{
    strLevelLabelViolin_ = strLevelLabelViolin;
}

QMap<QString, int> CDistribution::frequencies() const
{
    return mapFrequencies_;
}

void CDistribution::setFrequencies(const QMap<QString, int> &mapFrequencies)
{
    mapFrequencies_ = mapFrequencies;
}

QDataStream& operator<<(QDataStream& out, const CDistribution& distribution)
{
    out << distribution.getID()
        << distribution.variable()
        << distribution.group()
        << distribution.frequencies()
        << distribution.densities()
        << distribution.violinN()
        << distribution.levelLabelViolin()
        << distribution.violinValues()
        << distribution.boxPlotViolin()
        << distribution.getProperties();

    return out;
}

QDataStream& operator>>(QDataStream& in, CDistribution& distribution)
{
    quint32 lID;
    CVariable variable;
    CGroup group;
    QMap<QString, int> mapFrequencies;
    QVector<QPair<qreal, qreal>> vecDensities;
    QVector<QPair<qreal, qreal>> vecViolin;
    QMap<QString, qreal> mapBoxPlot;
    int iViolinN;
    QMap<QString, QVariant> mapProperties;
    QString strLevelLabelViolin;

    in >> lID
       >> variable
       >> group
       >> mapFrequencies
       >> vecDensities
       >> iViolinN
       >> strLevelLabelViolin
       >> vecViolin
       >> mapBoxPlot
       >> mapProperties;

    distribution.setID(lID);
    distribution.setGroup(group);
    distribution.setVariable(variable);
    distribution.setFrequencies(mapFrequencies);
    distribution.setDensities(vecDensities);
    distribution.setViolinN(iViolinN);
    distribution.setLevelLabelViolin(strLevelLabelViolin);
    distribution.setViolinValues(vecViolin);
    distribution.setBoxPlotViolin(mapBoxPlot);
    distribution.setProperties(mapProperties);

    return in;
}

QDebug operator<<(QDebug dbg, const CDistribution& distribution)
{
    dbg.nospace()   << g_spacer
                    << "CDistribution(" << distribution.getID()
                    << ")" << "\n";

    /* Group */
    g_spacer.indent();
        dbg.nospace() << g_spacer << "Groups:" << "\n";
        dbg.nospace() << g_spacer << distribution.group() << "\n";
    g_spacer.unindent();

    /* Variables */
    g_spacer.indent();
        dbg.nospace() << g_spacer << "Variables:" << "\n";
        dbg.nospace() << g_spacer << distribution.variable() << "\n";
    g_spacer.unindent();

    /* Frequencies */
    g_spacer.indent();
        dbg.nospace() << g_spacer << "Frequencies:" << "\n";
        dbg.nospace() << g_spacer << distribution.frequencies() << "\n";
    g_spacer.unindent();

    /* Densities */
    g_spacer.indent();
        dbg.nospace() << g_spacer << "Densities:" << "\n";
        dbg.nospace() << g_spacer << distribution.densities() << "\n";
    g_spacer.unindent();

    /* Densities */
    g_spacer.indent();
        dbg.nospace() << g_spacer << "Densities BW:" << distribution.densitiesBW() << "\n";
        dbg.nospace() << g_spacer << "Densities N:" << distribution.densitiesN() << "\n";
    g_spacer.unindent();

    /* Violin */
    g_spacer.indent();
        dbg.nospace() << g_spacer << "Violin:" << distribution.violinN() << distribution.violinValues() << "\n";
    g_spacer.unindent();

    /* Boxplot */
    g_spacer.indent();
        dbg.nospace() << g_spacer << "Boxplot:" << distribution.boxPlotViolin() << "\n";
    g_spacer.unindent();

    /* Properties */
    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(distribution.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}

