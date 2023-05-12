#include "variableeffect.h"
#include "utils.h"

using namespace utils;

CVariableEffect::CVariableEffect()
    : CVariable()
{
    qRegisterMetaType<CVariable>("CVariableEffect");
//    qRegisterMetaTypeStreamOperators<CVariable>("CVariableEffect");
}

CVariableEffect::CVariableEffect(const CVariableEffect &variableEffect)
    : CVariableEffect()
{
    this->operator =(variableEffect);
}

CVariableEffect::~CVariableEffect()
{
}


CVariableEffect& CVariableEffect::operator=(const CVariableEffect& variableEffect)
{
    if (this != &variableEffect)
    {
        CVariable::operator=(variableEffect);
        setEffectInfo(variableEffect.effectInfo());
    }
    return *this;
}


bool CVariableEffect::operator<(const CVariableEffect& variableEffect) const
{
    return (getID() < variableEffect.getID());
}


bool CVariableEffect::operator==(const CVariableEffect& variableEffect) const
{
    return (getID() == variableEffect.getID());
}

QJsonObject CVariableEffect::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObject = CVariable::toJsonObject(bIncludeProperties);

    return jsonObject;
}

void CVariableEffect::fromJsonObject(const QJsonObject &jsonObject)
{
//    DEBUG_OBJ_INFO

    CVariable::fromJsonObject(jsonObject);

    QJsonObject objEffectInfo = jsonObject["effectInfo"].toObject();

    CVariableEffectInfo effectInfo;
    effectInfo.fromJsonObject(objEffectInfo);

    setEffectInfo(effectInfo);
}

CVariableEffectInfo CVariableEffect::effectInfo() const
{
    return effectInfo_;
}

void CVariableEffect::setEffectInfo(const CVariableEffectInfo &effectInfo)
{
    effectInfo_ = effectInfo;
}

QDataStream& operator<<(QDataStream& out, const CVariableEffect& variable)
{
    out << static_cast <const CVariable&>( variable )
        << variable.effectInfo();

    return out;
}


QDataStream& operator>>(QDataStream& in, CVariableEffect& variable)
{
    CVariableEffectInfo effectInfo;

    in >> static_cast <CVariable&>( variable )
       >> effectInfo;

    variable.setEffectInfo(effectInfo);

    return in;
}


QDebug operator<<(QDebug dbg, const CVariableEffect& variable)
{
    dbg.nospace()   << g_spacer
                    << "CvariableEffect(" << variable.getID()
                    << ", " << variable.name()
                    << ", " << variable.type()
                    << ", " << variable.levels()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "EffectInfo:" << variable.effectInfo() << "\n";
    g_spacer.unindent();

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIteratorProperties(variable.getProperties());
    while (mapIteratorProperties.hasNext())
    {
          mapIteratorProperties.next();
          dbg.nospace() << g_spacer << mapIteratorProperties.key() << " " << mapIteratorProperties.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}
