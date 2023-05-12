#include "edge.h"

#include "utils.h"

using namespace utils;
#include <QPair>

CEdge::CEdge()
{
    qRegisterMetaType<CEdge>("CEdge");
//    qRegisterMetaTypeStreamOperators<CEdge>("CEdge");
}

CEdge::CEdge(const CNode &nodeSource, const CNode &nodeTarget, const QString &strLabel)
    :CEdge()
{
    setNodes(QPair<CNode, CNode>(nodeSource, nodeTarget));
    setLabel(strLabel);
}

CEdge::CEdge(const CEdge& edge)
: CEdge()
{
this->operator =(edge);
}


CEdge::~CEdge()
{
}

CEdge& CEdge::operator=(const CEdge& edge)
{
    if (this != &edge)
    {
        setID(edge.getID());
        setProperties(edge.getProperties());

        setNodes(edge.nodes());
        setLabel(edge.label());
        setType(edge.type());
        setEndPointsAlg(edge.endPointsAlg());
        setControlPointsAlg(edge.controlPointsAlg());
        setEndPointsUser(edge.endPointsUser());
        setControlPointsUser(edge.controlPointsUser());
        setData(edge.data());
        setVisible(edge.isVisible());
    }
    return *this;
}


bool CEdge::operator<(const CEdge& edge) const
{
    return (getID() < edge.getID());
}


bool CEdge::operator==(const CEdge& variable) const
{
    return (getID() == variable.getID());
}


int CEdge::edgeID() const
{
    return iEdgeID_;
}

void CEdge::setEdgeID(int iEdgeID)
{
    iEdgeID_ = iEdgeID;
}

QString CEdge::label() const
{
    return strLabel_;
}

void CEdge::setLabel(const QString &strLabel)
{
    strLabel_ = strLabel;
}

QVariant CEdge::data() const
{
    return data_;
}

void CEdge::setData(const QVariant &data)
{
    data_ = data;
}

CNode CEdge::sourceNode() const
{
    return pairNodes_.first;
}

void CEdge::setSourceNode(const CNode& sourceNode)
{
    pairNodes_.first = sourceNode;
}

CNode CEdge::targetNode() const
{
    return pairNodes_.second;
}

void CEdge::setTargetNode(const CNode& targetNode)
{
    pairNodes_.second = targetNode;
}

QString CEdge::type() const
{
    return strType_;
}

void CEdge::setType(const QString &strType)
{
    strType_ = strType;
}

QPair<QPointF, QPointF> CEdge::endPointsAlg() const
{
    return pairEndPointsAlg_;
}

void CEdge::setEndPointsAlg(const QPair<QPointF, QPointF> &pairEndPointsAlg)
{
    pairEndPointsAlg_ = pairEndPointsAlg;
}

QList<QPointF> CEdge::controlPointsAlg() const
{
    return lstControlPointsAlg_;
}

void CEdge::setControlPointsAlg(const QList<QPointF> &lstControlPointsAlg)
{
    lstControlPointsAlg_ = lstControlPointsAlg;
}

QPair<QPointF, QPointF> CEdge::endPointsUser() const
{
    return pairEndPointsUser_;
}

void CEdge::setEndPointsUser(const QPair<QPointF, QPointF> &pairEndPointsUser)
{
    pairEndPointsUser_ = pairEndPointsUser;
}

QList<QPointF> CEdge::controlPointsUser() const
{
    return lstControlPointsUser_;
}

void CEdge::setControlPointsUser(const QList<QPointF> &lstControlPointsUser)
{
    lstControlPointsUser_ = lstControlPointsUser;
}

bool CEdge::isVisible() const
{
    return bIsVisible_;
}

void CEdge::setVisible(bool bIsVisible)
{
    bIsVisible_ = bIsVisible;
}

QPair<CNode, CNode> CEdge::nodes() const
{
    return pairNodes_;
}

void CEdge::setNodes(const QPair<CNode, CNode> &pairNodes)
{
    pairNodes_ = pairNodes;
}

