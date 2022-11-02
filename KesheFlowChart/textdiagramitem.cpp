#include "textdiagramitem.h"

TextDiagramItem::TextDiagramItem(QGraphicsTextItem *parent)
    :QGraphicsTextItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void TextDiagramItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::TextEditorInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

void TextDiagramItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(textInteractionFlags()==Qt::NoTextInteraction)
         setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

QVariant TextDiagramItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemSelectedHasChanged)
         emit selectedChange(this);
    return value;
}
