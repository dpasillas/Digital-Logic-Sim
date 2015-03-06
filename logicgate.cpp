#include "logicgate.h"
#include <QPainter>
#include <QString>
#include <QStyleOptionGraphicsItem>
#include "circuitboard.h"
//#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QInputDialog>

QMap<int, QPainterPath> LogicGate::gPath;
QString* LogicGate::iecLabel = 0;
int LogicGate::typeCount[10];
QString LogicGate::gateName[11];
LogicGate::voidFunc LogicGate::setupFunc[ENCODER+1];
LogicGate::opFunc LogicGate::operateFunc[ENCODER+1];


void LogicGate::setupStandard(int i)
{
    properties = SingleOutput | IBussable | OBussable;

    output.append(new Pin(RIGHT,Pin::OUTPUT,lgt%2,this));
    output[0]->moveBy(32.,16.);
    output[0]->updatePath();

    setPinCount(i);
}

void LogicGate::setupStandard1()
{
    setupStandard(1);
}

void LogicGate::setupStandard2()
{
    setupStandard(2);
}

void LogicGate::setupMERG()
{
    properties = SingleOutput | VariableShape | OBussable;

    propagationDelay = 0;

    output.append(new Pin(RIGHT,Pin::OUTPUT,false,this));
    output[0]->setPos(12,10);
    output[0]->updatePath();

    for(int i = 0; i < 32; i++)
    {
        input.append(new Pin(LEFT,Pin::INPUT,false,this));
        input[i]->setPos(0,10);
        input[i]->updatePath();
        input[i]->hide();
    }

    setWidth(2);
}

void LogicGate::setupSPLI()
{
    properties = VariableShape | IBussable;

    propagationDelay = 0;

    for(int i = 0; i < 32; i++)
    {
        output.append(new Pin(RIGHT,Pin::OUTPUT,false,this));
        output[i]->setPos(12,10);
        output[i]->updatePath();
        output[i]->hide();
    }

    input.append(new Pin(LEFT,Pin::INPUT,false,this));
    input[0]->setPos(0,10);
    input[0]->updatePath();

    setWidth(2);
}

void LogicGate::setupMULT()
{
    properties = VariableShape | IBussable | OBussable | Mux;
    setMuxWidth(1);
}

void LogicGate::setupDECO()
{
    properties = VariableShape | Mux;
    setMuxWidth(2);
}

void LogicGate::setupCOUN()
{
    properties = VariableShape | MergingPins;

    defaultState = CURRENT;
    defaultValue = 0;

    for(int i = 0; i < 33; i++) //32 + 1 for the "full" marker
    {
        output.append(new Pin(UP, Pin::OUTPUT,false,this));
    }
    input.append(new Pin(DOWN,Pin::INPUT,false,this));
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));

    setWidth(4);
}

void LogicGate::setupREGI()
{
    properties = VariableShape | OBussable | MergingPins;

    for(int i = 0; i < 32; i++)
    {
        output.append(new Pin(UP, Pin::OUTPUT,false,this));
    }

    for(int i = 3; i < 32 + 3; i++)
    {
        input.append(new Pin(DOWN,Pin::INPUT,false,this));
    }

    input.append(new Pin(DOWN,Pin::INPUT,false,this)); //clk
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));//clr
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));//en

    setWidth(4);
}

void LogicGate::setupD_FF()
{
    input.append(new Pin(LEFT,Pin::INPUT,false,this)); //data or toggle
    input[0]->setPos(0,16);
    input[0]->updatePath();
    input.append(new Pin(LEFT,Pin::INPUT,false,this)); //clock
    input[1]->setPos(0,32);
    input[1]->updatePath();
    input.append(new Pin(UP,Pin::INPUT,false,this)); //set
    input[2]->setPos(16,0);
    input[2]->updatePath();
    input.append(new Pin(DOWN,Pin::INPUT,false,this)); //reset
    input[3]->setPos(16,48);
    input[3]->updatePath();

    output.append(new Pin(RIGHT,Pin::OUTPUT,false,this)); //Q
    output[0]->setPos(32,16);
    output[0]->updatePath();
    output.append(new Pin(RIGHT,Pin::OUTPUT,false,this)); //Q'
    output[1]->setPos(32,32);
    output[1]->updatePath();
}

void LogicGate::setupT_FF()
{
    input.append(new Pin(LEFT,Pin::INPUT,false,this)); //data or toggle
    input[0]->setPos(0,16);
    input[0]->updatePath();
    input.append(new Pin(LEFT,Pin::INPUT,false,this)); //clock
    input[1]->setPos(0,32);
    input[1]->updatePath();
    input.append(new Pin(UP,Pin::INPUT,false,this)); //set
    input[2]->setPos(16,0);
    input[2]->updatePath();
    input.append(new Pin(DOWN,Pin::INPUT,false,this)); //reset
    input[3]->setPos(16,48);
    input[3]->updatePath();

    output.append(new Pin(RIGHT,Pin::OUTPUT,false,this)); //Q
    output[0]->setPos(32,16);
    output[0]->updatePath();
    output.append(new Pin(RIGHT,Pin::OUTPUT,false,this)); //Q'
    output[1]->setPos(32,32);
    output[1]->updatePath();
}

void LogicGate::setupJ_K_()
{
    input.append(new Pin(LEFT,Pin::INPUT,false,this)); //J
    input[0]->setPos(0,12);
    input[0]->updatePath();
    input.append(new Pin(LEFT,Pin::INPUT,false,this)); //clock
    input[1]->setPos(0,24);
    input[1]->updatePath();
    input.append(new Pin(LEFT,Pin::INPUT,false,this)); //set
    input[2]->setPos(0,36);
    input[2]->updatePath();

    output.append(new Pin(RIGHT,Pin::OUTPUT,false,this)); //Q
    output[0]->setPos(32,16);
    output[0]->updatePath();
    output.append(new Pin(RIGHT,Pin::OUTPUT,false,this)); //Q'
    output[1]->setPos(32,32);
    output[1]->updatePath();
}

void LogicGate::setupL_CO()
{
    properties = VariableShape | MergingPins;

    for(int i = 0; i < 33; i++)
    {
        output.append(new Pin(UP, Pin::OUTPUT,false,this));
    }

    for(int i = 4; i < 32 + 4; i++)
    {
        input.append(new Pin(DOWN,Pin::INPUT,false,this));
    }

    input.append(new Pin(DOWN,Pin::INPUT,false,this)); //clk
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));//clr
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));//en
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));//ld

    setWidth(4);
}

void LogicGate::setupSL_R()
{
    properties = VariableShape | OBussable | MergingPins;

    for(int i = 0; i < 32; i++)
    {
        output.append(new Pin(UP, Pin::OUTPUT,false,this));
    }

    for(int i = 0; i < 32; i++)
    {
        input.append(new Pin(DOWN,Pin::INPUT,false,this));
    }

    input.append(new Pin(DOWN,Pin::INPUT,false,this)); // clock
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));//clr
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));//ld
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));//Rin
    input.append(new Pin(LEFT,Pin::INPUT,false,this));//LS

    setWidth(4);
}

void LogicGate::setupSR_R()
{
    properties = VariableShape | OBussable | MergingPins;

    for(int i = 0; i < 32; i++)
    {
        output.append(new Pin(UP, Pin::OUTPUT,false,this));
    }

    for(int i = 0; i < 32; i++)
    {
        input.append(new Pin(DOWN,Pin::INPUT,false,this));
    }

    input.append(new Pin(DOWN,Pin::INPUT,false,this));//clock
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));//clr
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));//en
    input.append(new Pin(LEFT,Pin::INPUT,false,this));//Lin
    input.append(new Pin(LEFT,Pin::INPUT,false,this));//RS

    setWidth(4);
}

void LogicGate::setupSB_R()
{
    properties = VariableShape | OBussable | MergingPins;

    for(int i = 0; i < 32; i++)
    {
        output.append(new Pin(UP, Pin::OUTPUT,false,this));
    }

    for(int i = 0; i < 32; i++)
    {
        input.append(new Pin(DOWN,Pin::INPUT,false,this));
    }

    input.append(new Pin(DOWN,Pin::INPUT,false,this));//clk 0
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));//clr 1
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));//en 2
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));//Rin 3
    input.append(new Pin(LEFT,Pin::INPUT,false,this));//RS 4
    input.append(new Pin(LEFT,Pin::INPUT,false,this));//Lin 5
    input.append(new Pin(LEFT,Pin::INPUT,false,this));//LS 6

    setWidth(4);
}

void LogicGate::setupADDE()
{
    input.append(new Pin(DOWN,Pin::INPUT,false,this));
    input[0]->setPos(48 / 3., 32);
    input[0]->updatePath();
    input.append(new Pin(DOWN,Pin::INPUT,false,this));
    input[1]->setPos(48 * 2 / 3., 32);
    input[1]->updatePath();
    input.append(new Pin(RIGHT,Pin::INPUT,false,this));
    input[2]->setPos(48, 16);
    input[2]->updatePath();

    output.append(new Pin(UP,Pin::OUTPUT,false,this));
    output[0]->setPos(48 / 2, 0);
    output[0]->updatePath();
    output.append(new Pin(LEFT,Pin::OUTPUT,false,this));
    output[1]->setPos(0, 16);
    output[1]->updatePath();
}

