#include "causalitypanelgraphviewscene.h"
#include <QGraphicsSceneMouseEvent>
#include "utils.h"
#include "causalitypanelgraphview.h"
#include "causalitypanelgrapviewhedge.h"
#include <QKeyEvent>

CCausalityPanelGraphViewScene::CCausalityPanelGraphViewScene(QObject *parent)
    : QGraphicsScene(parent)
{
}

CCausalityPanelGraphViewScene::CCausalityPanelGraphViewScene(const QRectF &sceneRect, QObject *parent)
    : QGraphicsScene(sceneRect, parent)
{

}

CCausalityPanelGraphViewScene::CCausalityPanelGraphViewScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(x, y, width, height, parent)
{

}

CCausalityPanelGraphViewScene::~CCausalityPanelGraphViewScene()
{

}

void CCausalityPanelGraphViewScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CCausalityPanelGraphViewScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void CCausalityPanelGraphViewScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void CCausalityPanelGraphViewScene::keyPressEvent(QKeyEvent *event)
{
    QGraphicsScene::keyPressEvent(event);
}
