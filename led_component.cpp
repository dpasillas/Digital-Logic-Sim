#include "led_component.h"

QList<QList<QPainterPath> > LED_Component::segPrototypes;

LED_Component::LED_Component(CircuitBoard *board, LED_Type lt) : OperatingComponent(board), ledType(lt)
{
    path = new QPainterPath();
    path->addRect(0,0,32,55);

    QMatrix m;
    m.shear(-1./6.,0); //16.666% shear to the left, from the top down
    QRectF rect = m.map(*path).boundingRect();
    *path = QPainterPath();
    path->addRect(rect.adjusted(-rect.left() - 2, -2, -rect.left() + 2, 2));

    if(segPrototypes.isEmpty())
    {
        for(int i = 0; i < 3; i++)
            segPrototypes.append(QList<QPainterPath>());

        createSegs();

        for(int i = 0; i < 7; i++)
        {
            segPrototypes[LED_7][i].translate(-rect.x() + .5,.5);
            segPrototypes[LED_7][i] = m.map(segPrototypes[LED_7][i]);
        }
        for(int i = 0; i < 14; i++)
        {
            segPrototypes[LED_14][i].translate(-rect.x() + .5,.5);
            segPrototypes[LED_14][i] = m.map(segPrototypes[LED_14][i]);
        }
        for(int i = 0; i < 16; i++)
        {
            segPrototypes[LED_16][i].translate(-rect.x() + .5,.5);
            segPrototypes[LED_16][i] = m.map(segPrototypes[LED_16][i]);
        }
    }

    switch(lt)
    {
    case LED_7:
        for(int i = 0; i < 7; i++)
        {
            segments.append(QColor(223,223,223));
            input.append(new Pin(LEFT,Pin::INPUT,false,this));
            input[i]->moveBy(0,4 + 47./6. * i);
            input[i]->updatePath();
        }
        break;
    case LED_14:
        for(int i = 0; i < 7; i++)
        {
            segments.append(QColor(223,223,223));
            input.append(new Pin(LEFT,Pin::INPUT,false,this));
            input[i]->moveBy(0,4 + 47./6. * i);
            input[i]->updatePath();
        }
        for(int i = 7; i < 14; i++)
        {
            segments.append(QColor(223,223,223));
            input.append(new Pin(RIGHT,Pin::INPUT,false,this));
            input[i]->moveBy(rect.width(),51 - 47./6. * (i - 7));
            input[i]->updatePath();
        }
        break;
    case LED_16:
        for(int i = 0; i < 7; i++)
        {
            segments.append(QColor(191,191,191));
            input.append(new Pin(LEFT,Pin::INPUT,false,this));
            input[i]->moveBy(0,4 + 47./6. * i);
            input[i]->updatePath();
        }
        for(int i = 7; i < 9; i++)
        {
            segments.append(QColor(191,191,191));
            input.append(new Pin(DOWN,Pin::INPUT,false,this));
            double w = rect.width();
            input[i]->moveBy( w/3. + (i-7) * w/3.,57);
            input[i]->updatePath();
        }
        for(int i = 9; i < 16; i++)
        {
            segments.append(QColor(191,191,191));
            input.append(new Pin(RIGHT,Pin::INPUT,false,this));
            input[i]->moveBy(rect.width(),51 - 47./6. * (i - 9));
            input[i]->updatePath();
        }
        break;
    default:
        ;
    }
    /*invisible = true;
    foreach(Pin* pin, input)
    {
        pin->updatePath();
    }*/
}

LED_Component::~LED_Component()
{
    destructFunc();
}

QRectF LED_Component::boundingRect() const
{
    return path->boundingRect();
}

QPainterPath LED_Component::shape() const
{
    return (invisible)?QPainterPath():*path;
}

void LED_Component::customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //OperatingComponent::paint(painter,option,widget);
    painter->drawPath(*path);
    for(int i = 0; i < segments.count(); i++)
    {
        QColor color(segments[i]);
        if(isSelected())
            color.setAlpha(64);
        painter->fillPath(segPrototypes[ledType][i],color);
    }
}

void LED_Component::operate(Pin *source)
{
    for(int i = 0; i < segments.count(); i++)
        segments[i] = (!input[i]->pinState())?
                        (input[i]->pinValue())?
                            QColor(255,0,0)
                        :QColor(191,191,191)
                      :QColor(191,191,191);
    update();
}