void LogicGate::setupENCO()
{
    properties = VariableShape | Mux;
    setMuxWidth(5);
}

LogicGate::LogicGate(LogicGate::LogicGateType t, CircuitBoard *board, LogicGate::SStyle s) :
        OperatingComponent(board), lgt(t), style(s)
{
    invisible = false;
    properties = 0;

    defaultState = UNKNOWN;
    defaultValue = 0;

    //any gate with a propagation delay of 0 has the potential to cause an infinite loop
    //through the event handling of CircuitBoard (oscillating circuits)
    propagationDelay = 1;
    width = 1;

    if(gPath.isEmpty())
    {
        for(int i = 0; i < 8; i++)
            typeCount[i] = 0;

        gateName[BUFFER] = QString("BUF_");
        gateName[NOT] = QString("NOT_");
        gateName[AND] = QString("AND_");
        gateName[NAND] = QString("NAND_");
        gateName[OR] = QString("OR_");
        gateName[NOR] = QString("NOR_");
        gateName[XOR] = QString("XOR_");
        gateName[XNOR] = QString("XNOR_");
        gateName[MERGER] = QString("MERG_");
        gateName[SPLITTER] = QString("SPLT_");

        gPath[BUFFER].moveTo(0.,0.);
        gPath[BUFFER].lineTo(32.,16.);
        gPath[BUFFER].lineTo(0.,32.);
        gPath[BUFFER].closeSubpath();

        gPath[NOT] = gPath[BUFFER];

        gPath[AND].moveTo(0.,0.);
        gPath[AND].lineTo(16.,0.);
        gPath[AND].arcTo(0.,0.,32,32,90,-180);
        gPath[AND].lineTo(0.,32.);
        gPath[AND].closeSubpath();

        gPath[NAND] = gPath[AND];

        gPath[OR].moveTo(0.,0.);
        gPath[OR].lineTo(8.,0.);
        gPath[OR].quadTo(22.72, 0., 32., 16.);
        gPath[OR].quadTo(22.72,32,8.,32.);
        gPath[OR].lineTo(0.,32.);
        gPath[OR].quadTo(8.,16.,0.,0.);

        gPath[NOR] = gPath[OR];

        gPath[XOR].moveTo(-4.,0.);
        gPath[XOR].quadTo(4.,16.,-4.,32.);
        gPath[XOR].quadTo(4.,16.,-4.,0.);

        gPath[XNOR] = gPath[XOR];
        
        gPath[MERGER].addRect(0,0,12,20);

        gPath[-1].moveTo(0.,0.);
        gPath[-1].lineTo(32.,0.);
        gPath[-1].lineTo(32.,32.);
        gPath[-1].lineTo(0.,32.);
        gPath[-1].closeSubpath();

        gPath[D_FF].addRect(0,0,32,48);
        gPath[D_FF].moveTo(0,32 - 3);
        gPath[D_FF].lineTo(6, 32);
        gPath[D_FF].lineTo(0, 32 + 3);
        gPath[D_FF].lineTo(6, 32);
        gPath[D_FF].closeSubpath();

        gPath[T_FF] = gPath[D_FF];

        gPath[J_K_FF].addRect(0,0,32,48);
        gPath[J_K_FF].moveTo(0,24 - 3);
        gPath[J_K_FF].lineTo(6,24);
        gPath[J_K_FF].lineTo(0,24 + 3);
        gPath[J_K_FF].lineTo(6,24);
        gPath[J_K_FF].closeSubpath();

        gPath[ADDER].addRect(0,0,48,32);

        iecLabel = new QString[4];

        iecLabel[BUFFER].append("1");
        iecLabel[AND/2].append("&");
        iecLabel[OR/2].append(QChar(8805)).append("1");
        iecLabel[XOR/2].append("=1");

        setupFunc[BUFFER] = &LogicGate::setupStandard1;
        setupFunc[NOT] = &LogicGate::setupStandard1;
        setupFunc[AND] = &LogicGate::setupStandard2;
        setupFunc[NAND] = &LogicGate::setupStandard2;
        setupFunc[OR] = &LogicGate::setupStandard2;
        setupFunc[NOR] = &LogicGate::setupStandard2;
        setupFunc[XOR] = &LogicGate::setupStandard2;
        setupFunc[XNOR] = &LogicGate::setupStandard2;
        setupFunc[MERGER] = &LogicGate::setupMERG;
        setupFunc[SPLITTER] = &LogicGate::setupSPLI;
        setupFunc[MULTIPLEXER] = &LogicGate::setupMULT;
        setupFunc[DECODER] = &LogicGate::setupDECO;
        setupFunc[COUNTER] = &LogicGate::setupCOUN;
        setupFunc[REGISTER] = &LogicGate::setupREGI;
        setupFunc[D_FF] = &LogicGate::setupD_FF;
        setupFunc[T_FF] = &LogicGate::setupT_FF;
        setupFunc[J_K_FF] = &LogicGate::setupJ_K_;
        setupFunc[L_COUNTER] = &LogicGate::setupL_CO;
        setupFunc[ADDER] = &LogicGate::setupADDE;
        setupFunc[SL_REGISTER] = &LogicGate::setupSL_R;
        setupFunc[SR_REGISTER] = &LogicGate::setupSR_R;
        setupFunc[SB_REGISTER] = &LogicGate::setupSB_R;
        setupFunc[ENCODER] = &LogicGate::setupENCO;

        operateFunc[BUFFER] = &LogicGate::opBUFF;
        operateFunc[NOT] = &LogicGate::opNOT;
        operateFunc[AND] = &LogicGate::opAND;
        operateFunc[NAND] = &LogicGate::opNAND;
        operateFunc[OR] = &LogicGate::opOR;
        operateFunc[NOR] = &LogicGate::opNOR;
        operateFunc[XOR] = &LogicGate::opXOR;
        operateFunc[XNOR] = &LogicGate::opXNOR;
        operateFunc[MERGER] = &LogicGate::opMERG;
        operateFunc[SPLITTER] = &LogicGate::opSPLI;
        operateFunc[MULTIPLEXER] = &LogicGate::opMULT;
        operateFunc[DECODER] = &LogicGate::opDECO;
        operateFunc[COUNTER] = &LogicGate::opCOUN;
        operateFunc[REGISTER] = &LogicGate::opREGI;
        operateFunc[D_FF] = &LogicGate::opD_FF;
        operateFunc[T_FF] = &LogicGate::opT_FF;
        operateFunc[J_K_FF] = &LogicGate::opJ_K_;
        operateFunc[L_COUNTER] = &LogicGate::opL_CO;
        operateFunc[ADDER] = &LogicGate::opADDE;
        operateFunc[SL_REGISTER] = &LogicGate::opSL_R;
        operateFunc[SR_REGISTER] = &LogicGate::opSR_R;
        operateFunc[SB_REGISTER] = &LogicGate::opSB_R;
        operateFunc[ENCODER] = &LogicGate::opENCO;
    }

    ((this)->*(setupFunc[lgt]))();
    //automatic naming -- removed for now
    /*if(output)
    do
    {
        output->setName(QString(gateName[t]).append(QString::number(typeCount[t],10)));
        typeCount[t]++;
    }while(!board->contains(output));*/ //automatic naming

    setCacheMode(DeviceCoordinateCache);
}

LogicGate::~LogicGate()
{
    destructFunc();
}

/*LogicGate::~LogicGate()
{//since deleted pins call operate(unsigned int&,LogicState&), a pure virtual function in OperatingComponent,
 //LogicGate needs to delete it's child pins before it is deleted, since backfeeding is allowed
    foreach(Pin* o, output)
        delete o;
    foreach(Pin* pin, input)
        delete pin;
}*/

void LogicGate::setPinCount(int count)
{
    if(lgt > XNOR)
        return;
    if(input.count() == count) // nothing needs to be done here
        return;
    switch(lgt)
    {
    case BUFFER:
    case NOT:
        if(count != 1)
            return;
        break;
    case AND:
    case NAND:
    case OR:
    case NOR:
        if(count < 2 || count > 4)
            return;
        break;
    default: //XOR or XNOR
        if(count != 2)
            return;
    }

    if(input.count() > 0)
    {
        foreach(Pin* pin, input)
            delete pin;
        input.clear();
    }

    for(int i = 0; i < count; i++)
    {
        input.append(new Pin(LEFT,Pin::INPUT,false,this));
        input[i]->setPinWidth(width);
    }

    switch(count)
    {
    case 1:
        input[0]->moveBy(0.,16.);
        input[0]->updatePath();
        break;
    case 2:
        input[0]->moveBy(0,32./3);
        input[0]->updatePath();
        input[1]->moveBy(0,64./3);
        input[1]->updatePath();
        break;
    default:
        for(int i = 0; i < count; i++)
        {
            input[i]->moveBy(0.,(28. * i )/(count - 1) + 2.);
            input[i]->updatePath();
        }
    }
}

void LogicGate::setStyle(LogicGate::SStyle s)
{
    style = s;
    if(lgt >= XOR)
        prepareGeometryChange();
}

