#include "pin.h"
#include "operatingcomponent.h"
#include "logicgate.h"
#include "logicsource.h"
#include <QInputDialog>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include "namelabel.h"
#include "circuitboard.h"
#include "connection.h"
#include <QBitmap>

QPainterPath* Pin::pinPath = 0;
QPainterPath* Pin::notPath = 0;
QString Pin::zString("Z");
QString Pin::xString("X");
Pin* Pin::current = 0;
int Pin::nextPinID = 0;
QMap<int, Pin*> Pin::pins;

Pin::Pin(Direction o, PinType t, bool n, OperatingComponent *parent): QGraphicsItem(parent), orientation(o),
    pType(t), value(0), width(1), notType(n), source(0), label(new NameLabel(this)), destructorActive(false)
{
    pid = nextPinID;
    pins[pid] = this;
    nextPinID++;

   //qDebug() << pid;

    if(t == OUTPUT)
    {
        destination = new QList<Pin*>;
        state = UNKNOWN;
        setUnique(true);
    }
    else
    {
        state = NO_CURRENT;
        destination = 0;
    }

    connections = new QList<Connection*>();

    if(!pinPath)
    {
        QPainterPath rect(QPointF(0.,-4.));
        rect.lineTo(20.,-4.);
        rect.lineTo(20.,4.);
        rect.lineTo(0.,4.);
        rect.closeSubpath();
        pinPath = new QPainterPath(QPointF(0.,0.));
        pinPath->lineTo(22.5,0.);
        pinPath->lineTo(24.,2.);
        pinPath->lineTo(22.5,4.);
        pinPath->lineTo(0.,4.);
        pinPath->closeSubpath();
        pinPath->translate(-4.,-2.);
        notPath = new QPainterPath(QPointF(0.,0.));
        notPath->addEllipse(QPointF(0.,0.),4.,4.);
        notPath->addPath(pinPath->intersected(rect).subtracted(*notPath));
    }

    switch(o)
    {
    case LEFT:
        pathRotation = 180;
        break;
    case RIGHT:
        pathRotation = 0;
        break;
    case UP:
        pathRotation = -90;
        break;
    case DOWN:
        pathRotation = 90;
    }

    updatePath();

    switch(o)
    {
    case LEFT:
        label->moveRight(myPath.boundingRect().left());
        break;
    case RIGHT:
        label->moveLeft(myPath.boundingRect().right());
        break;
    case UP:
        //label->moveLower(myPath.boundingRect().top());
        break;
    case DOWN:
        ;//label->moveUpper(myPath.boundingRect().bottom());
    }

    display = new QString*[3];
    display[NO_CURRENT] = &zString;
    display[UNKNOWN] = &xString;
    display[CURRENT] = new QString(QString::number(value,16));

    //setCacheMode(QGraphicsItem::ItemCoordinateCache);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlag(ItemIsSelectable);
    
    parentBoard()->insertPin(this);

    if(!(parentBoard()->propertyFlags() & CircuitBoard::DISPLAY_NAMES))
        label->hide();
}

Pin::Pin(bool uniq)
{
    pType = OUTPUT;
    setUnique(uniq);
    destination = 0;
    connections = 0;
    source = 0;
    label = 0;
}

Pin::~Pin()
{
    if(pins[pid] == this)
        pins.remove(pid);
    destructorActive = true;
    if(parentItem())
    {
        disconnect();
        parentBoard()->removePin(this);
        parentBoard()->removeEvents(this);
    }

    if(destination)
        delete destination;
    if(connections)
        delete connections;

    //if(scene())
    //    scene()->removeItem(this);
}

Pin::PinType Pin::pinType()
{
    return pType;
}

QRectF Pin::boundingRect() const
{
    QRectF rect = myPath.boundingRect();
    rect.adjust(-10,-10,20,20);
    return rect;
}

QPainterPath Pin::shape() const
{
    return myPath;
}

void Pin::updatePath()
{
    QMatrix matrix;
    matrix.rotate(pathRotation);
    myPath = (notType)?(matrix.map(*notPath)):(matrix.map(*pinPath));
    QPointF shifted(this->mapToParent(0.,0.));
    //qDebug() << parentItem()->shape().boundingRect();
    if(notType)
    {
        QPainterPath p;
        p.moveTo(-4,-4);
        p.arcTo(-4,-4,8,8,90,-180);
        p.arcTo(-4.01,-4.01,8.02,8.02,-90,180);
        myPath = myPath.subtracted(p);
    }
    myPath = myPath.translated(shifted).subtracted(parentItem()->shape()).translated(-shifted);

}