void LED_Component::createSegs()
{
    QPainterPath segPath;
    //start 7-seg-1
    segPath.moveTo(6,1);
    segPath.lineTo(25,1);
    segPath.lineTo(27.5,3.5);
    segPath.lineTo(24,7);
    segPath.lineTo(7,7);
    segPath.lineTo(3.5,3.5);
    segPath.closeSubpath();
    segPrototypes[LED_7].append(segPath);
    //end 7-seg-1

    //start 7-seg-2
    segPath = QPainterPath();
    segPath.moveTo(30,6);
    segPath.lineTo(30,25);
    segPath.lineTo(27,27);
    segPath.lineTo(24,24);
    segPath.lineTo(24,7);
    segPath.lineTo(27.5,3.5);
    segPath.closeSubpath();
    segPrototypes[LED_7].append(segPath);
    //end 7-seg-2

    //start 7-seg-3
    segPath = QPainterPath();
    segPath.moveTo(30,30);
    segPath.lineTo(30,48);
    segPath.lineTo(27.5,50.5);
    segPath.lineTo(24,47);
    segPath.lineTo(24,30);
    segPath.lineTo(27,27);
    segPath.closeSubpath();
    segPrototypes[LED_7].append(segPath);
    //end 7-seg-3

    //start 7-seg-4
    segPath = QPainterPath();
    segPath.moveTo(25,53);
    segPath.lineTo(6,53);
    segPath.lineTo(3.5,50.5);
    segPath.lineTo(7,47);
    segPath.lineTo(24,47);
    segPath.lineTo(27.5,50.5);
    segPath.closeSubpath();
    segPrototypes[LED_7].append(segPath);
    //end 7-seg-4

    //start 7-seg-5
    segPath = QPainterPath();
    segPath.moveTo(1,48);
    segPath.lineTo(1,30);
    segPath.lineTo(4,27);
    segPath.lineTo(7,30);
    segPath.lineTo(7,47);
    segPath.lineTo(3.5,50.5);
    segPath.closeSubpath();
    segPrototypes[LED_7].append(segPath);
    //end 7-seg-5

    //start 7-seg-6
    segPath = QPainterPath();
    segPath.moveTo(1,24);
    segPath.lineTo(1,6);
    segPath.lineTo(3.5,3.5);
    segPath.lineTo(7,7);
    segPath.lineTo(7,24);
    segPath.lineTo(4,27);
    segPath.closeSubpath();
    segPrototypes[LED_7].append(segPath);
    //end 7-seg-6

    //start 7-seg-7
    segPath = QPainterPath();
    segPath.moveTo(4,27);
    segPath.lineTo(7,24);
    segPath.lineTo(24,24);
    segPath.lineTo(27,27);
    segPath.lineTo(24,30);
    segPath.lineTo(7,30);
    segPath.closeSubpath();
    segPrototypes[LED_7].append(segPath);
    //end 7-seg-7

    double segW = 3;
    double x0 = 1, x1 = x0 + segW / 2., x2 = x0 + segW, x3 = 31./2. - segW / 2, x4 = 31./2.,
            x5 = 31 - x3, x6 =  31 - x2, x7 = 31 - x1, x8 = 31 - x0,
           y0 = x0, y1 = y0 + segW / 4., y2 = y0 + segW / 2., y3 = y0 + segW, y4 = y3 + 6,
           y5 = 54./2. - segW / 2., y6 = 54./2., y7 = 54 - y5,
           y8 = 54 - y4, y9 = 54 - y3, y10 = 54 - y2, y11 = 54 - y1, y12 = 54 - y0;
    QPointF rtl(18 + 3./8.,y5), vtl(x3,32 + 14./23.), vbr(x5, 21 + 9./23.), lbr(12 + 5./8., y7),
            rbl(rtl.x(),y7), vtr(x5,vtl.y()), vbl(x3, vbr.y()), ltr(lbr.x(), y5);

    //start 14-seg-1
    segPath = QPainterPath();
    segPath.moveTo(x2,y0);
    segPath.lineTo(x6,y0);
    segPath.lineTo(x7,y1);
    segPath.lineTo(x6,y3);
    segPath.lineTo(x5,y3);
    segPath.lineTo(x4,y2);
    segPath.lineTo(x3,y3);
    segPath.lineTo(x2,y3);
    segPath.lineTo(x1,y1);
    segPath.closeSubpath();
    segPrototypes[LED_14].append(segPath);
    //end 14-seg-1

    //start 14-seg-2
    segPath = QPainterPath();
    segPath.moveTo(x8,y3);
    segPath.lineTo(x8,y5);
    segPath.lineTo(x7,y6);
    segPath.lineTo(x6,y5);
    segPath.lineTo(x6,y3);
    segPath.lineTo(x7,y1);
    segPath.closeSubpath();
    segPrototypes[LED_14].append(segPath);
    //end 14-seg-2

    //start 14-seg-3
    segPath = QPainterPath();
    segPath.moveTo(x8,y7);
    segPath.lineTo(x8,y9);
    segPath.lineTo(x7,y11);
    segPath.lineTo(x6,y9);
    segPath.lineTo(x6,y7);
    segPath.lineTo(x7,y6);
    segPath.closeSubpath();
    segPrototypes[LED_14].append(segPath);
    //end 14-seg-3

    //start 14-seg-4
    segPath = QPainterPath();
    segPath.moveTo(x6,y12);
    segPath.lineTo(x2,y12);
    segPath.lineTo(x1,y11);
    segPath.lineTo(x2,y9);
    segPath.lineTo(x3,y9);
    segPath.lineTo(x4,y10);
    segPath.lineTo(x5,y9);
    segPath.lineTo(x6,y9);
    segPath.lineTo(x7,y11);
    segPath.closeSubpath();
    segPrototypes[LED_14].append(segPath);
    //end 14-seg-4

    //start 14-seg-5
    segPath = QPainterPath();
    segPath.moveTo(x0,y7);
    segPath.lineTo(x0,y9);
    segPath.lineTo(x1,y11);
    segPath.lineTo(x2,y9);
    segPath.lineTo(x2,y7);
    segPath.lineTo(x1,y6);
    segPath.closeSubpath();
    segPrototypes[LED_14].append(segPath);
    //end 14-seg-5

    //start 14-seg-6
    segPath = QPainterPath();
    segPath.moveTo(x0,y3);
    segPath.lineTo(x0,y5);
    segPath.lineTo(x1,y6);
    segPath.lineTo(x2,y5);
    segPath.lineTo(x2,y3);
    segPath.lineTo(x1,y1);
    segPath.closeSubpath();
    segPrototypes[LED_14].append(segPath);
    //end 14-seg-6

    //start 14-seg-7
    segPath = QPainterPath();
    segPath.moveTo(x1,y6);
    segPath.lineTo(x2,y5);
    segPath.lineTo(ltr);//11,24);
    segPath.lineTo(x4,y6);
    segPath.lineTo(lbr);//11,30);
    segPath.lineTo(x2,y7);
    segPath.closeSubpath();
    segPrototypes[LED_14].append(segPath);
    //end 14-seg-7

    //start 14-seg-8
    segPath = QPainterPath();
    segPath.moveTo(x7,y6);
    segPath.lineTo(x6,y5);
    segPath.lineTo(rtl);//18,24);
    segPath.lineTo(x4,y6);
    segPath.lineTo(rbl);//18,30);
    segPath.lineTo(x6,y7);
    segPath.closeSubpath();
    segPrototypes[LED_14].append(segPath);
    //end 14-seg-8

    //start 14-seg-9
    segPath = QPainterPath();
    segPath.moveTo(x2,y3);
    //segPath.lineTo(9,7);
    segPath.lineTo(vbl);//13,19);
    segPath.lineTo(x4,y6);
    segPath.lineTo(ltr);//11,24);
    segPath.lineTo(x2,y4);
    segPath.closeSubpath();
    segPrototypes[LED_14].append(segPath);
    //end 14-seg-9

    //start 14-seg-10
    segPath = QPainterPath();
    segPath.moveTo(x4,y2);
    segPath.lineTo(x5,y3);
    segPath.lineTo(vbr);//18,19);
    segPath.lineTo(x4,y6);
    segPath.lineTo(vbl);//13,19);
    segPath.lineTo(x3,y3);
    segPath.closeSubpath();
    segPrototypes[LED_14].append(segPath);
    //end 14-seg-10

    //start 14-seg-11
    segPath = QPainterPath();
    segPath.moveTo(x6,y3);
    segPath.lineTo(x6,y4);
    segPath.lineTo(rtl);//20,24);
    segPath.lineTo(x4,y6);
    segPath.lineTo(vbr);//18,19);
    //segPath.lineTo(22,7);
    segPath.closeSubpath();
    segPrototypes[LED_14].append(segPath);
    //end 14-seg-11

    //start 14-seg-12
    segPath = QPainterPath();
    segPath.moveTo(x2,y9);
    //segPath.lineTo(9,47);
    segPath.lineTo(vtl);//13,35);
    segPath.lineTo(x4,y6);
    segPath.lineTo(lbr);//11,30);
    segPath.lineTo(x2,y8);
    segPath.closeSubpath();
    segPrototypes[LED_14].append(segPath);
    //end 14-seg-12

    //start 14-seg-13
    segPath = QPainterPath();
    segPath.moveTo(x4,y10);
    segPath.lineTo(x5,y9);
    segPath.lineTo(vtr);//18,35);
    segPath.lineTo(x4,y6);
    segPath.lineTo(vtl);//13,35);
    segPath.lineTo(x3,y9);
    segPath.closeSubpath();
    segPrototypes[LED_14].append(segPath);
    //end 14-seg-13

    //start 14-seg-14
    segPath = QPainterPath();
    segPath.moveTo(x6,y9);
    segPath.lineTo(x6,y8);
    segPath.lineTo(rbl);//20,30);
    segPath.lineTo(x4,y6);
    segPath.lineTo(vtr);//18,35);
    //segPath.lineTo(22,47);
    segPath.closeSubpath();
    segPrototypes[LED_14].append(segPath);
    //end 14-seg-14


    //start 16-seg-1
    segPath = QPainterPath();
    segPath.moveTo(x2,y0);
    segPath.lineTo(x3,y0);
    segPath.lineTo(x4,y2);
    segPath.lineTo(x3,y3);
    segPath.lineTo(x2,y3);
    segPath.lineTo(x1,y1);
    segPath.closeSubpath();
    segPrototypes[LED_16].append(segPath);
    //end 16-seg-1

    //start 16-seg-2
    segPath = QPainterPath();
    segPath.moveTo(x6,y0);
    segPath.lineTo(x7,y1);
    segPath.lineTo(x6,y3);
    segPath.lineTo(x5,y3);
    segPath.lineTo(x4,y2);
    segPath.lineTo(x5,y0);
    segPath.closeSubpath();
    segPrototypes[LED_16].append(segPath);
    //end 16-seg-2

    //start 16-seg-3
    segPrototypes[LED_16].append(segPrototypes[LED_14][1]);
    //end 16-seg-3

    //start 16-seg-4
    segPrototypes[LED_16].append(segPrototypes[LED_14][2]);
    //end 16-seg-4

    //start 16-seg-5
    segPath = QPainterPath();
    segPath.moveTo(x6,y12);
    segPath.lineTo(x5,y12);
    segPath.lineTo(x4,y10);
    segPath.lineTo(x5,y9);
    segPath.lineTo(x6,y9);
    segPath.lineTo(x7,y11);
    segPath.closeSubpath();
    segPrototypes[LED_16].append(segPath);
    //end 16-seg-5

    //start 16-seg-6
    segPath = QPainterPath();
    segPath.moveTo(x2,y12);
    segPath.lineTo(x1,y11);
    segPath.lineTo(x2,y9);
    segPath.lineTo(x3,y9);
    segPath.lineTo(x4,y10);
    segPath.lineTo(x3,y12);
    segPath.closeSubpath();
    segPrototypes[LED_16].append(segPath);
    //end 16-seg-6

    //start 16-seg-7
    segPrototypes[LED_16].append(segPrototypes[LED_14][4]);
    //end 16-seg-7

    //start 16-seg-8
    segPrototypes[LED_16].append(segPrototypes[LED_14][5]);
    //end 16-seg-8

    //start 16-seg-9
    segPrototypes[LED_16].append(segPrototypes[LED_14][6]);
    //end 16-seg-9

    //start 16-seg-10
    segPrototypes[LED_16].append(segPrototypes[LED_14][7]);
    //end 16-seg-10

    //start 16-seg-11
    segPrototypes[LED_16].append(segPrototypes[LED_14][8]);
    //end 16-seg-11

    //start 16-seg-12
    segPrototypes[LED_16].append(segPrototypes[LED_14][9]);
    //end 16-seg-12

    //start 16-seg-13
    segPrototypes[LED_16].append(segPrototypes[LED_14][10]);
    //end 16-seg-13

    //start 16-seg-14
    segPrototypes[LED_16].append(segPrototypes[LED_14][11]);
    //end 16-seg-14

    //start 16-seg-15
    segPrototypes[LED_16].append(segPrototypes[LED_14][12]);
    //end 16-seg-15

    //start 16-seg-16
    segPrototypes[LED_16].append(segPrototypes[LED_14][13]);
    //end 16-seg-16
}

QPoint LED_Component::center()
{
    return path->boundingRect().center().toPoint();
}