void LogicGate::setGraph(CircuitBoard *board)
{
    graph = board;
}

QRectF LogicGate::boundingRect() const
{
    return shape().boundingRect();
}

QPainterPath LogicGate::shape() const
{
    QPainterPath p;
    if(hasProperties(VariableShape))
        p = path;
    else
        p = gPath[(style == IEC)?-1:lgt];

    if((lgt == XOR || lgt == XNOR) && style == IEEE)
        p.addPath(gPath[OR]);

    return (invisible)?QPainterPath():p;
}

void LogicGate::customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPath(shape());
    if(style == IEC && lgt <= XNOR)
        painter->drawText(0,0,32,32,Qt::AlignCenter,iecLabel[lgt/2]);

     static const QTextOption l(Qt::AlignVCenter | Qt::AlignLeft),
        r(Qt::AlignVCenter | Qt::AlignRight),
        t(Qt::AlignTop | Qt::AlignHCenter),
        b(Qt::AlignBottom | Qt::AlignHCenter);

    static QRectF rect;
    int h;

    switch(lgt) //draw necessary text
    {
    case ADDER:
        rect = QRectF(shape().boundingRect());
        rect.adjust(2,2,-2,-2);
        painter->drawText(rect,"Cout",l);
        painter->drawText(rect,"Cin",r);
        break;
    case COUNTER:
        rect = QRectF(input[0]->x() - 10,0,20,20);
        painter->drawText(rect,"K",t);

        rect = QRectF(shape().boundingRect());
        h = rect.height();
        rect.adjust(2,h/3 - 5,-2, -2*h/3 + 5);
        painter->drawText(rect,"En",r);

        rect.translate(0,h / 3);
        painter->drawText(rect,"Clr", r);

        break;
    case REGISTER:
        rect = QRectF(shape().boundingRect());
        h = rect.height();
        rect.adjust(2,h/3 - 5,-2, -2*h/3 + 5);
        painter->drawText(rect,"Ld",r);

        rect.translate(0,h / 3);
        painter->drawText(rect,"Clr", r);
        break;
    case SL_REGISTER:
        rect = QRectF(shape().boundingRect());
        h = rect.height();
        rect.adjust(2,h/4 - 5,-2, -3*h/4 + 5);
        painter->drawText(rect,"Ld",r);

        rect.translate(0,h / 4);
        painter->drawText(rect,"Rin", r);
        painter->drawText(rect,"LS",l);

        rect.translate(0,h / 4);
        painter->drawText(rect,"Clr", r);
        break;
    case SR_REGISTER:
        rect = QRectF(shape().boundingRect());
        h = rect.height();
        rect.adjust(2,h/3 - 5,-2, -2*h/3 + 5);
        painter->drawText(rect,"Ld",r);
        painter->drawText(rect,"RS",l);

        rect.translate(0,h / 3);
        painter->drawText(rect,"Clr", r);
        painter->drawText(rect,"Lin",l);
        break;
    case SB_REGISTER:
        rect = QRectF(shape().boundingRect());
        h = rect.height();
        rect.adjust(2,h/4 - 5,-2, -3*h/4 + 5);
        painter->drawText(rect,"Ld",r);
        painter->drawText(rect,"RS",l);

        rect.translate(0,h / 4);
        painter->drawText(rect,"Rin", r);
        painter->drawText(rect,"Lin",l);

        rect.translate(0,h / 4);
        painter->drawText(rect,"Clr", r);
        painter->drawText(rect,"LS",l);
        break;
    case D_FF:
        rect = QRectF(shape().boundingRect());
        h = rect.height();
        rect.adjust(2,h/3 - 10,-2, -2*h/3 + 10);
        painter->drawText(rect, "D", l);
        painter->drawText(rect, "Q", r);

        rect.translate(0, h / 3);

        painter->drawText(rect, "Q'", r);

        rect = QRectF(shape().boundingRect());
        //rect.adjust(3,3,-3,-3);

        painter->drawText(rect,"S",t);
        painter->drawText(rect,"R",b);
        break;
    case T_FF:
        rect = QRectF(shape().boundingRect());
        h = rect.height();
        rect.adjust(2,h/3 - 10,-2, -2*h/3 + 10);
        painter->drawText(rect, "T", l);
        painter->drawText(rect, "Q", r);

        rect.translate(0, h / 3);

        painter->drawText(rect, "Q'", r);

        rect = QRectF(shape().boundingRect());
        //rect.adjust(3,3,-3,-3);

        painter->drawText(rect,"S",t);
        painter->drawText(rect,"R",b);
        break;
    case J_K_FF:
        rect = QRectF(shape().boundingRect());
        h = rect.height();
        rect.adjust(2,h/4 - 10,-2, -3*h/4 + 10);
        painter->drawText(rect, "J", l);

        rect.translate(0,h/2);
        painter->drawText(rect, "K", l);

        rect.translate(0, -3*h/4 + h/3);
        painter->drawText(rect,"Q",r);

        rect.translate(0, h/3);
        painter->drawText(rect, "Q'", r);
        break;

    case L_COUNTER:
        rect = QRectF(input[32]->x() - 10,0,20,20);
        painter->drawText(rect,"K",t);

        rect = QRectF(shape().boundingRect());
        h = rect.height();
        rect.adjust(2,h/4 - 5,-2, -3*h/4 + 5);
        painter->drawText(rect,"En",r);

        rect.translate(0, h / 4);
        painter->drawText(rect,"Ld",r);

        rect.translate(0, h / 4);
        painter->drawText(rect,"Clr", r);

        break;

    default:;
    }

}

unsigned int LogicGate::mask(int width)
{
    if(width == 1)
        return 1;
    if(width <= 0)
        return 0;
    return (mask(width - 1) << 1) + 1;
}

void LogicGate::opBUFF(Pin* source)
{
    unsigned int value = input[0]->pinValue();
    LogicState s = (input[0]->pinState())?UNKNOWN:CURRENT;

    postEvent(value, s);
}

void LogicGate::opNOT(Pin* source)
{
    unsigned int value;
    LogicState s;
    unsigned int bitMask = mask(output[0]->pinWidth()); //-1 = 0xFFFF FFFF
    s = CURRENT;

    value = input[0]->pinValue();
    s = (input[0]->pinState())?UNKNOWN:s;

    value = ~value;
    value &= bitMask;

    postEvent(value, s);
}

void LogicGate::opAND(Pin* source)
{
    unsigned int value = -1;
    LogicState s = CURRENT;

    foreach(Pin* pin, input)
    {
        if(!pin->pinState()) // if pin->pinState() == CURRENT
            value &= pin->pinValue();
        s = (LogicState)(s|(pin->pinState())?UNKNOWN:s); //if any pin has a non-CURRENT state, set s to UNKNOWN
    }
    if(!value) //if the value becomes 0, then UNKNOWNs an NO_CURRENTs don't matter
        s = CURRENT;

    postEvent(value,s);
}

void LogicGate::opNAND(Pin* source)
{
    unsigned int value = -1;
    LogicState s = CURRENT;

    unsigned int bitMask = mask(output[0]->pinWidth()); //-1 = 0xFFFF FFFF


    foreach(Pin* pin, input)
    {
        if(!pin->pinState()) // if pin->pinState() == CURRENT
            value &= pin->pinValue();
        s = (LogicState)(s|(pin->pinState())?UNKNOWN:s); //if any pin has a non-CURRENT state, set s to UNKNOWN
    }
    if(!value) //if the value becomes 0, then UNKNOWNs an NO_CURRENTs don't matter
        s = CURRENT;

    value = ~value;
    value &= bitMask;

    postEvent(value, s);
}

void LogicGate::opOR(Pin* source)
{
    unsigned int value = 0;
    LogicState s = CURRENT;
    unsigned int bitMask = mask(output[0]->pinWidth());

    foreach(Pin* pin, input)
    {

        value |= pin->pinValue();
        s = (LogicState)(s|(pin->pinState())?UNKNOWN:s);

        qDebug() << pin->pinValue() << ' ' << (int)s;
    }
    if(bitMask == (bitMask & value)) //if the value is the maximum possible, UNKNOWNs and NO_CURRENTs don't matter
        s = CURRENT;
    qDebug() << bitMask << ' ' << value;

    postEvent(value, s);
}

void LogicGate::opNOR(Pin* source)
{
    unsigned int value = 0;
    LogicState s = CURRENT;
    unsigned int bitMask = mask(output[0]->pinWidth()); //-1 = 0xFFFF FFFF

    foreach(Pin* pin, input)
    {
        value |= pin->pinValue();
        s = (LogicState)(s|(pin->pinState())?UNKNOWN:s);
    }
    if(bitMask == (bitMask & value)) //if the value is the maximum possible, UNKNOWNs and NO_CURRENTs don't matter
        s = CURRENT;

    value = ~value;
    value &= mask(output[0]->pinWidth());

    postEvent(value, s);
}

void LogicGate::opXOR(Pin* source)
{
    unsigned int value;
    LogicState s;

    value = input[0]->pinValue() ^ input[1]->pinValue();
    s = (input[0]->pinState() || input[1]->pinState())?UNKNOWN:CURRENT;

    postEvent(value, s);
}

