#ifndef CEXTENDEDGRAPHICSSCENE_H
#define CEXTENDEDGRAPHICSSCENE_H

#include <QGraphicsScene>

class CExtendedGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

    CExtendedGraphicsScene(QObject *parent = nullptr);
    CExtendedGraphicsScene(const QRectF &sceneRect, QObject *parent = nullptr);
    CExtendedGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = nullptr);
    virtual ~CExtendedGraphicsScene();

//    void editorLostFocus(DiagramTextItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
//    bool isItemChange(int type) const;

    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
};

#endif // CEXTENDEDGRAPHICSSCENE_H
