#ifndef CEXTENDEDGRAPHICSVIEW_H
#define CEXTENDEDGRAPHICSVIEW_H

#include <QGraphicsView>
#include "extendedgraphicsscene.h"

class CExtendedGraphicsView : public QGraphicsView
{
public:
    CExtendedGraphicsView();
    virtual ~CExtendedGraphicsView();

    void doTranslate(qreal rx, qreal ry);
    void alignOriginLeft();
    void resetZoom();

protected:
    QGraphicsScene *pScene_;
//    bool eventFilter(QObject* object, QEvent* event) override;
};

#endif // CEXTENDEDGRAPHICSVIEW_H
