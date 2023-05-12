#include "multiedge.h"

#include "utils.h"

using namespace utils;

CMultiEdge::CMultiEdge()
{
    qRegisterMetaType<CMultiEdge>("CMultiEdge");
//    qRegisterMetaTypeStreamOperators<CMultiEdge>("CMultiEdge");
}

CMultiEdge::CMultiEdge(const CMultiEdge& edge)
: CMultiEdge()
{
this->operator =(edge);
}


CMultiEdge::~CMultiEdge()
{
}

CMultiEdge& CMultiEdge::operator=(const CMultiEdge& multiEdge)
{
    if (this != &multiEdge)
    {
        setID(multiEdge.getID());
        setProperties(multiEdge.getProperties());

        setFirstNode(multiEdge.firstNode());
        setSecondNode(multiEdge.secondNode());

        setEdges(multiEdge.edges());

        setDataFirstEdge(multiEdge.dataFirstEdge());
        setDataSecondEdge(multiEdge.dataSecondEdge());

        setIsVisible(multiEdge.isVisible());

    }
    return *this;
}


bool CMultiEdge::operator<(const CMultiEdge& edge) const
{
    return (getID() < edge.getID());
}


bool CMultiEdge::operator==(const CMultiEdge& variable) const
{
    return (getID() == variable.getID());
}


bool CMultiEdge::isVisible() const
{
    return bIsVisible_;
}

void CMultiEdge::setIsVisible(bool bIsVisible)
{
    bIsVisible_ = bIsVisible;
}

QVariant CMultiEdge::dataFirstEdge() const
{
    return dataFirstEdge_;
}

void CMultiEdge::setDataFirstEdge(const QVariant &dataFirstEdge)
{
    dataFirstEdge_ = dataFirstEdge;
}

QVariant CMultiEdge::dataSecondEdge() const
{
    return dataSecondEdge_;
}

void CMultiEdge::setDataSecondEdge(const QVariant &dataSecondEdge)
{
    dataSecondEdge_ = dataSecondEdge;
}

bool CMultiEdge::belongs(const CEdge &edge) const
{
    if ( (edge.sourceNode() == firstNode() && edge.targetNode() == secondNode() ) ||
         (edge.sourceNode() == secondNode() && edge.targetNode() == firstNode() ) )
    {
        return true;
    }

    return false;
}

QList<CEdge> CMultiEdge::edges() const
{
    return lstEdges_;
}

void CMultiEdge::setEdges(const QList<CEdge> &value)
{
    lstEdges_ = value;
}

void CMultiEdge::addEdge(const CEdge &edge, bool bAllowDuplicates)
{
    if(bAllowDuplicates)
    {
        lstEdges_.append(edge);
    }
    else
    {
        if(!lstEdges_.contains(edge))
        {
            lstEdges_.append(edge);
        }
    }
}

bool CMultiEdge::contains(const CEdge &aEdge) const
{
    foreach(CEdge edge, lstEdges_)
    {
        if(edge.sourceNode() == aEdge.sourceNode() && edge.targetNode() == aEdge.targetNode())
        {
            return true;
        }
    }

    return false;
}

CNode CMultiEdge::firstNode() const
{
    return firstNode_;
}

void CMultiEdge::setFirstNode(const CNode &firstNode)
{
    firstNode_ = firstNode;
}

CNode CMultiEdge::secondNode() const
{
    return secondNode_;
}

void CMultiEdge::setSecondNode(const CNode &secondNode)
{
    secondNode_ = secondNode;
}


QDataStream& operator<<(QDataStream& out, const CMultiEdge& multiEdge)
{
    out << multiEdge.getID() << multiEdge.getProperties()
        << multiEdge.firstNode()
        << multiEdge.secondNode()
        << multiEdge.edges()
        << multiEdge.dataFirstEdge()
        << multiEdge.dataSecondEdge()
        << multiEdge.isVisible();

    return out;
}

QDataStream& operator>>(QDataStream& in, CMultiEdge& multiEdge)
{
    quint32 lID;
    QMap<QString, QVariant> mapProperties;
    CNode firstNode;
    CNode secondNode;

    QList<CEdge> lstEdges;

    QVariant dataFirstEdge;
    QVariant dataSecondEdge;
    bool bIsVisible;

    in >> lID >> mapProperties
       >> firstNode
       >> secondNode
       >> lstEdges
       >> dataFirstEdge
       >> dataSecondEdge
       >> bIsVisible;

    CMultiEdge multiEdgeNew;

    multiEdgeNew.setFirstNode(firstNode);
    multiEdgeNew.setSecondNode(secondNode);

    multiEdgeNew.setEdges(lstEdges);

    multiEdgeNew.setDataFirstEdge(dataFirstEdge);
    multiEdgeNew.setDataSecondEdge(dataSecondEdge);

    multiEdge = multiEdgeNew;

    return in;
}


QDebug operator<<(QDebug dbg, const CMultiEdge& edge)
{
    dbg.nospace()   << g_spacer
                    << "CMultiEdge(" << edge.getID()
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