void LogicGate::opXNOR(Pin* source)
{
    unsigned int value;
    LogicState s;
    unsigned int bitMask = mask(output[0]->pinWidth()); //-1 = 0xFFFF FFFF

    value = input[0]->pinValue() ^ input[1]->pinValue();
    s = (input[0]->pinState() || input[1]->pinState())?UNKNOWN:CURRENT;

    value = ~value;
    value &= bitMask;

    postEvent(value, s);
}

void LogicGate::opMERG(Pin* source)
{
    unsigned int value = 0;
    LogicState s = CURRENT;

    for(int i = 0; i < width; i++)
    {
        value <<= 1;
        value += input[width - i - 1]->pinValue();

        s = (LogicState)(s & input[width - i - 1]->pinState());
    }

    postEvent(value,s);
}

void LogicGate::opSPLI(Pin* source)
{
    unsigned int value = input[0]->pinValue();
    LogicState s = input[0]->pinState();

    for(int i = 0; i < width; i++)
    {
        postEvent(value%2,s,output[i]);
        value >>= 1;
    }
}

void LogicGate::opMULT(Pin* source)
{
    unsigned int choice = 0;
    LogicState s = CURRENT;
    for(int i = 0; i < chooser.size(); i++)
    {
        choice += (chooser[i]->pinValue() << i);
        s = LogicState(chooser[i]->pinState() | s);
    }

    s = LogicState(input[choice]->pinState() | s);

    if(s)
        s = UNKNOWN;

    postEvent(input[choice]->pinValue(),s);
}

void LogicGate::opDECO(Pin* source)
{
    unsigned int choice = 0;
    LogicState s = CURRENT;

    for(int i = 0; i < muxWidth; i++)
    {

        s = (LogicState)(input[i]->pinState() | s);
        choice += input[i]->pinValue() << i;
    }

    if(s == NO_CURRENT)
        s = UNKNOWN;

    for(int i = 0; i < (1 << muxWidth);i++)
        postEvent(((i == choice)?1:0),s,output[i]);
}

void LogicGate::opCOUN(Pin* source)
{
    unsigned int value, bitMask = mask(output[0]->pinWidth());
    LogicState s = CURRENT;

    static QMap<Pin*, int> sourceValue;
    static QMap<Pin*, LogicState> sourceState;
    bool reset, enabled;

    if(source != input[0]) // if(clock isn't the sender, no operation is necessary)
                return;
    enabled = (input[2]->pinState() == CURRENT)?input[2]->pinValue():false;
    reset = (input[1]->pinState() == CURRENT)?(input[1]->pinValue()):false;

    if(reset)
    {
        foreach(Pin* o, output)
            postEvent(0,CURRENT, o);
    }
    else if(enabled)
    {
        //if(rising edge)
        if(sourceValue[source] == 0 && input[0]->pinValue() == 1)
        {
            if(!hasProperties(Bussed))
            {
                value = 0;
                for(int i = 0; i < width; i++)
                    value += output[i]->pinValue() << i;

                value += 1;

                for(int i = 0; i < width; i++) //output[output.size() - 1] is reserved for k
                {
                    postEvent(value%2, CURRENT, output[i]);
                    value >>= 1;
                }

                value = 1; //tick
                for(int i = 1; i < width; i++)
                    value &= output[i]->pinValue();
                postEvent(value && !output[0]->pinValue(),CURRENT,output[32]);
            }
            else
            {
                unsigned int val = output[0]->pinValue() + 1;
                val &= bitMask;
                postEvent(val,CURRENT,output[0]);

                if(val == bitMask)
                    postEvent(1, CURRENT,output[32]);
                else
                    postEvent(0, CURRENT,output[32]);

            }
        }

    }
    sourceValue[source] = input[0]->pinValue();
    sourceState[source] = input[0]->pinState();
}

void LogicGate::opREGI(Pin* source)
{
    static QMap<Pin*, int> sourceValue;
    static QMap<Pin*, LogicState> sourceState;
    bool enabled, reset;

    if(source != input[32]) // if(clock isn't the sender, no operation is necessary)
        return;
    enabled = (input[32 + 2]->pinState() == CURRENT)?input[32 + 2]->pinValue():false;
    reset = (input[32 + 1]->pinState() == CURRENT)?(input[32 + 1]->pinValue()):false;

    if(reset)
    {
        foreach(Pin* o, output)
            postEvent(0,CURRENT, o);
    }

    else if(enabled)
    {
        //if(rising edge)
        if(sourceValue[source] == 0 && input[32]->pinValue() == 1)
        {
            for(int i = 0; i < width; i++) //output[output.size() - 1] is reserved for k
            {
                postEvent(input[i]->pinValue(), input[i]->pinState(),output[i]);
                if(hasProperties(Bussed))
                    break;
            }
        }
    }

    sourceValue[source] = input[32]->pinValue();
    sourceState[source] = input[32]->pinState();
}

void LogicGate::opD_FF(Pin* source)
{
    unsigned int value = 0;
    LogicState s = CURRENT;

    static QMap<Pin*, int> sourceValue;
    static QMap<Pin*, LogicState> sourceState;

    if(source == input[0])
        return; //changing data does not affect the flip-flop
    if(source == input[1])
    {
        if(sourceValue[source] == 0 && input[1]->pinValue() == 1 &&
           sourceState[source] == CURRENT && input[1]->pinState() == CURRENT
           && !(input[2]->pinValue() == 1 || input[3]->pinValue() == 1 ||
                input[2]->pinState() || input[3]->pinState())) //rising edge
        {
            value = input[0]->pinValue();
            s = input[0]->pinState();

            postEvent(value, s, output[0]);
            postEvent(~value & 1, s, output[1]);

            sourceValue[source] = input[1]->pinValue();
            sourceState[source] = input[1]->pinState();
        }
        else
        {
            sourceValue[source] = input[1]->pinValue();
            sourceState[source] = input[1]->pinState();
            return;
        }
    }
    else //set or reset flipped
    {
        if(input[2]->pinValue() == 1 && input[3]->pinValue() == 1) //set is true
        {
            postEvent(1,CURRENT,output[0]);
            postEvent(1,CURRENT,output[1]);
        }
        else if(input[2]->pinValue() == 1)
        {
            postEvent(1,CURRENT,output[0]);
            postEvent(0,CURRENT,output[1]);
        }
        else if(input[3]->pinValue() == 1)
        {
            postEvent(0,CURRENT,output[0]);
            postEvent(1,CURRENT,output[1]);
        }
        else if(output[0]->pinValue() == 1 && output[1]->pinValue() == 1)
        {//neither set or reset is on, and both outputs have a value of 1
            postEvent(0,UNKNOWN,output[0]);
            postEvent(0,UNKNOWN,output[1]);
        }//this results in an unstable configuration
    }
}

void LogicGate::opT_FF(Pin* source)
{
    unsigned int value = 0;
    LogicState s = CURRENT;

    static QMap<Pin*, int> sourceValue;
    static QMap<Pin*, LogicState> sourceState;

    if(source == input[0])
            return; //changing data does not affect the flip-flop
        if(source == input[1]) //clock
        {
            if(sourceValue[source] == 0 && input[1]->pinValue() == 1 &&
               sourceState[source] == CURRENT && input[1]->pinState() == CURRENT
               && !(input[2]->pinValue() == 1 || input[3]->pinValue() == 1 ||
                    input[2]->pinState() || input[3]->pinState())) //rising edge
            {
                value = output[0]->pinValue();
                s = output[0]->pinState();

                if(input[0]->pinValue()) //if(T == 1)
                    value = ~value & 1;

                postEvent(value, s, output[0]);
                postEvent(~value & 1, s, output[1]);

                sourceValue[source] = input[1]->pinValue();
                sourceState[source] = input[1]->pinState();
            }
            else
            {
                sourceValue[source] = input[1]->pinValue();
                sourceState[source] = input[1]->pinState();
                return;
            }
        }
        else //set or reset flipped
        {
            if(input[2]->pinValue() == 1 && input[3]->pinValue() == 1) //set is true
            {
                postEvent(1,CURRENT,output[0]);
                postEvent(1,CURRENT,output[1]);
            }
            else if(input[2]->pinValue() == 1)
            {
                postEvent(1,CURRENT,output[0]);
                postEvent(0,CURRENT,output[1]);
            }
            else if(input[3]->pinValue() == 1)
            {
                postEvent(0,CURRENT,output[0]);
                postEvent(1,CURRENT,output[1]);
            }
            else if(output[0]->pinValue() == 1 && output[1]->pinValue() == 1)
            {//neither set or reset is on, and both outputs have a value of 1
                postEvent(0,UNKNOWN,output[0]);
                postEvent(0,UNKNOWN,output[1]);
            }//this results in an unstable configuration
        }
}

