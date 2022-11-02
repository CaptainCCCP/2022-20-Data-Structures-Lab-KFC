#include "arrow.h"
#include "diagramitem.h"

#include<QPainter>
#include<QPen>
#include<QtMath>

Arrow::Arrow(DiagramItem *startItem, DiagramItem *endItem, QGraphicsItem *parent)
    :myStartItem(startItem),myEndItem(endItem),QGraphicsLineItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable,true);//select
    setPen(QPen(myColor,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));//参数
}

QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);//加上箭头
    return path;
}

void Arrow::updatePosition()
{
    //QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    QLineF line(myStartItem->pos(), myEndItem->pos());
    setLine(line);
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
    if (myStartItem->collidesWithItem(myEndItem))//重叠
        return;
//连线状态的设置
    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setBrush(myColor);

    QLineF centerLine(myStartItem->pos(), myEndItem->pos());//
    QPolygonF endPolygon = myEndItem->polygon();
    QPointF p1 = endPolygon.first() + myEndItem->pos();//点加上
    QPointF intersectPoint;

    for (int i = 1; i < endPolygon.count(); ++i) {//每两个点查一次
        QPointF p2 = endPolygon.at(i) + myEndItem->pos();
        QLineF polyLine = QLineF(p1, p2);
        QLineF::IntersectionType intersectionType =
            polyLine.intersects(centerLine, &intersectPoint);
        if (intersectionType == QLineF::BoundedIntersection)
        break;//找到
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, myStartItem->pos()));//绘制
//算两个点形成箭头的三角形
    double angle = std::atan2(-line().dy(), line().dx());

    QPointF arrowP1 = line().p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                    cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                    cos(angle + M_PI - M_PI / 3) * arrowSize);

    arrowHead.clear();
    arrowHead << line().p1() << arrowP1 << arrowP2;
//选中时的效果
    painter->drawLine(line());
    painter->drawPolygon(arrowHead);
    if (isSelected()) {
        painter->setPen(QPen(myColor, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}
