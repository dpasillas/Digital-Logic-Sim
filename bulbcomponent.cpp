#include "bulbcomponent.h"
#include "math.h"
#include <QPropertyAnimation>

QPainterPath* BulbComponent::bulbPath = 0;

BulbComponent::BulbComponent(CircuitBoard *board):OperatingComponent(board),
    on(false)
{
    if(!bulbPath)
    {
        double PI = acos(-1), angle = PI / 4., cX1 ,cX2, cX3, cY1, cY2, cY3, x1, x2, y1, y2, slope;
        bulbPath = new QPainterPath();
        //bulbPath->addEllipse(QPointF(16,16),16,16);
        bulbPath->moveTo(16 + 16 * cos(angle),16 + 16 * sin(angle));
        bulbPath->arcTo(0,0,32,32,-45,270);

        cX1 = 16 + 16 * (cos(PI + angle + PI/9.));
        cY1 = 16 + 16 * sin(angle) + cX1 - (16 + 16 * cos(PI + angle));
        x1 = 16 + 16 * cos(PI + angle + PI/7.);
        y1 = 36;
        bulbPath->quadTo(cX1,cY1,x1,y1);
        slope = (y1 - cY1)/(x1 - cX1);
        x2 = x1 + 1;
        y2 = 40;
        cX2 = x2 - .5;
        cY2 = y1 + slope * (cX2 - x1);
        bulbPath->quadTo(cX2,cY2,x2,y2);
        slope = (y2 - cY2)/(x2 - cX2);
        cX3 = x2 + .5;
        cY3 = y2 + slope * (cX3 - x2);
        bulbPath->cubicTo(cX3, cY3, 32 - cX3, cY3, 32 - x2, y2);
        bulbPath->quadTo(32-cX2,cY2,32-x1,y1);
        bulbPath->quadTo(32-cX1,cY1, 16 + 16 * cos(angle), 16 + 16 * sin(angle));
    }
    path = bulbPath;

    input.append(new Pin(DOWN,Pin::INPUT,false,this));
    input.at(0)->translate(16,path->boundingRect().bottom() - 10);
    input.at(0)->updatePath();

    gItem = new GradientItem(this);
    gItem->moveBy(16,16);
    gItem->hide();

    setZValue(9);
}

BulbComponent::~BulbComponent()
{
    destructFunc();
}

QRectF BulbComponent::boundingRect() const
{
    return path->boundingRect().adjusted(-2,-2,4,4);
}

QPainterPath BulbComponent::shape() const
{
    return (invisible)?QPainterPath():*path;
}

void BulbComponent::customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //OperatingComponent::paint(painter,option,widget);
    painter->drawPath(*path);

}

void BulbComponent::operate(Pin *source)
{
    Pin* pin = input.at(0);
    on = (!pin->pinState() && pin->pinValue());
    if(on)
    {
        if(!gItem->isVisible())
            gItem->show();
    }
    else
        gItem->hide();
}
