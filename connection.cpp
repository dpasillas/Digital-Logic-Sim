#include "connection.h"
#include <QPainter>
#include <QGraphicsScene>

Connection::Connection(Pin *s, Pin *d):source(s), destination(d), path(0)
{
    updatePath();
    setZValue(-1);
    setFlags(ItemSendsGeometryChanges | ItemStacksBehindParent);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    s->scene()->addItem(this);
}

Connection::~Connection()
{
    if(path)
        delete path;
    if(source)
        source->connections->removeAll(this);
    if(destination)
        destination->connections->removeAll(this);

    //if(scene())
    //    scene()->removeItem(this);
}

QRectF Connection::boundingRect() const
{
    QRectF rect(path->boundingRect());
    rect.adjust(-2,-2,4,4);
    return rect;
}

QPainterPath Connection::shape() const
{
    return *path;
}

void Connection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(QColor(Qt::black),4);
    QColor color = (source->pinState())?Qt::red:(source->pinValue())?Qt::blue:Qt::white;
    painter->strokePath(*path,pen);
    pen = QPen(color.lighter(128),2);
    painter->strokePath(*path,pen);
}

void Connection::updatePath()
{
    setToolTip(QString("Width: %1").arg(source->pinWidth()));
    if(path)
        delete path;
    path = new QPainterPath();

    if(source->name().size())
    {
        update();
        return;
    }

    QPointF p1, p2, c1, c2, temp;
    QRectF rect = source->boundingRect();

    static const int pd = 60;
    switch(source->dir())
    {
    case LEFT:
        temp = QPointF(rect.left() + 12.5,rect.top() + rect.bottom()/2. + 1.);
        p1 = mapFromItem(source,temp);
        c1 = mapFromItem(source, temp - QPointF(pd,0));
        break;
    case RIGHT:
        temp = QPointF(rect.right() - 22.5,rect.top() + rect.bottom()/2. + 1.);
        p1 = mapFromItem(source,temp);
        c1 = mapFromItem(source, temp + QPointF(pd,0));
        break;
    case UP:
        temp = QPointF(rect.left() + rect.right()/2. + 1,rect.top() + 12.5);
        p1 = mapFromItem(source,temp);
        c1 = mapFromItem(source, temp - QPointF(0,pd));
        break;
    case DOWN:
        temp = QPointF(rect.left() + rect.right()/2. + 1,rect.bottom() - 22.5);
        p1 = mapFromItem(source,rect.left() + rect.right()/2. + 1,rect.bottom() - 22.5);
        c1 = mapFromItem(source, temp + QPointF(0,pd));
    }

    rect = destination->boundingRect();
    switch(destination->dir())
    {
    case LEFT:
        temp = QPointF(rect.left() + 12.5,rect.top() + rect.bottom()/2. + 1.);
        p2 = mapFromItem(destination,temp);
        c2 = mapFromItem(destination,temp - QPointF(pd,0));
        break;
    case RIGHT:
        temp = QPointF(rect.right() - 22.5,rect.top() + rect.bottom()/2. + 1.);
        p2 = mapFromItem(destination,temp);
        c2 = mapFromItem(destination,temp + QPointF(pd,0));
        break;
    case UP:
        temp = QPointF(rect.left() + rect.right()/2. + 1,rect.top() + 12.5);
        p2 = mapFromItem(destination,temp);
        c2 = mapFromItem(destination,temp - QPointF(0,pd));
        break;
    case DOWN:
        temp = QPointF(rect.left() + rect.right()/2. + 1,rect.bottom() - 22.5);
        p2 = mapFromItem(destination,temp);
        c2 = mapFromItem(destination,temp + QPointF(0,pd));
    }

    path->moveTo(p1);
    path->cubicTo(c1,c2,p2);
    path->cubicTo(c2,c1,p1);
    path->addEllipse(p1,1,1);
    path->addEllipse(p2,1,1);
    prepareGeometryChange();

    update();
}