QJsonObject CEdge::toJsonObject(bool bIncludeProperties) const
{
    QJsonObject jsonObject;

    jsonObject["id"] = QString::number(getID());
    jsonObject["edgeID"] = QString::number(edgeID());
    jsonObject["firstNode"] = nodes().first.toJsonObject();
    jsonObject["secondNode"] = nodes().second.toJsonObject();
    jsonObject["label"] = label();
    jsonObject["type"] = type();

    /* Points alg */
    jsonObject["fistEndPointAlg"] = pointToJsonObject(endPointsAlg().first);
    jsonObject["secondEndPointAlg"] = pointToJsonObject(endPointsAlg().second);

    QJsonArray arrPoints;
    foreach(QPointF pnt, controlPointsAlg())
    {
        arrPoints.append(pointToJsonObject(pnt));
    }
    jsonObject["controlPointsAlg"] = arrPoints;

    /* Points user */
    jsonObject["fistEndPointUser"] = pointToJsonObject(endPointsUser().first);
    jsonObject["secondEndPointUser"] = pointToJsonObject(endPointsUser().second);

    QJsonArray arrPointsUser;
    foreach(QPointF pnt, controlPointsUser())
    {
        arrPointsUser.append(pointToJsonObject(pnt));
    }
    jsonObject["controlPointsUser"] = arrPointsUser;

    /* Visible */
    jsonObject["visisble"] = isVisible();

    /* Properties */
    if(bIncludeProperties)
    {
        jsonObject["properties"] = VariantMaptoJsonObject(_mapProperties);
    }

    return jsonObject;
}

