#ifndef TEXTDIAGRAMITEM_H
#define TEXTDIAGRAMITEM_H
#include<QGraphicsTextItem>

class TextDiagramItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    enum{Type = UserType +3 };
    TextDiagramItem(QGraphicsTextItem* parent = nullptr);

    int type() const override{return Type;}
signals:
    void lostFocus(TextDiagramItem *item);
    void selectedChange(QGraphicsItem *item);
    // QGraphicsItem interface
protected:
    void focusOutEvent(QFocusEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value)override;
//双击、取消、切换
};

#endif // TEXTDIAGRAMITEM_H
