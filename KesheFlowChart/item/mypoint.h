#ifndef MYPOINT_H
#define MYPOINT_H
#include "diagramitem.h"

class myPoint:public DiagramItem
{
private:
    virtual void paintChart(QPainter & p);
public:

};

#endif // MYPOINT_H