void CEdge::fromJsonObject(const QJsonObject &jsonObject)
{
    /* ID */
    QString strID = jsonObject["id"].toString();
    bool ok;
    quint32 lID = strID.toUInt(&ok, 10);
    if(lID != 0)
    {
        setID(lID);
    }

    /* Edge ID */
    QString strEdgeID = jsonObject["edgeID"].toString();
    setEdgeID(strEdgeID.toInt());

    /* Label */
    QString strLabel = jsonObject["label"].toString();
    setLabel(strLabel);

    /* Type */
    QString strType = jsonObject["type"].toString();
    setType(strType);

    /* Nodes */
    CNode firstNode;
    CNode secondNode;
    QJsonValue valFirstNode = jsonObject["firstNode"];
    if(valFirstNode.isObject())
    {
        firstNode.fromJsonObject(valFirstNode.toObject());
    }
    QJsonValue valSecondNode = jsonObject["secondNode"];
    if(valSecondNode.isObject())
    {
        secondNode.fromJsonObject(valSecondNode.toObject());
    }
    setNodes(QPair<CNode, CNode>(firstNode, secondNode));

    /* End points alg */
    QPointF pntFirstEndPointAlg;
    QPointF pntSecondEndPointAlg;
    QJsonValue valFirstPointAlg = jsonObject["fistEndPointAlg"];
    if(valFirstPointAlg.isObject())
    {
        pntFirstEndPointAlg = pointFromJsonObject(valFirstPointAlg.toObject());
    }
    QJsonValue valSecondtPointAlg = jsonObject["secondEndPointAlg"];
    if(valSecondtPointAlg.isObject())
    {
        pntSecondEndPointAlg = pointFromJsonObject(valSecondtPointAlg.toObject());
    }
    setEndPointsAlg(QPair<QPointF, QPointF>(pntFirstEndPointAlg, pntSecondEndPointAlg));

    /* Control points alg */
    QList<QPointF> lstControlPointsAlg;
    QJsonValue valControlPointsAlg = jsonObject["controlPointsAlg"];
    if(valControlPointsAlg.isArray())
    {
        QJsonArray arrPoints = valControlPointsAlg.toArray();
        foreach(QJsonValue valPoint, arrPoints)
        {
            lstControlPointsAlg.append(pointFromJsonObject(valPoint.toObject()));
        }
    }
    setControlPointsAlg(lstControlPointsAlg);

    /* End points user */
    QPointF pntFirstEndPointUser;
    QPointF pntSecondEndPointUser;
    QJsonValue valFirstPointUser = jsonObject["fistEndPointUser"];
    if(valFirstPointUser.isObject())
    {
        pntFirstEndPointUser = pointFromJsonObject(valFirstPointUser.toObject());
    }
    QJsonValue valSecondtPointUser = jsonObject["secondEndPointUser"];
    if(valSecondtPointUser.isObject())
    {
        pntSecondEndPointUser = pointFromJsonObject(valSecondtPointUser.toObject());
    }
    setEndPointsUser(QPair<QPointF, QPointF>(pntFirstEndPointUser, pntSecondEndPointUser));

    /* Control points alg */
    QList<QPointF> lstControlPointsUser;
    QJsonValue valControlPointsUser = jsonObject["controlPointsUser"];
    if(valControlPointsUser.isArray())
    {
        QJsonArray arrPoints = valControlPointsUser.toArray();
        foreach(QJsonValue valPoint, arrPoints)
        {
            lstControlPointsUser.append(pointFromJsonObject(valPoint.toObject()));
        }
    }
    setControlPointsUser(lstControlPointsUser);

    /* Visible */
    bool bVisible = jsonObject["visisble"].toBool();
    setVisible(bVisible);

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

void CEdge::update(const CEdge &edge)
{
    setProperties(edge.getProperties());

    setNodes(edge.nodes());
    setLabel(edge.label());
    setType(edge.type());
    setEndPointsAlg(edge.endPointsAlg());
    setControlPointsAlg(edge.controlPointsAlg());
    setEndPointsUser(edge.endPointsUser());
    setControlPointsUser(edge.controlPointsUser());
    setData(edge.data());
    setVisible(edge.isVisible());
}

QJsonObject CEdge::pointToJsonObject(const QPointF &pnt) const
{
    QJsonObject obj;
    obj["x"] = pnt.x();
    obj["y"] = pnt.y();
    return obj;
}

QPointF CEdge::pointFromJsonObject(const QJsonObject &objPoint) const
{
    QPointF pnt;

    qreal x = objPoint["x"].toDouble();
    qreal y = objPoint["y"].toDouble();

    pnt.setX(x);
    pnt.setY(y);

    return pnt;
}

QDataStream& operator<<(QDataStream& out, const CEdge& edge)
{
    out << edge.getID() << edge.getProperties()
        << edge.nodes()
        << edge.label()
        << edge.type()
        << edge.endPointsAlg()
        << edge.controlPointsAlg()
        << edge.endPointsUser()
        << edge.controlPointsUser()
        << edge.data()
        << edge.isVisible();

        return out;
}

QDataStream& operator>>(QDataStream& in, CEdge& edge)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;

    QPair<CNode, CNode> pairNodes;

    QString strLabel;
    QString strType;
    QPair<QPointF, QPointF> pairEndPointsAlg;
    QList<QPointF> lstControlPointsAlg;
    QPair<QPointF, QPointF> pairEndPointsUser;
    QList<QPointF> lstControlPointsUser;

    QVariant data;
    bool bIsVisible;

    in >> lID >> mapProperties
       >> pairNodes
       >> strLabel
       >> strType
       >> pairEndPointsAlg
       >> lstControlPointsAlg
       >> pairEndPointsUser
       >> lstControlPointsUser
       >> data
       >> bIsVisible;

    CEdge edgeNew;
    edgeNew.setNodes(pairNodes);
    edgeNew.setLabel(edge.label());
    edgeNew.setType(edge.type());
    edgeNew.setEndPointsAlg(edge.endPointsAlg());
    edgeNew.setControlPointsAlg(edge.controlPointsAlg());
    edgeNew.setEndPointsUser(edge.endPointsUser());
    edgeNew.setControlPointsUser(edge.controlPointsUser());
    edgeNew.setData(edge.data());
    edgeNew.setVisible(bIsVisible);

    edge = edgeNew;

    return in;
}


QDebug operator<<(QDebug dbg, const CEdge& edge)
{
    dbg.nospace()   << g_spacer
                    << "CEdge(" << edge.getID()
                    << ", " << edge.nodes()
                    << ", " << edge.label()
                    << ", " << edge.type()
                    << ", " << edge.endPointsAlg()
                    << ", " << edge.controlPointsAlg()
                    << ", " << edge.endPointsUser()
                    << ", " << edge.controlPointsUser()
                    << ", " << edge.data()
                    << ", " << edge.isVisible()
                    << ")" << "\n";

    g_spacer.indent();
    dbg.nospace() << g_spacer << "Properties:" << "\n";
    QMapIterator<QString, QVariant> mapIterator(edge.getProperties());
    while (mapIterator.hasNext())
    {
          mapIterator.next();
          dbg.nospace() << g_spacer << mapIterator.key() << " " << mapIterator.value() << "\n";
    }
    g_spacer.unindent();

    return dbg.maybeSpace();
}
