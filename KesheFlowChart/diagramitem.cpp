#include "diagramitem.h"

DiagramItem::DiagramItem(DiagramType diagramType,
                         QMenu *myContextMenu,
                         QGraphicsItem *parent)
    :QGraphicsPolygonItem(parent),
      myDiagramType(diagramType),
      myContextMenu(myContextMenu)
{

}

void DiagramItem::removeArrow(Arrow * arrow){

}

void DiagramItem::removeArrows()
{

}

void DiagramItem::addArrow(Arrow *arrow)
{

}

QPixmap DiagramItem::image() const
{

}

void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

}

QVariant DiagramItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{

}
