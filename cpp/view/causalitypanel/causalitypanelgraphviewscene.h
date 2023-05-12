#ifndef CAUSALITYPANELGRAPHVIEWSCENE_H
#define CAUSALITYPANELGRAPHVIEWSCENE_H

#include <QGraphicsScene>

class CCausalityPanelGraphViewScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CCausalityPanelGraphViewScene(QObject *parent = nullptr);
    CCausalityPanelGraphViewScene(const QRectF &sceneRect, QObject *parent = nullptr);
    CCausalityPanelGraphViewScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = nullptr);
    virtual ~CCausalityPanelGraphViewScene();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
signals:

};

#endif // CAUSALITYPANELGRAPHVIEWSCENE_H
