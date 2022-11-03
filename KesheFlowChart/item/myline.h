#ifndef MYLINE_H
#define MYLINE_H
#include "diagramitem.h"
#include <QtMath>
class myLine
{
    Q_OBJECT
private:
    QPoint startPoint;
    QPoint endPoint;
    DiagramItem *StartItem;
    DiagramItem *EndItem;
    ////////////////////////////////////////////////////////////////
    virtual void paintSizeEdge(QPainter & p){}
    virtual void updateScene(){}
    virtual void specialWidgetUpdate(int &x,int &y, int &w, int &h);
    virtual void specialPaintUpdate(QPoint &s, QPoint &e);
public:

};

#endif // MYLINE_H