void LogicGate::opJ_K_(Pin* source)
{
    unsigned int value = 0;
    LogicState s = CURRENT;

    static QMap<Pin*, int> sourceValue;
    static QMap<Pin*, LogicState> sourceState;

    if(source != input[1])
        return; //changing data does not affect the flip-flop

    if(sourceValue[source] == 0 && input[1]->pinValue() == 1 &&
       sourceState[source] == CURRENT && input[1]->pinState() == CURRENT
       && !(input[0]->pinState() || input[2]->pinState())) //rising edge
    {
        if(input[0]->pinValue()) //J == TRUE
        {
            if(input[2]->pinValue()) //K == TRUE
            {
                value = output[1]->pinValue(); // toggle
                s = output[1]->pinState();
            }
            else
            {
                value = 1; //set
                s = CURRENT;
            }
        }
        else
        {
            if(input[2]->pinValue())
            {
                value = 0; //reset
                s = CURRENT;
            }
            else
            {
                value = output[0]->pinValue(); //stay the same
                s = output[0]->pinState();
            }
        }

        postEvent(value, s, output[0]);
        postEvent(~value & 1, s, output[1]);
    }

    sourceValue[source] = input[1]->pinValue();
    sourceState[source] = input[1]->pinState();
}

void LogicGate::opL_CO(Pin* source)
{
    unsigned int value, bitMask = mask(output[0]->pinWidth());
    //LogicState s = CURRENT;

    static QMap<Pin*, int> sourceValue;
    static QMap<Pin*, LogicState> sourceState;
    bool reset, enabled, load;

    if(source != input[32 + 0]) // if(clock isn't the sender, no operation is necessary)
                return;
    load = (input[32 + 2]->pinState() == CURRENT)?input[32 + 2]->pinValue():false;
    enabled = (input[32 + 3]->pinState() == CURRENT)?input[32 + 3]->pinValue():false;
    reset = (input[32 + 1]->pinState() == CURRENT)?(input[32 + 1]->pinValue()):false;

    if(reset)
    {
        foreach(Pin* o, output)
            postEvent(0,CURRENT, o);
    }
    else if(load)
    {
        if(sourceValue[source] == 0 && input[32]->pinValue() == 1)
        {
            for(int i = 0; i < width; i++) //output[output.size() - 1] is reserved for k
            {
                postEvent(input[i]->pinValue(), input[i]->pinState(),output[i]);
                if(hasProperties(Bussed))
                    break;
            }
        }
    }
    else if(enabled)
    {
        //if(rising edge)
        if(sourceValue[source] == 0 && input[32]->pinValue() == 1)
        {
            if(!hasProperties(Bussed))
            {
                value = 0;
                for(int i = 0; i < width; i++)
                    value += output[i]->pinValue() << i;

                value += 1;

                for(int i = 0; i < width; i++) //output[output.size() - 1] is reserved for k
                {
                    postEvent(value%2, CURRENT, output[i]);
                    value >>= 1;
                }
                value = 1;
                for(int i = 1; i < width; i++)
                    value &= output[i]->pinValue();
                postEvent(value && !output[0]->pinValue(),CURRENT,output[width]);
            }
            else
            {
                unsigned int val = output[0]->pinValue() + 1;
                val &= bitMask;
                postEvent(val,CURRENT,output[0]);

                if(val == bitMask)
                    postEvent(1, CURRENT,output[32]);
                else
                    postEvent(0, CURRENT,output[32]);
            }
        }

    }
    sourceValue[source] = input[0]->pinValue();
    sourceState[source] = input[0]->pinState();
}

void LogicGate::opADDE(Pin* source)
{
    unsigned int v = 0, out = 0;
    LogicState s = CURRENT;

    for(int i = 0; i < 3; i++)
    {
        v += input[i]->pinValue();
        s = (LogicState)(input[i]->pinState() | s);
    }

    out = v / (1 << width);
    v = v % (1 << width);

    if(s)
        s = UNKNOWN;

    postEvent(v,s,output[0]);
    postEvent(out,s,output[1]);


}

void LogicGate::opSL_R(Pin* source)
{
    unsigned int value;
    static QMap<Pin*, int> sourceValue;
    static QMap<Pin*, LogicState> sourceState;
    bool enabled, reset, shift;

    if(source != input[32]) // if(clock isn't the sender, no operation is necessary)
        return;
    enabled = (input[32 + 2]->pinState() == CURRENT)?input[32 + 2]->pinValue():false;
    reset = (input[32 + 1]->pinState() == CURRENT)?(input[32 + 1]->pinValue()):false;
    shift = (input[32 + 4]->pinState() == CURRENT)?(input[32 + 4]->pinValue()):false;

    if(!(sourceValue[source] == 0 && input[32]->pinValue() == 1))
    {
        if(reset)
        {
            foreach(Pin* o, output)
                postEvent(0,CURRENT, o);
        }
        else if(shift)
        {
            if(hasProperties(Bussed))
            {
                value = output[0]->pinValue();
                value = value << 1;
                value += input[32 + 3]->pinValue();
                postEvent(value,output[0]->pinState(),output[0]);
            }
            else
            {
                for(int i = 1; i < width; i++)
                    postEvent(output[i - 1]->pinValue(), output[i - 1]->pinState(), output[i]);
                postEvent(input[32 + 3]->pinValue(), input[32 + 3]->pinState(), output[0]);
            }
        }
        else if(enabled)
        {
            for(int i = 0; i < width; i++) //output[output.size() - 1] is reserved for k
            {
                postEvent(input[i]->pinValue(), input[i]->pinState(),output[i]);
                if(hasProperties(Bussed))
                    break;
            }
        }
    }

    sourceValue[source] = input[32]->pinValue();
    sourceState[source] = input[32]->pinState();
}

void LogicGate::opSR_R(Pin* source)
{
    unsigned int value;
    static QMap<Pin*, int> sourceValue;
    static QMap<Pin*, LogicState> sourceState;
    bool enabled, reset, shift;

    if(source != input[32]) // if(clock isn't the sender, no operation is necessary)
        return;
    enabled = (input[32 + 2]->pinState() == CURRENT)?input[32 + 2]->pinValue():false;
    reset = (input[32 + 1]->pinState() == CURRENT)?(input[32 + 1]->pinValue()):false;
    shift = (input[32 + 4]->pinState() == CURRENT)?(input[32 + 4]->pinValue()):false;

    if(!(sourceValue[source] == 0 && input[32]->pinValue() == 1))
    {
        if(reset)
        {
            foreach(Pin* o, output)
                postEvent(0,CURRENT, o);
        }
        else if(shift)
        {
            if(hasProperties(Bussed))
            {
                value = output[0]->pinValue();
                value = value >> 1;
                value += input[32 + 3]->pinValue() << (width - 1);
                postEvent(value,output[0]->pinState(),output[0]);
            }
            else
            {
                for(int i = 0; i < width - 1; i++)
                    postEvent(output[i + 1]->pinValue(), output[i + 1]->pinState(), output[i]);
                postEvent(input[32 + 3]->pinValue(), input[32 + 3]->pinState(), output[width - 1]);
            }
        }
        else if(enabled)
        {
            for(int i = 0; i < width; i++) //output[output.size() - 1] is reserved for k
            {
                postEvent(input[i]->pinValue(), input[i]->pinState(),output[i]);
                if(hasProperties(Bussed))
                    break;
            }
        }
    }

    sourceValue[source] = input[32]->pinValue();
    sourceState[source] = input[32]->pinState();
}

void LogicGate::opSB_R(Pin* source)
{

    unsigned int value;
    static QMap<Pin*, int> sourceValue;
    static QMap<Pin*, LogicState> sourceState;
    bool enabled, reset, shiftL, shiftR;

    if(source != input[32]) // if(clock isn't the sender, no operation is necessary)
        return;
    enabled = (input[32 + 2]->pinState() == CURRENT)?input[32 + 2]->pinValue():false;
    reset = (input[32 + 1]->pinState() == CURRENT)?(input[32 + 1]->pinValue()):false;
    shiftR = (input[32 + 4]->pinState() == CURRENT)?(input[32 + 4]->pinValue()):false;
    shiftL = (input[32 + 6]->pinState() == CURRENT)?(input[32 + 6]->pinValue()):false;

    if(!(sourceValue[source] == 0 && input[32]->pinValue() == 1))
    {
        if(reset)
        {
            foreach(Pin* o, output)
                postEvent(0,CURRENT, o);
        }
        else if(shiftR)
        {
            if(hasProperties(Bussed))
            {
                value = output[0]->pinValue();
                value = value >> 1;
                value += input[32 + 5]->pinValue() << (width - 1);
                postEvent(value,output[0]->pinState(),output[0]);
            }
            else
            {
                for(int i = 0; i < width - 1; i++)
                    postEvent(output[i + 1]->pinValue(), output[i + 1]->pinState(), output[i]);
                postEvent(input[32 + 5]->pinValue(), input[32 + 5]->pinState(), output[width - 1]);
            }
        }
        else if(shiftL)
        {
            if(hasProperties(Bussed))
            {
                value = output[0]->pinValue();
                value = value << 1;
                value += input[32 + 3]->pinValue();
                postEvent(value,output[0]->pinState(),output[0]);
            }
            else
            {
                for(int i = 1; i < width; i++)
                    postEvent(output[i - 1]->pinValue(), output[i - 1]->pinState(), output[i]);
                postEvent(input[32 + 3]->pinValue(), input[32 + 3]->pinState(), output[0]);
            }
        }
        else if(enabled)
        {
            for(int i = 0; i < width; i++) //output[output.size() - 1] is reserved for k
            {
                postEvent(input[i]->pinValue(), input[i]->pinState(),output[i]);
                if(hasProperties(Bussed))
                    break;
            }
        }
    }

    sourceValue[source] = input[32]->pinValue();
    sourceState[source] = input[32]->pinState();
}

