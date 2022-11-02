#include "diagramscene.h"
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include "arrow.h"
DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    :QGraphicsScene{parent},myItemMenu{itemMenu}
{
    myMode = MoveItem;
    line = nullptr;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
}

void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if(isItemChange(Arrow::Type)){
        Arrow *item = dynamic_cast<Arrow *>(selectedItems().first());
        if(item){
            item->setColor(myLineColor);
        update();
                }
    }
}

void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if(isItemChange(TextDiagramItem::Type)){
        TextDiagramItem *item = dynamic_cast<TextDiagramItem *>(selectedItems().first());
        if(item){
            item->setDefaultTextColor(myTextColor);
        //update();
            }
    }
}

void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if(isItemChange(DiagramItem::Type)){
        DiagramItem *item = dynamic_cast<DiagramItem *>(selectedItems().first());
        if(item){
            item->setBrush(myItemColor);
       // update();
            }
    }
}

void DiagramScene::setFont(const QFont &font)
{
    myFont = font;
    if(isItemChange(TextDiagramItem::Type)){
        QGraphicsTextItem *item = qgraphicsitem_cast<TextDiagramItem *>(selectedItems().first());
        if(item)
            item->setFont(myFont);
    }
}

void DiagramScene::setMode(DiagramScene::Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
    myItemType = type;
}

void DiagramScene::editorLostFocus(TextDiagramItem *item)
{
    QTextCursor cursor = item->textCursor();//不会影响原本的文字内容
    cursor.clearSelection();
    item->setTextCursor(cursor);
    if(item->toPlainText().isEmpty()){//文本为空
        removeItem(item);
        item->deleteLater();
    }
}
//
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;//不是右键就退出
    switch (myMode){//控制生命周期
    case InsertItem:{
        DiagramItem *item = new DiagramItem(myItemType, myItemMenu);
        item->setBrush(myItemColor);
        addItem(item);//加到scene里
        item->setPos(mouseEvent->scenePos());
        emit itemInserted(item);
        break;
    }
    case InsertLine:
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                    mouseEvent->scenePos()));
        line->setPen(QPen(myLineColor, 2));
        addItem(line);
        break;
    case InsertText:{
        TextDiagramItem *textItem = new TextDiagramItem();
        textItem->setFont(myFont);
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);//编辑状态 鼠标键盘
        textItem->setZValue(1000.0);//非常高

        connect(textItem, &TextDiagramItem::lostFocus,this, &DiagramScene::editorLostFocus);
        connect(textItem, &TextDiagramItem::selectedChange,this, &DiagramScene::itemSelected);
        //信号与槽将item scene window连接起来
        addItem(textItem);
        textItem->setDefaultTextColor(myTextColor);
        textItem->setPos(mouseEvent->scenePos());
        emit textInserted(textItem);//建立完成了
        break;
    }
    default:
        ;
    }

}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != nullptr) {//已存在
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);//这线会删
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);//qt提供的
    }
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != nullptr && myMode == InsertLine) {
        //已经画了
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();//这样处理之后后面才能获取到点
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();
        //起点终点
        removeItem(line);
        delete line;//删掉已有的线

        if (startItems.count() > 0 && endItems.count() > 0 &&//确实有点可以连
            startItems.first()->type() == DiagramItem::Type &&//不是叠着的
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first()) {//不是同一个点

            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());

            Arrow *arrow = new Arrow(startItem, endItem);//添加箭头
            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);

            addItem(arrow);//添加到scene中
            arrow->updatePosition();
        }
    }
    line = nullptr;//删掉轨迹线
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool DiagramScene::isItemChange(int type) const
{
    const QList<QGraphicsItem*>items = selectedItems();
    auto lambdaObj=[type](const QGraphicsItem*item){return item->type()==type;};
    return std::find_if(items.begin(),items.end(),lambdaObj)!=items.end();
}