void Pin::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setClipRect(option->exposedRect);

    this->setFlag(ItemIsSelectable);
    foreach(QGraphicsItem *item, scene()->selectedItems())
    {
        if(!qgraphicsitem_cast<Pin *>(item))
        {
            this->setFlag(ItemIsSelectable, false);
            break;
        }
    }//if another item type is selected, pins aren't selectable;


    QBrush brush(Qt::white);
    QPen pen(Qt::black,1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin);
    if(parentItem()->isSelected()) // reflects parent's selection
    {
        brush.setColor(QColor(0,121,221,128));
        pen.setColor(QColor(0,121,221).darker());
        pen.setStyle(Qt::DotLine);
    }
    else if(isSelected()) // reflects own selection
    {
        brush.setColor(QColor(255,0,0,128));
        pen.setStyle(Qt::DotLine);
        pen.setColor(QColor(128,0,0));
    }

    painter->setBrush(brush);
    painter->setPen(pen);

    painter->drawPath(myPath);

    if(width > 1)
    {
        QPixmap texture = QBitmap(":/images/tex_1.png");
        brush.setTexture(texture);
        brush.setColor(pen.color());
        painter->fillPath(myPath, brush);
    }

    if(parentBoard()->propertyFlags() & CircuitBoard::SHOW_I_O)
    {
        QFont font(painter->font());
        font.setPixelSize(8);
        painter->setFont(font);
        QRectF rect;
        int flag = 0;

        switch(orientation)
        {
        case LEFT:
            rect = QRectF(-24,-10,20,8);
            flag = Qt::AlignRight; break;
        case RIGHT:
            rect = QRectF(4,-10,20,8);
            flag = Qt::AlignLeft; break;
        case UP:
            rect = QRectF(6,-8,20,8);
            flag = Qt::AlignLeft; break;
        case DOWN:
            rect = QRectF(6,-0,20,8);
            flag = Qt::AlignLeft; break;
        }

        painter->setPen(Qt::red);
        painter->drawText(rect, flag | Qt::AlignVCenter, *display[state]);
    }

}

void Pin::disconnect()
{
    while(!connections->isEmpty())
        delete connections->at(0);



    if(source == this)
    {
        foreach(Pin* pin, *destination)
        {
            pin->source = 0;
            if(pin != this)
            {
                pin->state = NO_CURRENT;
                pin->value = 0;
                pin->update();
                updatePin(pin);
            }
        }
        destination->clear();
        destination->append(this);
        source = this;
        return;
    }
    if(pType && destination)
    {
        foreach(Pin* pin, *destination)
        {
            pin->source = 0;
            pin->state = NO_CURRENT;
            pin->value = 0;
            pin->update();
            updatePin(pin);
        }
        destination->clear();
    }
    else
    {
        if(source)
        {
            source->destination->removeAll(this);
            source = 0;
            state = NO_CURRENT;
            value = 0;
            if(!destructorActive)
            {
                update();
                updatePin(this);
            }
        }
    }

}

void Pin::connectCurrentIfMatch()
{
    if(current && (current->name() == name()))
        connect(current);
}

bool Pin::connect(Pin *pin)
{
    if(width != pin->width)
        return false;
    if(pType && pin->pType) //OUTPUT == 1, two output pins are not connectable
        return false;
    if(pType)
    {
        if(pin->source != this)
            pin->disconnect();
        else
            return false;
        destination->append(pin);
        pin->source = this;
        pin->setValue(value);
        pin->state = state;
        if(pin->name() != name())
            pin->setName(name(), false); //this pin's connection has already been handled, don't connect it again
        pin->update();
        updatePin(pin);
        Connection *c = new Connection(this, pin);
        if(!(parentBoard()->propertyFlags() & CircuitBoard::DISPLAY_CONNECTIONS))
            c->hide();
        connections->append(c);
        pin->connections->append(c);
        return true;
    }
    if(pin->pType && source != pin) //at this point, this Pin is an input Pin, so check if pin is an output pin
    {
        return pin->connect(this);
    }
    //at this point, this Pin is an input Pin, but has no open connections or the other pin is also in input pin
    return false;
}

bool Pin::connect(int id)
{
    return connect(pins[id]);
}

bool Pin::connect(int i1, int i2)
{
    return pins[i1]->connect(pins[i2]);
}

void Pin::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << isSelected();
    if(event->button() == Qt::LeftButton)
    {
        event->accept();
        foreach(QGraphicsItem *item, scene()->selectedItems())
        {
            if(Pin *pin = qgraphicsitem_cast<Pin*>(item))
            {
                connect(pin);
            }
            else break; //since pins cannot be selected at the same time as other components,
                        //if selectedItems() contains a non-pin component, no further seach is necessary
        }
    }
}

void Pin::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem*> sI = scene()->selectedItems();
    if(!sI.isEmpty() && qgraphicsitem_cast<Pin*>(sI[0]) != this) //if other pins are selected
    {
        foreach(QGraphicsItem* item, sI)
            item->setSelected(false);
        this->setSelected(false);
    }
    else
        setSelected(true);
}

