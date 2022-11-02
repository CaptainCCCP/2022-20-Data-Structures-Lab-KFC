#include "diagramitem.h"
#include "arrow.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
DiagramItem::DiagramItem(DiagramType diagramType,
                         QMenu *myContextMenu,
                         QGraphicsItem *parent)
    :QGraphicsPolygonItem(parent),
      myDiagramType(diagramType),
      myContextMenu(myContextMenu)
{
    QPainterPath path,pathtwo;
    switch (myDiagramType) {
        case StartEnd:
            path.moveTo(90, 0);
            path.arcTo(40, -60, 50, 50, 0, 90);
            path.arcTo(-80, -60, 50, 50, 90, 90);
            path.arcTo(-80, 0, 50, 50, 180, 90);
            path.arcTo(40, 0, 50, 50, 270, 90);
            //path.lineTo(200, 25);
            myPolygon = path.toFillPolygon();
            break;
        case zhuShi:
            pathtwo.moveTo(90, 0);
            pathtwo.arcTo(40, -60, 50, 50, 0, 90);
            pathtwo.arcTo(-80, -60, 50, 50, 90, 90);
            pathtwo.arcTo(-80, 0, 50, 50, 180, 90);
            pathtwo.arcTo(40, 0, 50, 50, 270, 90);
            pathtwo.lineTo(200, 25);
            myPolygon = pathtwo.toFillPolygon();
            break;
        case Conditional:
            myPolygon << QPointF(-100, 0) << QPointF(0, 50)
                      << QPointF(100, 0) << QPointF(0, -50)
                      << QPointF(-100, 0);
            break;
        case Step:
            myPolygon << QPointF(-100, -50) << QPointF(100, -50)
                      << QPointF(100, 50) << QPointF(-100, 50)
                      << QPointF(-100, -50);
            break;
        case Io:
            myPolygon << QPointF(-120, -60) << QPointF(-70, 60)
                      << QPointF(120, 60) << QPointF(70, -60)
                      << QPointF(-120, -60);
            break;
        default:

            break;
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void DiagramItem::removeArrow(Arrow * arrow)
{
    arrows.removeAll(arrow);
}

void DiagramItem::removeArrows()
{
    const auto arrowsCopy = arrows;
    for (Arrow *arrow : arrowsCopy) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void DiagramItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}

QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250,250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black,8));
    painter.translate(125,125);
    painter.drawPolyline(myPolygon);
    return pixmap;
}

void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant DiagramItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        for (Arrow *arrow : qAsConst(arrows))
            arrow->updatePosition();
    }

    return value;
}
