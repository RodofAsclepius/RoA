#include "variableeffectinfo.h"
#include "utils.h"

using namespace utils;

CVariableEffectInfo::CVariableEffectInfo()
    : CPropertiesInterface()
{
    qRegisterMetaType<CVariableEffectInfo>("CVariableEffectInfo");
//    qRegisterMetaTypeStreamOperators<CVariableEffectInfo>("CVariableEffectInfo");

}

CVariableEffectInfo::CVariableEffectInfo(const CVariableEffectInfo& effectInfo)
    : CVariableEffectInfo()
{
    this->operator =(effectInfo);
}

CVariableEffectInfo::~CVariableEffectInfo()
{
}

CVariableEffectInfo& CVariableEffectInfo::operator=(const CVariableEffectInfo& effectInfo)
{
    if (this != &effectInfo)
    {
        setID(effectInfo.getID());
        setProperties(effectInfo.getProperties());
        setValues(effectInfo.values());
        setMethod(effectInfo.method());
    }
    return *this;
}

bool CVariableEffectInfo::operator<(const CVariableEffectInfo& variable) const
{
    return (getID() < variable.getID());
}


bool CVariableEffectInfo::operator==(const CVariableEffectInfo& variable) const
{
    return (getID() == variable.getID());
}

void CVariableEffectInfo::fromJsonObject(QJsonObject objEffectInfo)
{
    /* Values */
    QMap<QString, qreal> mapValues;
    foreach(QString strValue, objEffectInfo.keys())
    {
        mapValues[strValue] = objEffectInfo[strValue].toDouble();
    }

    setValues(mapValues);

}

QMap<QString, qreal> CVariableEffectInfo::values() const
{
    return mapValues_;
}

void CVariableEffectInfo::setValues(const QMap<QString, qreal> &value)
{
    mapValues_ = value;
}

QString CVariableEffectInfo::method() const
{
    return strMethod;
}

void CVariableEffectInfo::setMethod(const QString &value)
{
    strMethod = value;
}

QDataStream& operator<<(QDataStream& out, const CVariableEffectInfo& effectInfo)
{
    out << effectInfo.getID()
        << effectInfo.getProperties()
        << effectInfo.values()
        << effectInfo.method();

    return out;
}


QDataStream& operator>>(QDataStream& in, CVariableEffectInfo& effectInfo)
{
    quint32 lID;
    QVariantMap mapProperties;
    QMap<QString, qreal> mapValues;
    QString strMethod;

    in >> lID
       >> mapProperties
       >> mapValues
       >> strMethod;

    CVariableEffectInfo effectInfoNew;
    effectInfoNew.setID(lID);
    effectInfoNew.setProperties(mapProperties);

    effectInfoNew.setValues(mapValues);
    effectInfoNew.setMethod(strMethod);

    effectInfo = effectInfoNew;

    return in;
}


QDebug operator<<(QDebug dbg, const CVariableEffectInfo& effectInfo)
{
    dbg.nospace()   << g_spacer
                    << "CVariableEffectInfo(" << effectInfo.getID()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Values:" << effectInfo.values() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIteratorProperties(effectInfo.getProperties());
    while (mapIteratorProperties.hasNext())
    {
          mapIteratorProperties.next();
          dbg.nospace() << g_spacer << mapIteratorProperties.key() << " " << mapIteratorProperties.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}
