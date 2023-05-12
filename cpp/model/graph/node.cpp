#include "node.h"
#include "utils.h"

using namespace utils;

CNode::CNode()
{
    QString strLabelNew = QString("Node " + QString::number(getID(), 10));
    setLabel(strLabelNew);
}

CNode::CNode(const QString &strLabel, const QVariant &data, const QString &strType)
    :CNode()
{
    setLabel(strLabel);
    setData(data);
    setType(strType);
}

CNode::CNode(const QString& strLabel, const QString &strType)
{
    qRegisterMetaType<CNode>("CNode");
//    qRegisterMetaTypeStreamOperators<CNode>("CNode");

    setLabel(strLabel);
    setType(strType);
}

CNode::CNode(const CNode& variable)
: CNode()
{
this->operator =(variable);
}


CNode::~CNode()
{
}

CNode& CNode::operator=(const CNode& node)
{
    if (this != &node)
    {
        setID(node.getID());
        setProperties(node.getProperties());

        setLabel(node.label());
        setType(node.type());
        setPosAlg(node.posAlg());
        setPosUser(node.posUser());
        setData(node.data());
        setIsVisible(node.IsVisible());
    }
    return *this;
}


bool CNode::operator<(const CNode& variable) const
{
    return (getID() < variable.getID());
}


bool CNode::operator==(const CNode& variable) const
{
    return (getID() == variable.getID());
}


int CNode::nodeID() const
{
    return iNodeID_;
}

void CNode::setNodeID(int iNodeID)
{
    iNodeID_ = iNodeID;
}

QString CNode::label() const
{
    return strLabel_;
}

void CNode::setLabel(const QString &strLabel)
{
    strLabel_ = strLabel;
}

QPointF CNode::posAlg() const
{
    return pntPosAlg_;
}

void CNode::setPosAlg(const QPointF &pntPosAlg)
{
    pntPosAlg_ = pntPosAlg;
}

QPointF CNode::posUser() const
{
    return pntPosUser_;
}

void CNode::setPosUser(const QPointF &pntPosUser)
{
    pntPosUser_ = pntPosUser;
}

QVariant CNode::data() const
{
    return data_;
}

void CNode::setData(const QVariant &data)
{
    data_ = data;
}

QString CNode::type() const
{
    return strType_;
}

void CNode::setType(const QString &strType)
{
    strType_ = strType;
}

bool CNode::IsVisible() const
{
    return bIsVisible_;
}

void CNode::setIsVisible(bool bIsVisible)
{
    bIsVisible_ = bIsVisible;
}

QJsonObject CNode::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObject;

    jsonObject["id"] = QString::number(getID());
    jsonObject["nodeID"] = QString::number(nodeID());
    jsonObject["label"] = label();
    jsonObject["pntPosAlgX"] = pntPosAlg_.x();
    jsonObject["pntPosAlgY"] = pntPosAlg_.y();
    jsonObject["pntPosUserX"] = pntPosUser_.x();
    jsonObject["pntPosUserY"] = pntPosUser_.y();
//    jsonObject["data"] = data.toString();
    jsonObject["visible"] = IsVisible();

    if(bIncludeProperties)
    {
        jsonObject["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObject;
}

void CNode::fromJsonObject(const QJsonObject &jsonObject)
{
    /* ID */
    QString strID = jsonObject["id"].toString();
    bool ok;
    quint32 lID = strID.toUInt(&ok, 10);
    if(lID != 0)
    {
        setID(lID);
    }

    /* node id */
    QString strNodeID = jsonObject["nodeID"].toString();
    setNodeID(strNodeID.toInt());

    /* label */
    QString strLabel = jsonObject["label"].toString();
    setLabel(strLabel);

    /* Point pos algo */
    QString strPntPosAlgX = jsonObject["pntPosAlgX"].toString();
    QString strPntPosAlgY = jsonObject["pntPosAlgY"].toString();
    QPointF pntPosAlg(strPntPosAlgX.toDouble(), strPntPosAlgY.toDouble());
    setPosAlg(pntPosAlg);

    QString strPntPosUserX = jsonObject["pntPosUserX"].toString();
    QString strPntPosUserY = jsonObject["pntPosUserY"].toString();
    QPointF pntPosUser(strPntPosUserX.toDouble(), strPntPosUserY.toDouble());
    setPosUser(pntPosUser);

//    jsonObject["data"] = data.toString();
    bool bVisible = jsonObject["visible"].toBool();
    setIsVisible(bVisible);
}

void CNode::update(const CNode &node)
{
    setProperties(node.getProperties());

    setLabel(node.label());
    setType(node.type());
    setPosAlg(node.posAlg());
    setPosUser(node.posUser());
    setData(node.data());
    setIsVisible(node.IsVisible());
}

QDataStream& operator<<(QDataStream& out, const CNode& node)
{
    out << node.getID() << node.getProperties()
        << node.label()
        << node.type()
        << node.posAlg()
        << node.posUser()
        << node.data()
        << node.IsVisible();

    return out;
}

QDataStream& operator>>(QDataStream& in, CNode& node)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;
    QString strLabel;
    QString strType;
    QPointF pntPosAlg;
    QPointF pntPosUser;
    QVariant data;
    bool bIsVisible;

    in >> lID >> mapProperties
       >> strLabel
       >> strType
       >> pntPosAlg
       >> pntPosUser
       >> data
       >> bIsVisible;

    CNode nodeNew;
    nodeNew.setLabel(strLabel);
    nodeNew.setType(strType);
    nodeNew.setPosAlg(pntPosAlg);
    nodeNew.setPosUser(pntPosUser);
    nodeNew.setData(data);
    nodeNew.setIsVisible(bIsVisible);
    nodeNew.setID(lID);
    nodeNew.setProperties(mapProperties);

    node = nodeNew;

    return in;
}

QDebug operator<<(QDebug dbg, const CNode& node)
{
    dbg.nospace()   << g_spacer
                    << "CNode(" << node.getID()
                    << ", " << node.label()
                    << ", " << node.type()
                    << ", " << node.posAlg()
                    << ", " << node.posUser()
                    << ", " << node.data()
                    << ", " << node.IsVisible()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(node.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}
