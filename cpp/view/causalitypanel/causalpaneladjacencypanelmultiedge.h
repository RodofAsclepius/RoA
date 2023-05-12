#ifndef CCAUSALPANELADJACENCYPANELMULTIEDGE_H
#define CCAUSALPANELADJACENCYPANELMULTIEDGE_H

#include "propertiesinterface.h"
#include "groupslist.h"

class CCausalPanelAdjacencyPanelMultiEdge : public CPropertiesInterface
{
    Q_OBJECT
public:
    CCausalPanelAdjacencyPanelMultiEdge();

    CCausalPanelAdjacencyPanelMultiEdge(const QString& strNodeName1, const QString& strNodeName2);
    virtual ~CCausalPanelAdjacencyPanelMultiEdge();

//    CCausalPanelAdjacencyPanelMultiEdge& operator=(const CCausalPanelAdjacencyPanelMultiEdge& variable);
//    virtual bool operator==(const CCausalPanelAdjacencyPanelMultiEdge& variable) const;
//    virtual bool operator<(const CCausalPanelAdjacencyPanelMultiEdge& Variable) const;
//    friend QDataStream& operator>>(QDataStream& in, CCausalPanelAdjacencyPanelMultiEdge& variable);

    void addGroupArrowAlg(const QString& strNodeName1, const QString& strNodeName2, const CGroup& group);
    void addGroupArrowUser(const QString& strNodeName1, const QString& strNodeName2, const CGroup& group);

    void clear();

protected:
    QString strNodeName1_;
    QString strNodeName2_;
    CGroupsList lstGroupsArrowsAlg_;
    CGroupsList lstGroupsArrowsUser_;

    CGroupsList lstGroupsArrowsReverseAlg_;
    CGroupsList lstGroupsArrowsReverseUser_;

//    friend uint qHash(const CCausalPanelAdjacencyPanelMultiEdge &key, uint seed);
};

Q_DECLARE_METATYPE(CCausalPanelAdjacencyPanelMultiEdge)

//QDataStream& operator<<(QDataStream& out, const CCausalPanelAdjacencyPanelMultiEdge& variable);
//QDataStream& operator>>(QDataStream& in, CCausalPanelAdjacencyPanelMultiEdge& variable);
//QDebug operator<<(QDebug dbg, const CCausalPanelAdjacencyPanelMultiEdge& variable);

//inline uint qHash(const CCausalPanelAdjacencyPanelMultiEdge &key, uint seed = 0)
//{
//    Q_UNUSED(seed)

//    return qHash(key.getID());// * key.getName().length();
//}

#endif // CCAUSALPANELADJACENCYPANELMULTIEDGE_H