void LogicGate::opENCO(Pin *source)
{
    unsigned int choice = 0;
    LogicState s = CURRENT;

    for(int i = (1 << muxWidth) - 1; i >= 0; i--)
    {
        s = (LogicState)(input[i]->pinState() | s);
        if(s)
            break;

        if(input[i]->pinValue())
        {
            choice = i;
            break;
        }
    }

    if(s == NO_CURRENT)
        s = UNKNOWN;

    for(int i = 0; i < muxWidth; i++)
    {
        postEvent(choice % 2, s, output[i]);
        choice >>= 1;
    }
}

void LogicGate::operate(Pin* source)
{
/*    unsigned int value;
    LogicState s;
    unsigned int bitMask = (output[0])?mask(output[0]->pinWidth()):-1; //-1 = 0xFFFF FFFF
    s = CURRENT;
    int choice = 0;
    static QMap<Pin*, int> sourceValue;
    static QMap<Pin*, LogicState> sourceState;
    bool reset, enabled;*/

    ((this)->*(operateFunc[lgt]))(source);


}

void LogicGate::setWidth(int w)
{ //32 max, 16 for counters and registers
    //this->prepareGeometryChange();
    width = w;
    static int SPACING = 7, MARGIN = 5, PIN_THICKNESS = 8;
    int x = 0;

    if(hasProperties(IBussable) && !hasProperties(MergingPins))
    foreach(Pin* pin, input)
        pin->setPinWidth(w);

    if(hasProperties(OBussable) && !hasProperties(MergingPins))
    foreach(Pin* pin, output)
        pin->setPinWidth(w);

    if(lgt == SPLITTER || lgt == MERGER)
    {
        centerPoint.setX(6);
        centerPoint.setY(MARGIN + PIN_THICKNESS / 2 + ((SPACING + PIN_THICKNESS) * (width - 1))/ 2);

        prepareGeometryChange();
        path -= path;
        path.addRect(0,0,12,centerPoint.y() * 2);
    }
    if(lgt == COUNTER || lgt == REGISTER || lgt == L_COUNTER || lgt == SL_REGISTER || lgt == SR_REGISTER || lgt == SB_REGISTER)
    {
        if(!hasProperties(Bussed))
            x = MARGIN * 2 + PIN_THICKNESS + ((SPACING + PIN_THICKNESS) * (width - 1));
        else
            x = MARGIN * 2 + PIN_THICKNESS;
        centerPoint.setX((x + MARGIN * 2 + PIN_THICKNESS + SPACING * 2)/2);
        centerPoint.setY(20);

        path -= path;
        path.addRect(0,0,x,centerPoint.y() * 2);
        path.addRect(x,0,MARGIN * 2 + PIN_THICKNESS + 2 * SPACING, centerPoint.y() * 2);
        QPainterPath cp; //clock indicator path
        cp.moveTo(-3,0);
        cp.lineTo(0,-6);
        cp.lineTo(3,0);
        cp.lineTo(0,-6);
        cp.closeSubpath();
        cp.translate(x + MARGIN + PIN_THICKNESS / 2 + SPACING, 40);
        path.addPath(cp);
    }

    switch(lgt)
    {
    case ADDER:
        output[0]->setPinWidth(w);
        input[0]->setPinWidth(w);
        input[1]->setPinWidth(w);
        break;
    case MERGER:
        foreach(Pin* pin, input)
        {
            pin->hide();
            //pin->disconnect();
        }
        for(int i = 0; i < width; i++)
        {
            input[i]->setPos(0, MARGIN + PIN_THICKNESS / 2 + (width - i - 1) * (PIN_THICKNESS + SPACING));
            input[i]->updatePath();
            input[i]->show();
        }

        foreach(Pin* pin, input)
        {
            if(!pin->isVisible())
            {
                pin->disconnect();
                pin->setName("");
            }
        }

        output[0]->setPos(12, centerPoint.y());
        output[0]->updatePath();
        break;
    case SPLITTER:
        input[0]->setPos(0, centerPoint.y());
        input[0]->updatePath();

        foreach(Pin* pin, output)
        {
            pin->hide();
            //pin->disconnect();
        }
        for(int i = 0; i < width; i++)
        {
            output[i]->setPos(12, MARGIN + PIN_THICKNESS / 2 + (width - i - 1) * (PIN_THICKNESS + SPACING));
            output[i]->updatePath();
            output[i]->show();
        }
        foreach(Pin* pin, output)
        {
            if(!pin->isVisible())
            {
                pin->disconnect();
                pin->setName("");
            }
        }

        break;
    case COUNTER:
        foreach(Pin* pin, output)
        {
            pin->hide();
            //pin->disconnect();
        }
        foreach(Pin* pin, input)
            pin->hide();

        if(!hasProperties(Bussed))
        {
            output[0]->setPinWidth(1);
            for(int i = 0; i < width; i++)
            {
                output[i]->setPos(MARGIN + PIN_THICKNESS / 2 + (width - i - 1) * (PIN_THICKNESS + SPACING),0);
                output[i]->updatePath();
                output[i]->show();
                output[i]->setValue(0);
                output[i]->setState(CURRENT);
            }
        }
        else
        {
            output[0]->setPinWidth(w);
            output[0]->setPos(x / 2, 0);
            output[0]->updatePath();
            output[0]->show();
        }

        output[32]->setPos(x + (MARGIN + PIN_THICKNESS / 2) + SPACING,0);
        output[32]->updatePath();
        output[32]->show();
        output[32]->setValue(0);
        output[32]->setState(CURRENT);

        input[0]->setPos(output[32]->x(), 40); // clock input
        input[0]->updatePath();
        input[0]->show();

        input[1]->setPos(centerPoint.x() * 2, 40 * 2 / 3);
        input[1]->updatePath();
        input[1]->show();

        input[2]->setPos(centerPoint.x() * 2, 40 / 3);
        input[2]->updatePath();
        input[2]->show();

        foreach(Pin* pin, output)
        {
            if(!pin->isVisible())
            {
                pin->disconnect();
                pin->setName("");
            }
        }

        break;
    case REGISTER:
        foreach(Pin* pin, output)
        {
            pin->hide();
            //pin->disconnect();
        }
        foreach(Pin* pin, input)
        {
            pin->hide();
            //pin->disconnect();
        }
        if(!hasProperties(Bussed))
        {
            output[0]->setPinWidth(1);
            input[0]->setPinWidth(1);
            for(int i = 0; i < width; i++)
            {
                output[i]->setPos(MARGIN + PIN_THICKNESS / 2 + (width - i - 1) * (PIN_THICKNESS + SPACING),0);
                output[i]->updatePath();
                output[i]->show();
            }
            for(int i = 0; i < width; i++)
            {
                input[i]->setPos(MARGIN + PIN_THICKNESS / 2 + (width - i - 1) * (PIN_THICKNESS + SPACING),40);
                input[i]->updatePath();
                input[i]->show();
            }

        }
        else
        {
            output[0]->setPinWidth(w);
            output[0]->setPos(x / 2, 0);
            output[0]->updatePath();
            output[0]->show();

            input[0]->setPinWidth(w);
            input[0]->setPos(x / 2, 40);
            input[0]->updatePath();
            input[0]->show();
        }

        input[32 + 0]->setPos(x + (MARGIN + PIN_THICKNESS / 2) + SPACING, 40); // clock input
        input[32 + 0]->updatePath();
        input[32 + 0]->show();

        input[32 + 1]->setPos(centerPoint.x() * 2, 40 * 2 / 3);
        input[32 + 1]->updatePath();
        input[32 + 1]->show();

        input[32 + 2]->setPos(centerPoint.x() * 2, 40 / 3);
        input[32 + 2]->updatePath();
        input[32 + 2]->show();

        foreach(Pin* pin, output)
        {
            if(!pin->isVisible())
            {
                pin->disconnect();
                pin->setName("");
            }
        }

        foreach(Pin* pin, input)
        {
            if(!pin->isVisible())
            {
                pin->disconnect();
                pin->setName("");
            }
        }

        break;
    case L_COUNTER:
        foreach(Pin* pin, output)
        {
            pin->hide();
            //pin->disconnect();
        }
        foreach(Pin* pin, input)
        {
            pin->hide();
            //pin->disconnect();
        }
        if(!hasProperties(Bussed))
        {
            output[0]->setPinWidth(1);
            input[0]->setPinWidth(1);
            for(int i = 0; i < width; i++)
            {
                output[i]->setPos(MARGIN + PIN_THICKNESS / 2 + (width - i - 1) * (PIN_THICKNESS + SPACING),0);
                output[i]->updatePath();
                output[i]->show();
            }
            for(int i = 0; i < width; i++)
            {
                input[i]->setPos(MARGIN + PIN_THICKNESS / 2 + (width - i - 1) * (PIN_THICKNESS + SPACING),40);
                input[i]->updatePath();
                input[i]->show();
            }

        }
        else
        {
            output[0]->setPinWidth(w);
            output[0]->setPos(x / 2, 0);
            output[0]->updatePath();
            output[0]->show();

            input[0]->setPinWidth(w);
            input[0]->setPos(x / 2, 40);
            input[0]->updatePath();
            input[0]->show();
        }

        output[32]->setPos(x + (MARGIN + PIN_THICKNESS / 2) + SPACING,0);
        output[32]->updatePath();
        output[32]->show();
        output[32]->setValue(0);
        output[32]->setState(CURRENT);

        input[32 + 0]->setPos(x + (MARGIN + PIN_THICKNESS / 2) + SPACING, 40); // clock input
        input[32 + 0]->updatePath();
        input[32 + 0]->show();

        input[32 + 1]->setPos(centerPoint.x() * 2, 40 * 3 / 4);
        input[32 + 1]->updatePath();
        input[32 + 1]->show();

        input[32 + 2]->setPos(centerPoint.x() * 2, 40 * 2 / 4);
        input[32 + 2]->updatePath();
        input[32 + 2]->show();

        input[32 + 3]->setPos(centerPoint.x() * 2, 40 / 4);
        input[32 + 3]->updatePath();
        input[32 + 3]->show();

        foreach(Pin* pin, output)
        {
            if(!pin->isVisible())
            {
                pin->disconnect();
                pin->setName("");
            }
        }

        foreach(Pin* pin, input)
        {
            if(!pin->isVisible())
            {
                pin->disconnect();
                pin->setName("");
            }
        }

        break;
    case SL_REGISTER:
        foreach(Pin* pin, output)
        {
            pin->hide();
            //pin->disconnect();
        }
        foreach(Pin* pin, input)
        {
            pin->hide();
            //pin->disconnect();
        }
        if(!hasProperties(Bussed))
        {
            output[0]->setPinWidth(1);
            input[0]->setPinWidth(1);
            for(int i = 0; i < width; i++)
            {
                output[i]->setPos(MARGIN + PIN_THICKNESS / 2 + (width - i - 1) * (PIN_THICKNESS + SPACING),0);
                output[i]->updatePath();
                output[i]->show();
            }
            for(int i = 0; i < width; i++)
            {
                input[i]->setPos(MARGIN + PIN_THICKNESS / 2 + (width - i - 1) * (PIN_THICKNESS + SPACING),40);
                input[i]->updatePath();
                input[i]->show();
            }

        }
        else
        {
            output[0]->setPinWidth(w);
            output[0]->setPos(x / 2, 0);
            output[0]->updatePath();
            output[0]->show();

            input[0]->setPinWidth(w);
            input[0]->setPos(x / 2, 40);
            input[0]->updatePath();
            input[0]->show();
        }

        input[32 + 0]->setPos(x + (MARGIN + PIN_THICKNESS / 2) + SPACING, 40); // clock input
        input[32 + 0]->updatePath();
        input[32 + 0]->show();

        input[32 + 1]->setPos(centerPoint.x() * 2, 40 * 3 / 4);
        input[32 + 1]->updatePath();
        input[32 + 1]->show();

        input[32 + 2]->setPos(centerPoint.x() * 2, 40 * 1/ 4);
        input[32 + 2]->updatePath();
        input[32 + 2]->show();

        input[32 + 3]->setPos(centerPoint.x() * 2, 40 * 2 / 4);
        input[32 + 3]->updatePath();
        input[32 + 3]->show();

        input[32 + 4]->setPos(0, 40 * 2 / 4);
        input[32 + 4]->updatePath();
        input[32 + 4]->show();

        foreach(Pin* pin, output)
        {
            if(!pin->isVisible())
            {
                pin->disconnect();
                pin->setName("");
            }
        }

        foreach(Pin* pin, input)
        {
            if(!pin->isVisible())
            {
                pin->disconnect();
                pin->setName("");
            }
        }

        break;
    case SR_REGISTER:
        foreach(Pin* pin, output)
        {
            pin->hide();
            //pin->disconnect();
        }
        foreach(Pin* pin, input)
        {
            pin->hide();
            //pin->disconnect();
        }
        if(!hasProperties(Bussed))
        {
            output[0]->setPinWidth(1);
            input[0]->setPinWidth(1);
            for(int i = 0; i < width; i++)
            {
                output[i]->setPos(MARGIN + PIN_THICKNESS / 2 + (width - i - 1) * (PIN_THICKNESS + SPACING),0);
                output[i]->updatePath();
                output[i]->show();
            }
            for(int i = 0; i < width; i++)
            {
                input[i]->setPos(MARGIN + PIN_THICKNESS / 2 + (width - i - 1) * (PIN_THICKNESS + SPACING),40);
                input[i]->updatePath();
                input[i]->show();
            }

        }
        else
        {
            output[0]->setPinWidth(w);
            output[0]->setPos(x / 2, 0);
            output[0]->updatePath();
            output[0]->show();

            input[0]->setPinWidth(w);
            input[0]->setPos(x / 2, 40);
            input[0]->updatePath();
            input[0]->show();
        }

        input[32 + 0]->setPos(x + (MARGIN + PIN_THICKNESS / 2) + SPACING, 40); // clock input
        input[32 + 0]->updatePath();
        input[32 + 0]->show();

        input[32 + 1]->setPos(centerPoint.x() * 2, 40 * 2 / 3);
        input[32 + 1]->updatePath();
        input[32 + 1]->show();

        input[32 + 2]->setPos(centerPoint.x() * 2, 40 * 1 / 3);
        input[32 + 2]->updatePath();
        input[32 + 2]->show();

        input[32 + 3]->setPos(0, 40 * 2 / 3);
        input[32 + 3]->updatePath();
        input[32 + 3]->show();

        input[32 + 4]->setPos(0, 40 * 1 / 3);
        input[32 + 4]->updatePath();
        input[32 + 4]->show();

        foreach(Pin* pin, output)
        {
            if(!pin->isVisible())
            {
                pin->disconnect();
                pin->setName("");
            }
        }

        foreach(Pin* pin, input)
        {
            if(!pin->isVisible())
            {
                pin->disconnect();
                pin->setName("");
            }
        }

        break;
    case SB_REGISTER:
        foreach(Pin* pin, output)
        {
            pin->hide();
            //pin->disconnect();
        }
        foreach(Pin* pin, input)
        {
            pin->hide();
            //pin->disconnect();
        }
        if(!hasProperties(Bussed))
        {
            output[0]->setPinWidth(1);
            input[0]->setPinWidth(1);
            for(int i = 0; i < width; i++)
            {
                output[i]->setPos(MARGIN + PIN_THICKNESS / 2 + (width - i - 1) * (PIN_THICKNESS + SPACING),0);
                output[i]->updatePath();
                output[i]->show();
            }
            for(int i = 0; i < width; i++)
            {
                input[i]->setPos(MARGIN + PIN_THICKNESS / 2 + (width - i - 1) * (PIN_THICKNESS + SPACING),40);
                input[i]->updatePath();
                input[i]->show();
            }

        }
        else
        {
            output[0]->setPinWidth(w);
            output[0]->setPos(x / 2, 0);
            output[0]->updatePath();
            output[0]->show();

            input[0]->setPinWidth(w);
            input[0]->setPos(x / 2, 40);
            input[0]->updatePath();
            input[0]->show();
        }

        input[32 + 0]->setPos(x + (MARGIN + PIN_THICKNESS / 2) + SPACING, 40); // clock input
        input[32 + 0]->updatePath();
        input[32 + 0]->show();

        input[32 + 1]->setPos(centerPoint.x() * 2, 40 * 3 / 4);
        input[32 + 1]->updatePath();
        input[32 + 1]->show();

        input[32 + 2]->setPos(centerPoint.x() * 2, 40 * 1 / 4);
        input[32 + 2]->updatePath();
        input[32 + 2]->show();

        input[32 + 3]->setPos(centerPoint.x() * 2, 40 * 2 / 4);
        input[32 + 3]->updatePath();
        input[32 + 3]->show();

        input[32 + 4]->setPos(0, 40 * 1 / 4);
        input[32 + 4]->updatePath();
        input[32 + 4]->show();

        input[32 + 5]->setPos(0, 40 * 2 / 4);
        input[32 + 5]->updatePath();
        input[32 + 5]->show();

        input[32 + 6]->setPos(0, 40 * 3 / 4);
        input[32 + 6]->updatePath();
        input[32 + 6]->show();

        foreach(Pin* pin, output)
        {
            if(!pin->isVisible())
            {
                pin->disconnect();
                pin->setName("");
            }
        }

        foreach(Pin* pin, input)
        {
            if(!pin->isVisible())
            {
                pin->disconnect();
                pin->setName("");
            }
        }
        break;
    default:;
    }

    moveBy(1,1);
    moveBy(-1,-1);
}

