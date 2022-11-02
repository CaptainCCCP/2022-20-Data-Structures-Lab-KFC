#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H
#include <QGraphicsPolygonItem>
#include "arrow.h"
class DiagramItem : public QGraphicsPolygonItem//QGraphicsItem-QAbstractGraphicsShapeItem-
{
public:

    enum{ Type = UserType+15 };//图形/箭头/
    enum DiagramType{ Step,Conditional,StartEnd,Io,zhuShi };//

    DiagramItem(DiagramType diagramType,QMenu *myContextMenu,QGraphicsItem *parent = nullptr);

    //返回查询函数
    DiagramType diagramType() const{return myDiagramType;}
    QPolygonF polygon() const {return myPolygon;}
    int type()const override{return Type;}
    //箭头部分
    void removeArrow(Arrow *arrow);
    void removeArrows();
    void addArrow(Arrow *arrow);
    //
    QPixmap image()const;//图标

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event)override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value)override;

private:
    //数据部分
    DiagramType myDiagramType;//显示哪种图形
    QPolygonF myPolygon;//点连起来形成哪种形状
    QList<Arrow*>arrows;//保存的所有箭头指针
    QMenu *myContextMenu;// 右键弹出的菜单
};

#endif // DIAGRAMITEM_H
