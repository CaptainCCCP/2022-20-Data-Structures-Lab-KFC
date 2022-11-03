#ifndef ARROW_H
#define ARROW_H
#include <QGraphicsLineItem>
#include <QtMath>

class DiagramItem;
class Arrow : public QGraphicsLineItem
{
private:
    DiagramItem *myStartItem;
    DiagramItem * myEndItem;//起点终点图形
    QPolygonF arrowHead;
    QColor myColor = Qt::black;
    //
    int startDirect,EndDirect;
    QPointF aPoint,thePoint;
    QList<QPointF> PointThrough;

protected://间接调用
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget)override;
public:
    enum{Type=UserType +4};
    Arrow(DiagramItem *startItem,DiagramItem *endItem,QGraphicsItem *parent = nullptr);

    ////////////////////////////////////////////////////
    int type() const override{return Type;}
    //返回函数
    DiagramItem *startItem()const{return myStartItem;}
    DiagramItem *endItem()const{return myEndItem;}

    void setColor(const QColor &color){myColor = color;}
    void updatePosition();//create line
    //
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    //判断出入方向
    int ReturnStartDirect();
    int ReturnEndDirect();

};

#endif // ARROW_H
