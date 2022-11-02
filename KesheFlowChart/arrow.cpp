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

//int Arrow::ReturnStartDirect()
//{

//}

//int Arrow::ReturnEndDirect()
//{

//}

void Arrow::updatePosition()
{
    //QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    QLineF line(myStartItem->pos(),thePoint);
    setLine(line);//需要解释
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
//////////////////////////////////////////////////////////////////////////////////////////
    QLineF centerLine(myStartItem->pos(), myEndItem->pos());//
    QPolygonF endPolygon = myEndItem->polygon();//把图形提取出来
    QPolygonF startPolygon = myStartItem->polygon();
    QPointF p1 = startPolygon.first() + myStartItem->pos();
    //从第一个点开始找，enditem坐标加上第一个点以enditem为00点的-x、-y坐标形成scene下的global坐标
    ///////////////////////////////////////////////////////////////////////////////////////
    QPointF intersectPoint1,intersectPoint2;//交叉点
    int Pcase =0;
    int StartPointcase = 0;
    //////////////////////////////////开始图形的点////////////////////////////////////////
    for (int i = 1; i < startPolygon.count(); ++i) {//每两个点查一次
        QPointF p2 = startPolygon.at(i) + myStartItem->pos();
        QLineF polyLine = QLineF(p1, p2);
        QLineF::IntersectionType intersectionType =
            polyLine.intersects(centerLine, &intersectPoint1);
        if (intersectionType == QLineF::BoundedIntersection){//
            if((p1.x()<(myStartItem->pos().x())) && (p1.y()<=(myStartItem->pos().y()))
                    && (p2.x()<=(myStartItem->pos().x())) && (p2.y()>(myStartItem->pos().y()))){
                StartPointcase =1;//左
            }else if(((p1.x()<=myStartItem->pos().x())) && (p1.y()>(myStartItem->pos().y()))
                     && (p2.x()>(myStartItem->pos().x())) && (p2.y()>=(myStartItem->pos().y()))){
                 StartPointcase =2;
             }else if(((p1.x()>myStartItem->pos().x())) && (p1.y()>=(myStartItem->pos().y()))
                      && (p2.x()>=(myStartItem->pos().x())) && (p2.y()<(myStartItem->pos().y()))){
                  StartPointcase =3;
              }else if(((p1.x()>=myStartItem->pos().x())) && (p1.y()<(myStartItem->pos().y()))
                       && (p2.x()<(myStartItem->pos().x())) && (p2.y()<=(myStartItem->pos().y()))){
                   StartPointcase =4;
               }

        break;//找到
        }
        p1 = p2;
    }
    switch (StartPointcase) {
    case 1:
        aPoint.setX(myStartItem->pos().x()-150);
        aPoint.setY(myStartItem->pos().y());
        break;
    case 2:
        aPoint.setX(myStartItem->pos().x());
        aPoint.setY(myStartItem->pos().y()+150);
        break;
    case 3:
        aPoint.setX(myStartItem->pos().x()+150);
        aPoint.setY(myStartItem->pos().y());
        break;
    case 4:
        aPoint.setX(myStartItem->pos().x());
        aPoint.setY(myStartItem->pos().y()-150);
        break;
    default:
        aPoint.setX(myStartItem->pos().x());
        aPoint.setY(myStartItem->pos().y()+150);
        break;
    }
    ///////////////////////////////////////////////////////////////////////////////////////
    QPointF p3 = endPolygon.first() + myEndItem->pos();
    for (int i = 1; i < endPolygon.count(); ++i) {//每两个点查一次
        QPointF p4 = endPolygon.at(i) + myEndItem->pos();
        QLineF polyLine = QLineF(p3, p4);
        QLineF::IntersectionType intersectionType2 =
            polyLine.intersects(centerLine, &intersectPoint2);
        if (intersectionType2 == QLineF::BoundedIntersection){
            if((p3.x()<(myEndItem->pos().x())) && (p3.y()<=(myEndItem->pos().y()))
                    && (p4.x()<=(myEndItem->pos().x())) && (p4.y()>(myEndItem->pos().y()))){
                Pcase =1;//左
            }else if(((p3.x()<=myEndItem->pos().x())) && (p3.y()>(myEndItem->pos().y()))
                     && (p4.x()>(myEndItem->pos().x())) && (p4.y()>=(myEndItem->pos().y()))){
                 Pcase =2;
             }else if(((p3.x()>myEndItem->pos().x())) && (p3.y()>=(myEndItem->pos().y()))
                      && (p4.x()>=(myEndItem->pos().x())) && (p4.y()<(myEndItem->pos().y()))){
                  Pcase =3;
              }else if(((p3.x()>=myEndItem->pos().x())) && (p3.y()<(myEndItem->pos().y()))
                       && (p4.x()<(myEndItem->pos().x())) && (p4.y()<=(myEndItem->pos().y()))){
                   Pcase =4;
               }

        break;//找到
        }
        p3 = p4;
    }
    switch (Pcase) {
    case 1:
        thePoint.setX(myEndItem->pos().x()-150);
        thePoint.setY(myEndItem->pos().y());
        break;
    case 2:
        thePoint.setX(myEndItem->pos().x());
        thePoint.setY(myEndItem->pos().y()+150);
        break;
    case 3:
        thePoint.setX(myEndItem->pos().x()+150);
        thePoint.setY(myEndItem->pos().y());
        break;
    case 4:
        thePoint.setX(myEndItem->pos().x());
        thePoint.setY(myEndItem->pos().y()-150);
        break;
    default:
        thePoint.setX(myEndItem->pos().x());
        thePoint.setY(myEndItem->pos().y()+150);
        break;
    }
    //QLineF intersectLine = QLineF(p1,p2);
    /////////////////////////////////////////////////////////////////////
    setLine(QLineF(thePoint, aPoint));//绘制
    //在这里可以添加arrow数量

//形成箭头的三角形，及线条角度
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
    if (isSelected()) {//选中的边框
        painter->setPen(QPen(myColor, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}
