#include "variablebalanceinfo.h"
#include "utils.h"

using namespace utils;

CVariableBalanceInfo::CVariableBalanceInfo()
    : CPropertiesInterface()
{
    qRegisterMetaType<CVariableBalanceInfo>("CVariableBalanceInfo");
//    qRegisterMetaTypeStreamOperators<CVariableBalanceInfo>("CVariableBalanceInfo");

}

CVariableBalanceInfo::CVariableBalanceInfo(const CVariableBalanceInfo& balanceInfo)
    : CVariableBalanceInfo()
{
    this->operator =(balanceInfo);
}

CVariableBalanceInfo::~CVariableBalanceInfo()
{
}

CVariableBalanceInfo& CVariableBalanceInfo::operator=(const CVariableBalanceInfo& balanceInfo)
{
    if (this != &balanceInfo)
    {
        setID(balanceInfo.getID());
        setLevelValues(balanceInfo.levelValues());
        setProperties(balanceInfo.getProperties());
    }
    return *this;
}

bool CVariableBalanceInfo::operator<(const CVariableBalanceInfo& variable) const
{
    return (getID() < variable.getID());
}


bool CVariableBalanceInfo::operator==(const CVariableBalanceInfo& variable) const
{
    return (getID() == variable.getID());
}

void CVariableBalanceInfo::fromJsonObject(QJsonObject objBalanceInfo)
{
    QMap<QString, QMap<QString, qreal>> mapLevelValues;

    foreach(QString strLevel, objBalanceInfo.keys())
    {
        QMap<QString, qreal> balanceInfo;
        QJsonObject objBalanceValues = objBalanceInfo[strLevel].toObject();
        foreach(QString strMetric, objBalanceValues.keys())
        {
            balanceInfo[strMetric] = objBalanceValues[strMetric].toDouble();
        }

        mapLevelValues[strLevel] = balanceInfo;
    }

    setLevelValues(mapLevelValues);
}

QMap<QString, QMap<QString, qreal> > CVariableBalanceInfo::levelValues() const
{
    return mapLevelValues;
}

void CVariableBalanceInfo::setLevelValues(const QMap<QString, QMap<QString, qreal> > &value)
{
    mapLevelValues = value;
}

QDataStream& operator<<(QDataStream& out, const CVariableBalanceInfo& balanceInfo)
{

    out << balanceInfo.getID()
        << balanceInfo.levelValues();

    return out;
}


QDataStream& operator>>(QDataStream& in, CVariableBalanceInfo& balanceInfo)
{
    quint32 lID;
    QMap<QString, QMap<QString, qreal>> mapLevelValues;

    in >> lID
       >> mapLevelValues;

    CVariableBalanceInfo balanceInfoNew;
    balanceInfoNew.setID(lID);
    balanceInfoNew.setLevelValues(mapLevelValues);

    balanceInfo = balanceInfoNew;

    return in;
}


QDebug operator<<(QDebug dbg, const CVariableBalanceInfo& balanceInfo)
{
    dbg.nospace()   << g_spacer
                    << "CVariableBalanceInfo(" << balanceInfo.getID()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "values:" << balanceInfo.levelValues() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIteratorProperties(balanceInfo.getProperties());
    while (mapIteratorProperties.hasNext())
    {
          mapIteratorProperties.next();
          dbg.nospace() << g_spacer << mapIteratorProperties.key() << " " << mapIteratorProperties.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}
