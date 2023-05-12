#include "variablebalance.h"
#include "utils.h"

using namespace utils;

CVariableBalance::CVariableBalance()
    : CVariable()
{
    qRegisterMetaType<CVariable>("CVariableBalance");
//    qRegisterMetaTypeStreamOperators<CVariable>("CVariableBalance");
}

CVariableBalance::CVariableBalance(const CVariableBalance &variableBalance)
    : CVariableBalance()
{
    this->operator =(variableBalance);
}

CVariableBalanceInfo CVariableBalance::balanceInfo() const
{
    return balanceInfo_;
}

CVariableBalance::~CVariableBalance()
{
}


CVariableBalance& CVariableBalance::operator=(const CVariableBalance& variableBalance)
{
    if (this != &variableBalance)
    {
        CVariable::operator=(variableBalance);
        setBalanceInfo(variableBalance.balanceInfo());
    }
    return *this;
}


bool CVariableBalance::operator<(const CVariableBalance& variableBalance) const
{
    return (getID() < variableBalance.getID());
}


bool CVariableBalance::operator==(const CVariableBalance& variableBalance) const
{
    return (getID() == variableBalance.getID());
}

void CVariableBalance::setBalanceInfo(const CVariableBalanceInfo &mapBalanceInfo)
{
    balanceInfo_ = mapBalanceInfo;
}

QJsonObject CVariableBalance::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObject = CVariable::toJsonObject(bIncludeProperties);

    return jsonObject;
}

void CVariableBalance::fromJsonObject(const QJsonObject &jsonObject)
{
//    DEBUG_OBJ_INFO

    CVariable::fromJsonObject(jsonObject);

    QJsonObject objBalanceInfo = jsonObject["balance"].toObject();

    CVariableBalanceInfo balanceInfo;
    balanceInfo.fromJsonObject(objBalanceInfo);

    setBalanceInfo(balanceInfo);
}

QDataStream& operator<<(QDataStream& out, const CVariableBalance& variable)
{
    out << static_cast <const CVariable&>( variable )
        << variable.balanceInfo();

    return out;
}


QDataStream& operator>>(QDataStream& in, CVariableBalance& variable)
{
    CVariableBalanceInfo balanceInfo;

    in >> static_cast <CVariable&>( variable )
       >> balanceInfo;

    variable.setBalanceInfo(balanceInfo);

    return in;
}


QDebug operator<<(QDebug dbg, const CVariableBalance& variable)
{
    dbg.nospace()   << g_spacer
                    << "CVariableBalance(" << variable.getID()
                    << ", " << variable.name()
                    << ", " << variable.type()
                    << ", " << variable.levels()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "BalanceInfo:" << variable.balanceInfo() << "\n";
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