QPoint LogicGate::center()
{
    if(hasProperties(VariableShape))
        return centerPoint;
    else
        return OperatingComponent::center();
}

QAction* LogicGate::runContextMenu(QMenu *menu, QPoint screenPos)
{
    QAction *setWidthAction, *setPinCountAction, *setDelayAction, *bussedIO_Action, *setMuxWidthAction, *executed;

    if(lgt > NOT && lgt < XOR)//AND, NAND, OR, NOR
        setPinCountAction = menu->addAction("Set Pin Count...");
    if(lgt != DECODER && lgt != ENCODER)
        setWidthAction = menu->addAction("Set Width...");
    setDelayAction = menu->addAction("Set Delay...");
    if(hasProperties(MergingPins))
    {
        bussedIO_Action = menu->addAction("Bussed IO");
        bussedIO_Action->setCheckable(true);
        if(hasProperties(Bussed))
            bussedIO_Action->setChecked(true);
    }
    if(hasProperties(Mux))
        setMuxWidthAction = menu->addAction(QString("Set Multplexer Width.."));
    executed = menu->exec(screenPos);

    //QStringList list;
    //for(int i = 1; i < 6; i++)
    //    list << QString("%1 to 1 multiplexer").arg(QString::number(1 << i,10));

    //QInputDialog::getItem(OperatingComponent::getBoard(),"Multiplexer","Width: ",list,0,false,0);

    if(executed)
    {
        if(executed == setWidthAction)
        {
            bool b = false;
            int w = QInputDialog::getInt(OperatingComponent::getBoard()->parentWidget(),
                                         QString("Set Width"),QString("Width: "),width,1,16,1,&b);
            if(b)
                setWidth(w);
        }
        else if(executed == setDelayAction)
        {
            bool b = false;
            double d = QInputDialog::getDouble(OperatingComponent::getBoard()->parentWidget(),
                                               "Set Delay", "Delay: ", delay(), 0, 1000, 6, &b);
            if(b)
                setDelay(d);
        }
        else if(executed == setPinCountAction)
        {
            bool b = false;
            int c = QInputDialog::getInt(OperatingComponent::getBoard()->parentWidget(),
                                         QString("Set Pin Count"),QString("Pin Count: "),input.count(),2,4,1,&b);

            if(b)
                setPinCount(c);
        }
        else if(executed == bussedIO_Action)
        {
            if(hasProperties(Bussed))
                properties &= ~Bussed;
            else
                properties |= Bussed;

            setWidth(width);
        }
        else if(executed == setMuxWidthAction)
        {
            bool b;
            int w = QInputDialog::getInt(OperatingComponent::getBoard()->parentWidget(),
                                         QString("Set Multiplexer Width"), QString("Mux Width: "), muxWidth, 1,5,1,&b);
            if(b)
                setMuxWidth(w);
        }

        update();
    }

    return executed;
}