void Pin::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    bool ok;
    QMenu menu;
    QAction *dis = menu.addAction("Clear"),
            *rename = menu.addAction("Set Name..."), *selected;
    dis->setShortcut(QKeySequence("DELETE"));

    selected = menu.exec(event->screenPos());

    if(selected)
    {
        if(selected == rename)
        {
            QString text = QInputDialog::getText(parentBoard(), QObject::tr("Pin: ") + name(),
                                                    QObject::tr("New Pin Name:"), QLineEdit::Normal,name(), &ok);
            if(ok)
            {
                setName(text);
            }
        }
        else if(selected == dis)
        {
            disconnect();
            setName("");
        }
    }
}

void Pin::setValue(int i)
{
    if(state == CURRENT && value == i)
        return;
    delete display[CURRENT];
    display[CURRENT] = new QString(QString::number(i,16));
    value = i;

    setState(CURRENT);

    if(pType)
    {
        foreach(Pin* pin, *destination)
            pin->setValue(i);
        foreach(Connection* c, *connections)
            c->update();
    }
//    else
  //  {
    //    parentComponent()->operate(this);
    //}

}

void Pin::setState(LogicState s)
{
    if(state == s)
        return;
    state = s;
    if(pType)
    {
        foreach(Pin* pin, *destination)
            pin->setState(s);
        foreach(Connection* c, *connections)
            c->update();
    }
}

void Pin::setName(QString s, bool reconnect)
{
    if(s == name())
        return;

    if(pType == OUTPUT && s != "") // no two output pins may have the same name
    {
        Pin* testPin = new Pin(true);
        testPin->Component::setName(s);
        if(parentBoard()->containsOne(testPin))
        {
            delete testPin;
            return;
        }
        delete testPin;
    }

    if(destination)
        foreach(Pin* pin, *destination)
        {
            if(pin != this)
            {
                pin->Component::setName(s);
                pin->label->updateName();
            }
        }

    if(pinType() == OUTPUT)//this is necessary in order to preserve the order of output pins
        parentBoard()->removePin(this);

    Component::setName(s);
    label->updateName();

    if(s != "" && reconnect)
    {
        disconnect();

        current = this;
        parentBoard()->passConnect(this);
    }

    if(pinType())
    {
        parentBoard()->insertPin(this);
        foreach(Connection* c, *connections)
            c->updatePath();
    }
    
    if(source && source->name() != name())
        disconnect();
}

Direction Pin::dir()
{
    return orientation;
}

int Pin::pinWidth()
{
    return width;
}

unsigned int Pin::pinValue()
{
    return value;
}

LogicState Pin::pinState()
{
    return state;
}

CircuitBoard* Pin::parentBoard()
{
    return parentComponent()->getBoard();
}

LogicGate* Pin::parentGate()
{
    return (LogicGate*)parentItem();
}

OperatingComponent* Pin::parentComponent()
{
    return qgraphicsitem_cast<OperatingComponent*>(parentItem());
}

void Pin::updatePin(Pin *pin)
{
    pin->parentComponent()->operate(pin);
}

void Pin::updateNext()
{
    //double targetTime = parentBoard()->time() + parentComponent()->delay();
//    unsigned int v;
//    LogicState s;
    foreach(Pin* pin, *destination)
    {
        //double targetTime = parentBoard()->time() + pin->parentComponent()->delay();
        if(state)
            pin->setState(state);
        else
            pin->setValue(value);
        pin->update();

        pin->parentComponent()->operate(pin);
        //Pin* output = pin->parentComponent()->outputPin();

        //if(output->value != v || output->state != s)
            //parentBoard()->addEvent(output, targetTime, v, s);
        //else if(parentBoard()->contains(output, targetTime))
        //    parentBoard()->removeEvent(output, targetTime);
    }
}

bool Pin::exists()
{
   //qDebug() << (int)label << " " << name();
    return (long long)label;
}

void Pin::resetState()
{
    parentComponent()->setDefaultStateValue();
}

void Pin::showLabel()
{
    label->show();
}

void Pin::hideLabel()
{
    label->hide();
}

bool Pin::labelVisible()
{
    return label->isVisible();
}

void Pin::showConnections()
{
    foreach(Connection *c, *connections)
        c->show();
}

void Pin::hideConnections()
{
    foreach(Connection *c, *connections)
        c->hide();
}

void Pin::setPinID(int id)
{
    if(pins[pid] == this) //in case this pin ID will not be used again, remove it
        pins.remove(pid);
    if(pins[id] != 0) //in case the new id is already occupied, clear it
        pins.remove(id);
    pins[id] = this;
    pid = id;
    if(id + 1 > nextPinID)
    {
        nextPinID = id + 1;
    }
}

int Pin::pinID()
{
    return pid;
}

void Pin::setPinWidth(int w)
{
    if(w < 1 || w > 32)
        return;
    if(w == width)
        return;
    disconnect();
    width = w;

    setToolTip(QString::number(w,10));
}
