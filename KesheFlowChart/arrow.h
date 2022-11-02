#ifndef ARROW_H
#define ARROW_H
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
class DiagramItem;
class Arrow : public QGraphicsLineItem
{
public:
    enum{Type=UserType +4};
    Arrow(DiagramItem *startItem,DiagramItem *endItem,
          QGraphicsSceneMouseEvent *mouseEvent,QGraphicsItem *parent = nullptr);

    int type() const override{return Type;}
    //返回函数
    DiagramItem *startItem()const{return myStartItem;}
    DiagramItem *endItem()const{return myEndItem;}

    void setColor(const QColor &color){myColor = color;}
    void updatePosition();//create line
    //
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

protected://间接调用
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget,QGraphicsSceneMouseEvent *mouseEvent);

private:
    DiagramItem *myStartItem;
    DiagramItem * myEndItem;//起点终点
    QGraphicsSceneMouseEvent *myMouseEvent;
    QPolygonF arrowHead;

    QColor myColor = Qt::black;

};

#endif // ARROW_H