void LogicGate::setMuxWidth(int w)
{
    if(!hasProperties(Mux) || muxWidth == w || w < 1 || w > 5)
        return;

    muxWidth = w;

    static int SPACING = 7, MARGIN = 5, PIN_THICKNESS = 8;
    int pathWidth = MARGIN * 2 + PIN_THICKNESS + (w - 1) * (SPACING + PIN_THICKNESS),
        pathMaxHeight = MARGIN * 2 + PIN_THICKNESS + ((1 << w) - 1) * (SPACING + PIN_THICKNESS),
        pathMinHeight = pathMaxHeight - pathWidth,
        diff = pathMaxHeight - pathMinHeight;

    foreach(Pin* pin, chooser)
    {
        pin->hide();
    }
    foreach(Pin* pin, input)
    {
        pin->hide();
    }

    foreach(Pin* pin, output)
    {
        pin->hide();
    }

    if(lgt == MULTIPLEXER)
    {
        //adjust path, as appropriate
        path -= path;
        path.moveTo(0,0);
        path.lineTo(pathWidth,pathWidth / 2);
        path.lineTo(pathWidth, pathWidth/2 + pathMinHeight);
        path.lineTo(0, pathMaxHeight);
        path.closeSubpath();

        QPointF p(path.boundingRect().center());
        centerPoint.setX(p.x());
        centerPoint.setY(p.y());

        if(output.count() == 0)
            output.append(new Pin(RIGHT,Pin::OUTPUT,false,this));

        output[0]->show();
        output[0]->setPos(pathWidth, pathMaxHeight / 2);
        output[0]->updatePath();

        while(input.count() < (1 << w))
            input.append(new Pin(LEFT,Pin::INPUT,false,this));

        for(int i = 0; i < (1 << w); i++)
        {
            input[i]->setPos(0,MARGIN + PIN_THICKNESS / 2 + ((1 << w) - i - 1) * (PIN_THICKNESS + SPACING));
            input[i]->show();
            input[i]->updatePath();
        }

        while(chooser.count() < w)
            chooser.append(new Pin(DOWN,Pin::INPUT,false,this));

        for(int i = 0; i < w; i++)
        {
            chooser[w - i - 1]->setPos(MARGIN + PIN_THICKNESS / 2 + i * (PIN_THICKNESS + SPACING),
                               pathMaxHeight - (i + 1) * diff / (2 * (w + 1)));
            chooser[w - i - 1]->show();
            chooser[w - i - 1]->updatePath();
        }
    }
    if(lgt == DECODER)
    {
        //adjust path, as appropriate
        path -= path;
        path.moveTo(pathWidth,0);
        path.lineTo(pathWidth,pathMaxHeight);
        path.lineTo(0, pathWidth/2 + pathMinHeight);
        path.lineTo(0, pathWidth/2);
        path.closeSubpath();

        QPointF p(path.boundingRect().center());
        centerPoint.setX(p.x());
        centerPoint.setY(p.y());

        while(output.count() < (1 << w))
            output.append(new Pin(RIGHT,Pin::OUTPUT,false,this));

        for(int i = 0; i < (1 << w); i++)
        {
            output[i]->setPos(pathWidth,MARGIN + PIN_THICKNESS / 2 + ((1 << w) - i - 1) * (PIN_THICKNESS + SPACING));
            output[i]->show();
            output[i]->updatePath();
        }

        while(input.count() < w)
            input.append(new Pin(LEFT,Pin::INPUT,false,this));

        int yInc = (pathMinHeight - MARGIN * 2 - PIN_THICKNESS - (w - 1) * (PIN_THICKNESS + SPACING)) / 2;

        if(w == 1)
        {
            input[0]->setPos(0,pathMaxHeight / 2);
            input[0]->show();
            input[0]->updatePath();
        }
        else for(int i = 0; i < w; i++)
        {
            input[i]->setPos(0,MARGIN + diff / 2 + PIN_THICKNESS / 2 + yInc + (w - i - 1) * (PIN_THICKNESS + SPACING));
            input[i]->show();
            input[i]->updatePath();
        }
    }
    if(lgt == ENCODER)
    {
        //adjust path, as appropriate
        path -= path;
        path.moveTo(0,0);
        path.lineTo(0,pathMaxHeight);
        path.lineTo(pathWidth, pathWidth/2 + pathMinHeight);
        path.lineTo(pathWidth, pathWidth/2);
        path.closeSubpath();

        QPointF p(path.boundingRect().center());
        centerPoint.setX(p.x());
        centerPoint.setY(p.y());

        while(input.count() < (1 << w))
            input.append(new Pin(LEFT,Pin::INPUT,false,this));

        for(int i = 0; i < (1 << w); i++)
        {
            input[i]->setPos(0,MARGIN + PIN_THICKNESS / 2 + ((1 << w) - i - 1) * (PIN_THICKNESS + SPACING));
            input[i]->show();
            input[i]->updatePath();
        }

        while(output.count() < w)
            output.append(new Pin(RIGHT,Pin::OUTPUT,false,this));

        int yInc = (pathMinHeight - MARGIN * 2 - PIN_THICKNESS - (w - 1) * (PIN_THICKNESS + SPACING)) / 2;

        if(w == 1)
        {
            output[0]->setPos(pathWidth,pathMaxHeight / 2);
            output[0]->show();
            output[0]->updatePath();
        }
        else for(int i = 0; i < w; i++)
        {
            output[i]->setPos(pathWidth,MARGIN + diff / 2 + PIN_THICKNESS / 2 + yInc + (w - i - 1) * (PIN_THICKNESS + SPACING));
            output[i]->show();
            output[i]->updatePath();
        }
    }

    foreach(Pin* pin, chooser)
    {
        if(!pin->isVisible())
        {
            pin->disconnect();
            pin->setName("");
        }
    }
    foreach(Pin* pin, input)
    {
        if(!pin->isVisible())
        {
            pin->disconnect();
            pin->setName("");
        }
    }

    foreach(Pin* pin, output)
    {
        if(!pin->isVisible())
        {
            pin->disconnect();
            pin->setName("");
        }
    }

    moveBy(1,1);
    moveBy(-1,-1);
}

void LogicGate::setPropertiesTo(int flag)
{
    OperatingComponent::setPropertiesTo(flag);
    setWidth(width);
}

LogicGate::LogicGateType LogicGate::logicType()
{
    return lgt;
}

int LogicGate::getWidth()
{
    return width;
}

int LogicGate::getMuxWidth()
{
    return muxWidth;
}

void LogicGate::reset()
{
    foreach(Pin* pin, output)
    {
        pin->setValue(defaultValue);
        pin->setState(defaultState);
    }
    OperatingComponent::reset();
}
