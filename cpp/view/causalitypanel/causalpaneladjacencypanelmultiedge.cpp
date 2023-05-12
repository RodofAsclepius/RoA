#include "causalpaneladjacencypanelmultiedge.h"

CCausalPanelAdjacencyPanelMultiEdge::CCausalPanelAdjacencyPanelMultiEdge()
{

}

CCausalPanelAdjacencyPanelMultiEdge::~CCausalPanelAdjacencyPanelMultiEdge()
{

}

void CCausalPanelAdjacencyPanelMultiEdge::addGroupArrowAlg(const QString &strNodeName1, const QString &strNodeName2,
                                                           const CGroup &group)
{
    /* Arrow */
    if(strNodeName1 == strNodeName1_ && strNodeName2 == strNodeName2_)
    {
        if(!lstGroupsArrowsAlg_.contains(group))
        {
            lstGroupsArrowsAlg_.append(group);
        }
    }

    /* Reverse arrow */
    if(strNodeName2 == strNodeName1_ && strNodeName1 == strNodeName2_)
    {
        if(!lstGroupsArrowsReverseAlg_.contains(group))
        {
            lstGroupsArrowsReverseAlg_.append(group);
        }
    }
}

void CCausalPanelAdjacencyPanelMultiEdge::addGroupArrowUser(const QString &strNodeName1, const QString &strNodeName2,
                                                            const CGroup &group)
{
    /* Arrow */
    if(strNodeName1 == strNodeName1_ && strNodeName2 == strNodeName2_)
    {
        if(!lstGroupsArrowsUser_.contains(group))
        {
            lstGroupsArrowsUser_.append(group);
        }
    }

    /* Reverse arrow */
    if(strNodeName2 == strNodeName1_ && strNodeName1 == strNodeName2_)
    {
        if(!lstGroupsArrowsReverseUser_.contains(group))
        {
            lstGroupsArrowsReverseUser_.append(group);
        }
    }
}

void CCausalPanelAdjacencyPanelMultiEdge::clear()
{
    lstGroupsArrowsAlg_.clear();
    lstGroupsArrowsUser_.clear();
    lstGroupsArrowsReverseAlg_.clear();
    lstGroupsArrowsReverseUser_.clear();
}
