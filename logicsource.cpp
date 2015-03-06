#include "logicsource.h"
#include "circuitboard.h"
#include "pin.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

QPainterPath* LogicSource::path = 0;
QPainterPath* LogicSource::clockPath = 0;
int LogicSource::typeCount[6] = {0};
QString LogicSource::inputName[6];
QPointF* LogicSource::mousePressPos = new QPointF();

LogicSource::LogicSource(LogicSource::InputType it, CircuitBoard *parent):OperatingComponent(parent),
           inputType(it),checkable(false),checked(false)
{
    defaultState = CURRENT;
    defaultValue = 0;

    if(!path)
    {
        clockPath = new QPainterPath();

        clockPath->moveTo(4,12);
        clockPath->lineTo(8,12);
        clockPath->lineTo(8,20);
        clockPath->lineTo(16,20);
        clockPath->lineTo(16,12);
        clockPath->lineTo(24,12);
        clockPath->lineTo(24,20);
        clockPath->lineTo(28,20);

        path = new QPainterPath(QPointF(0,0));
        path->lineTo(32,0);
        path->lineTo(32,32);
        path->lineTo(0,32);
        path->closeSubpath();

        inputName[BUTTON] = QObject::tr("Button_");
        inputName[SWITCH] = QObject::tr("Switch_");
        inputName[CLOCK] = QObject::tr("Clock_");
        inputName[CUSTOM] = QObject::tr("Custom_");
        inputName[SOURCE] = QObject::tr("Source_");
        inputName[GROUND] = QObject::tr("Ground_");

        //for(int i = 0; i < 4; i++)
        //    typeCount[i] = 0;
    }

    if(it == LogicSource::SWITCH || it == LogicSource::BUTTON)
        checkable = true;
    else
        checkable = false;
    output.append(new Pin(RIGHT,Pin::OUTPUT,false,this));

    output[0]->moveBy(32,16);
    output[0]->updatePath();

    output[0]->setValue(0);

    if(it == LogicSource::SOURCE)
    {
        defaultValue = 1;
        output[0]->setValue(1);
    }

    if(it == LogicSource::CLOCK)
    {
        output[0]->destination->append(output[0]);
        output[0]->source = output[0];
        propagationDelay = 10;
        output[0]->updateNext();
    }

    /*do
    {
        output[0]->setName(QString(inputName[it]).append(QString::number(typeCount[it],10)));
        typeCount[it]++;
    }while(!parent->contains(output[0])); //automatic naming
*/
    setCacheMode(DeviceCoordinateCache);
}

LogicSource::~LogicSource()
{
    destructFunc();
}

QRectF LogicSource::boundingRect() const
{
    return QRectF(-1,-1,34,34);
}

QPainterPath LogicSource::shape() const
{
    return (invisible)?QPainterPath():*path;
}

void LogicSource::customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //OperatingComponent::paint(painter,option,widget);
    painter->drawPath(*path);

    if(checkable)
    {
        static QPainterPath e,e2,e3;
        if(e.isEmpty())
        {
            const int SIZE = 13;
            e.addEllipse(QPointF(16,16),SIZE, SIZE);
            e2.addEllipse(QPointF(16,16),SIZE - 2, SIZE - 2);
            e3.addEllipse(QPointF(16,16),SIZE - 4, SIZE - 4);
        }
        static int alpha;
        if(isSelected())
            alpha = 128;
        else
            alpha = 255;
        QLinearGradient l(16,1,16,31), l2(16,29,16,3);

        if(inputType == LogicSource::SWITCH)
        {
            l.setColorAt(0,QColor(255,0,0,alpha));
            l.setColorAt(1,QColor(255,200,200,alpha));
            l2.setColorAt(0,QColor(255,0,0,alpha));
            l2.setColorAt(1,QColor(255,200,200,alpha));
        }
        else if(inputType == LogicSource::BUTTON)
        {
            l.setColorAt(0,QColor(255,127,0,alpha));
            l.setColorAt(1,QColor(255,228,200,alpha));
            l2.setColorAt(0,QColor(255,127,0,alpha));
            l2.setColorAt(1,QColor(255,228,200,alpha));
        }

        painter->fillPath(e,l);
        painter->fillPath(e2,l2);
        if(checked)
            painter->fillPath(e3,l);
    }
    else if(inputType == LogicSource::SOURCE || inputType == LogicSource::GROUND)
    {
        QFont f = painter->font();
        f.setPixelSize(22);
        painter->setFont(f);
        QString text = (inputType == LogicSource::SOURCE)?QObject::tr("1"):QObject::tr("0");
        painter->drawText(this->boundingRect(),text, QTextOption(Qt::AlignCenter));
    }
    else if(inputType == LogicSource::CLOCK)
    {
        QPen pen(Qt::black,1);
        painter->strokePath(*clockPath,pen);
    }
}

void LogicSource::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //QGraphicsItem::mousePressEvent(event);
    *mousePressPos = event->scenePos();
    if(inputType == LogicSource::BUTTON)
    {
        checked = true;
        output[0]->setValue(1);
        update();
        output[0]->updateNext();
    }
}

void LogicSource::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    if(inputType == LogicSource::BUTTON && output[0]->pinValue())
    {
        checked = false;
        output[0]->setValue(0);
        update();
        output[0]->updateNext();
    }
}

void LogicSource::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    if(*mousePressPos != event->scenePos())
    {
        return;
    }
    if(inputType == LogicSource::SWITCH)
    {
        checked = !checked;
        output[0]->setValue((checked)?1:0);
        output[0]->update();
        output[0]->updateNext();
    }
    else if(inputType == LogicSource::BUTTON)
    {
        checked = false;
        output[0]->setValue(0);
        update();
        output[0]->updateNext();
    }
    this->setSelected(false);
}

void LogicSource::operate(Pin* source)
{   //this function is currently under the assumption that
    //the only logic source that operates is CLOCK, but this may change
    LogicState s;
    unsigned int v;

    switch(inputType)
    {
    case CLOCK:
        s = CURRENT;
        v = (output[0]->pinValue())?0:1;
        postEvent(v,s);
        break;
    default:;
    }
}

void LogicSource::setDefaultStateValue()
{
    if(inputType == LogicSource::SWITCH)
        output[0]->setValue(output[0]->pinValue());
    else
        OperatingComponent::setDefaultStateValue();
}

/*void LogicSource::reset()
{
    LogicSource::operate(0);
}*/

void LogicSource::setDelay(double d)
{
    OperatingComponent::setDelay(d);
    if(inputType == LogicSource::CLOCK)
    {
        graph->removeEvents(output[0]);
        reset();
    }
}

QAction* LogicSource::runContextMenu(QMenu *menu, QPoint screenPos)
{
    QAction *setDelayAction = menu->addAction("Set Delay..."), *executed = menu->exec(screenPos);

    if(executed)
    {
        if(executed == setDelayAction)
        {
            bool b = false;
            double d = QInputDialog::getDouble(OperatingComponent::getBoard()->parentWidget(),
                                               "Set Delay", "Delay: ", delay(), 0, 1000, 6, &b);
            if(b)
                setDelay(d);
        }
        update();
    }

    return executed;
}
