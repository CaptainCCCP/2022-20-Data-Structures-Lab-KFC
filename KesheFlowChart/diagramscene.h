#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H
#include "diagramitem.h"
#include "textdiagramitem.h"
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };//事件状态
    explicit DiagramScene(QMenu *itemMenu, QObject *parent = nullptr);//隐式调用获取menu

    //关于字体相关的函数
    QFont font() const { return myFont; }
    QColor textColor() const { return myTextColor; }
    QColor itemColor() const { return myItemColor; }
    QColor lineColor() const { return myLineColor; }
    void setLineColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setItemColor(const QColor &color);
    void setFont(const QFont &font);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    //鼠标事件
private:
    bool isItemChange(int type) const;//是否是选中
    //basic
    Mode myMode;
    DiagramItem::DiagramType myItemType;
    QMenu *myItemMenu;//右键出现的菜单
    QGraphicsLineItem *line;//点击时的线,release就删除
    //字体
    QFont myFont;
    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;
    //temp
    bool leftButtonDown;
    QPointF startPoint;
    TextDiagramItem *textItem;
//
public slots:
    void setMode(Mode mode);//移动/画线
    void setItemType(DiagramItem::DiagramType type);//图形类型
    void editorLostFocus(TextDiagramItem *item);//编辑时删除

signals:
    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);
};

#endif
