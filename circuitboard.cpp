#include "circuitboard.h"
#include <QWheelEvent>
#include <math.h>
#include "logicgate.h"
#include "logicsource.h"
#include "pin.h"
#include "bulbcomponent.h"
//#include <QDebug>
#include "led_component.h"
#include "mainwindow.h"
#include <QProgressBar>
#include <QStatusBar>
#include <qmath.h>
#include <QtOpenGL/QGLWidget>

CircuitBoard::CircuitBoard(QWidget *parent) : QGraphicsView(parent),
    propertyFlag(DISPLAY_CONNECTIONS | DISPLAY_NAMES),inputPinTree(new BinarySearchTree<Pin>()),
    outputPinTree(new BinarySearchTree<Pin>()),eventTree(new BinarySearchTree<LogicEvent>()),
    currentPin(0), newComponentFlag(0), simulationTime(0)
{   
    eventTree->setDeleteOnRemove(true);

    //setViewport(new QGLWidget);

    /*dComponent.insert(BUFF << 10, new LogicGate(LogicGate::BUFFER,this));
    dComponent.insert(NOTF << 10, new LogicGate(LogicGate::NOT,this));
    for(int i = 2; i < 5; i++)
    {
        int key;
        key = (ANDF << 10) | (i - 1);
        dComponent.insert(key, new LogicGate(LogicGate::AND,this));
        ((LogicGate*)dComponent[key])->setPinCount(i);

        key = (NANDF << 10) | (i - 1);
        dComponent.insert(key, new LogicGate(LogicGate::NAND,this));
        ((LogicGate*)dComponent[key])->setPinCount(i);

        key = (ORF << 10) | (i - 1);
        dComponent.insert(key, new LogicGate(LogicGate::OR,this));
        ((LogicGate*)dComponent[key])->setPinCount(i);

        key = (NORF << 10) | (i - 1);
        dComponent.insert(key, new LogicGate(LogicGate::NOR,this));
        ((LogicGate*)dComponent[key])->setPinCount(i);
    }

    dComponent.insert(XORF << 10, new LogicGate(LogicGate::XOR,this));
    dComponent.insert(XNORF << 10, new LogicGate(LogicGate::XNOR,this));
    dComponent.insert(SOURCEF << 10, new LogicSource(LogicSource::SOURCE,this));
    dComponent.insert(GROUNDF << 10,new LogicSource(LogicSource::GROUND,this));
    dComponent.insert(SWITCHF << 10,new LogicSource(LogicSource::SWITCH,this));
    dComponent.insert(BUTTONF << 10,new LogicSource(LogicSource::BUTTON,this));
    dComponent.insert(CLOCKF << 10,new LogicSource(LogicSource::CLOCK,this));
    dComponent.insert(BULBF << 10,new BulbComponent(this));
    dComponent.insert(LED_7F << 10,new LED_Component(this,LED_Component::LED_7));
    dComponent.insert(LED_14F << 10,new LED_Component(this,LED_Component::LED_14));
    dComponent.insert(LED_16F << 10,new LED_Component(this,LED_Component::LED_16));

    int key;

    key = (COUNTERF << 10) | (0);
    dComponent.insert(key, new LogicGate(LogicGate::COUNTER,this));
    ((LogicGate*)dComponent[key])->setWidth(1);

    key = (REGISTERF << 10) | (0);
    dComponent.insert(key, new LogicGate(LogicGate::REGISTER,this));
    ((LogicGate*)dComponent[key])->setWidth(1);

    for(int i = 2; i < 33; i++)
    {
        key = (MERGERF << 10) | ((i - 1) << 5);
        dComponent.insert(key, new LogicGate(LogicGate::MERGER,this));
        ((LogicGate*)dComponent[key])->setWidth(i);

        key = (SPLITTERF << 10) | ((i - 1) << 5);
        dComponent.insert(key, new LogicGate(LogicGate::SPLITTER,this));
        ((LogicGate*)dComponent[key])->setWidth(i);

        key = (COUNTERF << 10) | ((i - 1) << 5);
        dComponent.insert(key, new LogicGate(LogicGate::COUNTER,this));
        ((LogicGate*)dComponent[key])->setWidth(i);

        key = (REGISTERF << 10) | ((i - 1) << 5);
        dComponent.insert(key, new LogicGate(LogicGate::REGISTER,this));
        ((LogicGate*)dComponent[key])->setWidth(i);
    }

    for(int i = 1; i <= 5; i++)
    {
        key = (MULTIPLEXERF << 10) | (i - 1);
        dComponent.insert(key, new LogicGate(LogicGate::MULTIPLEXER,this));
        ((LogicGate*)dComponent[key])->setMuxWidth(i);

        key = (DECODERF << 10) | (i - 1);
        dComponent.insert(key, new LogicGate(LogicGate::DECODER,this));
        ((LogicGate*)dComponent[key])->setMuxWidth(i);
    }

    dComponent.insert(D_FFF << 10,new LogicGate(LogicGate::D_FF,this));
    dComponent.insert(T_FFF << 10,new LogicGate(LogicGate::T_FF,this));
    dComponent.insert(J_K_FFF << 10,new LogicGate(LogicGate::J_K_FF,this));
    */

    dComponent.insert(BUFF, new LogicGate(LogicGate::BUFFER,this));
    dComponent.insert(NOTF, new LogicGate(LogicGate::NOT,this));

    dComponent.insert(ANDF, new LogicGate(LogicGate::AND,this));
    dComponent.insert(NANDF, new LogicGate(LogicGate::NAND,this));
    dComponent.insert(ORF, new LogicGate(LogicGate::OR,this));
    dComponent.insert(NORF, new LogicGate(LogicGate::NOR,this));

    dComponent.insert(XORF, new LogicGate(LogicGate::XOR,this));
    dComponent.insert(XNORF, new LogicGate(LogicGate::XNOR,this));
    dComponent.insert(SOURCEF, new LogicSource(LogicSource::SOURCE,this));
    dComponent.insert(GROUNDF,new LogicSource(LogicSource::GROUND,this));
    dComponent.insert(SWITCHF,new LogicSource(LogicSource::SWITCH,this));
    dComponent.insert(BUTTONF,new LogicSource(LogicSource::BUTTON,this));
    dComponent.insert(CLOCKF,new LogicSource(LogicSource::CLOCK,this));
    this->removeEvents(dComponent[CLOCKF]->outputPins()[0]);

    dComponent.insert(BULBF,new BulbComponent(this));
    dComponent.insert(LED_7F,new LED_Component(this,LED_Component::LED_7));
    dComponent.insert(LED_14F,new LED_Component(this,LED_Component::LED_14));
    dComponent.insert(LED_16F,new LED_Component(this,LED_Component::LED_16));

    dComponent.insert(MERGERF, new LogicGate(LogicGate::MERGER,this));
    dComponent.insert(SPLITTERF, new LogicGate(LogicGate::SPLITTER,this));
    dComponent.insert(COUNTERF, new LogicGate(LogicGate::COUNTER,this));
    dComponent.insert(REGISTERF, new LogicGate(LogicGate::REGISTER,this));

    dComponent.insert(MULTIPLEXERF, new LogicGate(LogicGate::MULTIPLEXER,this));
    dComponent.insert(DECODERF, new LogicGate(LogicGate::DECODER,this));

    dComponent.insert(D_FFF,new LogicGate(LogicGate::D_FF,this));
    dComponent.insert(T_FFF,new LogicGate(LogicGate::T_FF,this));
    dComponent.insert(J_K_FFF,new LogicGate(LogicGate::J_K_FF,this));

    dComponent.insert(L_COUNTERF,new LogicGate(LogicGate::L_COUNTER,this));
    dComponent.insert(ADDERF,new LogicGate(LogicGate::ADDER,this));
    dComponent.insert(SL_REGISTERF,new LogicGate(LogicGate::SL_REGISTER,this));
    dComponent.insert(SR_REGISTERF,new LogicGate(LogicGate::SR_REGISTER,this));
    dComponent.insert(SB_REGISTERF,new LogicGate(LogicGate::SB_REGISTER,this));
    dComponent.insert(ENCODERF,new LogicGate(LogicGate::ENCODER,this));

    foreach(OperatingComponent* oc, dComponent)
    {
        foreach(Pin *o, oc->outputPins())
            o->setName("");
        oc->setZValue(10);
        oc->setDummy();
    }

    appendSelection = ctrlHeld = false;

    gridSize = 25;

    setDragMode(QGraphicsView::RubberBandDrag);

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->installEventFilter(this);
    //scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-(1 << 15),-(1 << 15), 1 << 16, 1 << 16);
    setScene(scene);

    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    QRectF rect(scene->sceneRect());
    rect.translate(-300,-200);
    //scene->setSceneRect(rect);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);              //antialiased rendering
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMinimumSize(600, 400);
    setWindowTitle(tr("Logic Sim"));
}

void CircuitBoard::insertPin(Pin *pin)
{
    if(pin->pinType() == Pin::OUTPUT)
        outputPinTree->insert(pin);
    else
        inputPinTree->insert(pin);
}

void CircuitBoard::removePin(Pin *pin)
{
    if(pin->pinType() == Pin::OUTPUT)
    {
        //outputPinTree->print();
        //qDebug() << "REMOVING OUTPUT";
        outputPinTree->remove(pin);
        //outputPinTree->print();
    }
    else
    {
        //inputPinTree->print();
        //qDebug() << "REMOVING INPUT";
        inputPinTree->remove(pin);
        //inputPinTree->print();
    }
}

void CircuitBoard::passConnect(Pin *pin)
{
    if(pin->pinType())
    {
        inputPinTree->execute(&Pin::connectCurrentIfMatch);
    }
    else
    {
        outputPinTree->execute(&Pin::connectCurrentIfMatch);
    }
}

void CircuitBoard::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, event->delta() / 1000.));
}

void CircuitBoard::scaleView(qreal scaleFactor)
{
    qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.25 || factor > 4)
        return;

    scale(scaleFactor, scaleFactor);
}

void CircuitBoard::drawBackground(QPainter *painter, const QRectF &rect)
{
    //Q_UNUSED(rect);
    //painter->fillRect(rect,Qt::white);
    painter->setClipRect(rect);
    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    painter->fillRect(rect.intersect(sceneRect),Qt::lightGray);

    //grid

    QPen pen(QColor(128,128,128,128),0);
    painter->save();

    painter->setPen(pen);
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    for(int i = (int)(rect.x()) / gridSize * gridSize; i <= rect.width() + rect.x(); i += gridSize)
    {
        painter->drawLine(QPointF(i, rect.y()), QPointF(i, rect.y() + rect.height()));
    }
    for(int i = (int)(rect.y()) / gridSize * gridSize; i <= rect.height() + rect.y(); i+= gridSize)
    {
        painter->drawLine(QPointF(rect.x(),i), QPointF(rect.x() + rect.width(), i));
    }

    //painter->fillRect(QRect(5,5,32,32),Qt::black);

    painter->restore();
}

/*void CircuitBoard::drawForeground(QPainter *painter, const QRectF &rect)
{
    painter->setClipRect(rect);

    painter->setBrush(Qt::white);
    painter->setPen(Qt::black);

    painter->drawText(mapToScene(2,25),QString::number(simulationTime),(int)Qt::AlignBottom,1);
}*/

void CircuitBoard::mousePressEvent(QMouseEvent *event)
{
    //qDebug() << event->pos();
    if(event->button() == Qt::RightButton)
    {
        return;
    }
    if(ctrlHeld)
        appendSelection = true;
    if(appendSelection)
    {
        selection.clear();
        selection.append(scene()->selectedItems());
    }
    if(newComponentFlag)
    {
        OperatingComponent *oc = createComponent(newComponentFlag, mapToScene(event->pos()));
        oc->moveBy(-oc->center().x(), -oc->center().y());

    }
    else
        QGraphicsView::mousePressEvent(event);

    foreach(QGraphicsItem* item, selection)
    {
        item->setSelected(true);
    }
}

void CircuitBoard::mouseMoveEvent(QMouseEvent *event)
{

    if(event->buttons() != Qt::NoButton)
    {
        if(appendSelection)
        {
            selection.append(scene()->selectedItems());
        }
        QGraphicsView::mouseMoveEvent(event);
        foreach(QGraphicsItem* item, selection)
        {
            item->setSelected(true);
        }
        appendSelection = false;
    }
    else
    {
        if(newComponentFlag)
        {
            int key = typePart(newComponentFlag);
            //key = (key & (ANDF | ORF))?key:(key & ~7);
            dComponent[key]->setPos(mapToScene(event->pos() - matrix().map(dComponent[key]->center())));
        }
        QGraphicsView::mouseMoveEvent(event); //
    }
}

void CircuitBoard::keyPressEvent(QKeyEvent *event)
{
    int moveX = 0, moveY = 0;

    switch(event->key())
    {
    case Qt::Key_Control:
        appendSelection = ctrlHeld = true;
        event->accept();
        break;
    case Qt::Key_Up:
        moveY = -1;
        event->accept();
        break;
    case Qt::Key_Down:
        moveY = 1;
        event->accept();
        break;
    case Qt::Key_Left:
        moveX = -1;
        event->accept();
        break;
    case Qt::Key_Right:
        moveX = 1;
        event->accept();
        break;
    //case Qt::Key_Delete:
    //    deleteSelected();
    //    break;
    case Qt::Key_W:
        if(scene()->selectedItems().count() == 1)
        {
            Pin* pin = qgraphicsitem_cast<Pin*>(scene()->selectedItems().at(0));
            if(pin)
            {
                foreach(QGraphicsItem* item, scene()->items())
                {
                    if(Pin* p = qgraphicsitem_cast<Pin*>(item))
                    {
                        if(pin->pinType() != p->pinType() && pin->pinWidth() == p->pinWidth())
                            p->setSelected(true);
                    }
                }
            }
        }
        break;
    case Qt::Key_Q:
        if(scene()->selectedItems().count() == 1)
        {
            Pin* pin = qgraphicsitem_cast<Pin*>(scene()->selectedItems().at(0));
            if(pin)
            {
                foreach(QGraphicsItem* item, scene()->items())
                {
                    if(Pin* p = qgraphicsitem_cast<Pin*>(item))
                    {
                        if(pin->pinType() != p->pinType() && pin->pinWidth() == p->pinWidth() &&
                           ((pin->pinType() == Pin::OUTPUT)?(!p->source):(true)))
                            p->setSelected(true);
                    }
                }
            }
        }
        break;
    case Qt::Key_V:
        if((event->modifiers() & Qt::ControlModifier))
        {
            qDebug() << "Pasting";
            paste();
        }
        break;
    case Qt::Key_C:
        if((event->modifiers() & Qt::ControlModifier))
        {
            qDebug() << "Copying";
            copy();
        }
        else if(scene()->selectedItems().count() == 1)
        {
            Pin* pin = qgraphicsitem_cast<Pin*>(scene()->selectedItems().at(0));
            if(pin)
            {
                if(pin->destination)
                    foreach(Pin *d, *(pin->destination))
                    {
                        d->setSelected(true);
                    }
                else if(pin->source)
                    pin->source->setSelected(true);
            }
        }
        break;
    case Qt::Key_Space:
        if(scene()->selectedItems().count() == 1)
        {
            Pin* pin = qgraphicsitem_cast<Pin*>(scene()->selectedItems().at(0));
            if(pin)
            {
                bool ok;
                QString text = QInputDialog::getText(this, QObject::tr("Pin: ") + pin->name(),
                                                        QObject::tr("New Pin Name:"), QLineEdit::Normal,pin->name(), &ok);
                if(ok)
                {
                    pin->setName(text);
                }
            }
        }
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
    if(this->dragMode() != QGraphicsView::ScrollHandDrag && (moveX || moveY))
    {
        foreach(QGraphicsItem* item, scene()->selectedItems())
        {
            if(OperatingComponent* oc = qgraphicsitem_cast<OperatingComponent*>(item))
                oc->moveBy(moveX, moveY);
        }
    }
}

void CircuitBoard::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Control:
        appendSelection = ctrlHeld = false;
        selection.clear();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

bool CircuitBoard::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == 24 || event->type() == 8 || event->type() == 25 || event->type() == 9 ||
       event->type() == 9 || event->type() == 155 || event->type() == 11 || event->type() == 10 ||
       event->type() == 163 || event->type() == 43)
        return false;
    //this is necessary to prevent the screen from panning with a keypress when other behaviors are desired
    //156 is QGraphicsSceneMousePressEvent, this is the behavior that selects a QGraphicsItem on a click
    // modify this behavior!
    if(this->dragMode() != QGraphicsView::ScrollHandDrag &&
       (/*event->type() == QEvent::KeyPress || */event->type() == QEvent::KeyRelease))
        return true;

    return false;
}

//checks whether eventTree contains pin an event for pin at time
bool CircuitBoard::contains(Pin *pin, double time)
{
    LogicEvent event(pin,time);
    return eventTree->find(event).target;
}

//checks whether the appropriate tree contains pin
bool CircuitBoard::contains(Pin *pin)
{
    if(pin->pinType())
        return &outputPinTree->find(pin) == pin;
    else
        return &inputPinTree->find(pin) == pin;
}

//checks whether the appropriate tree contains a pin such that the (found pin) == *pin
bool CircuitBoard::containsOne(Pin *pin)
{
    //outputPinTree->print();
    //outputPinTree->levelOrderPrint();
    if(pin->pinType() == Pin::OUTPUT)
    {
        QString n1 = outputPinTree->findFirst(pin).name();
        QString n2 = pin->name();

       //qDebug() << n1 << " " << n2;

        return n1 == n2;
    }
    else
    {
       //qDebug() << "INPUT!!!";
        return inputPinTree->findFirst(pin).name() == pin->name();
    }
}

double CircuitBoard::time()
{
    return simulationTime;
}

//creates and queues a logic event to occur such that pin->value is set to value
//  and pin->state is set to state at time targetTime
//
//if an event already exists with the same pin and targetTime, it is replaced by the new event
void CircuitBoard::addEvent(Pin *pin, double targetTime, unsigned int value, LogicState state)
{
    LogicEvent *e = new LogicEvent(pin,targetTime,value, state);
    eventTree->remove(e); //if the target has an event at targetTime
    eventTree->insert(e);
}

//
void CircuitBoard::removeEvent(Pin *pin, double targetTime)
{
    eventTree->remove(new LogicEvent(pin,targetTime));
}

void CircuitBoard::removeEvents(Pin *pin)
{
    LogicEvent::setToBeDeleted(pin);
    eventTree->execute(&LogicEvent::enqueueIfMatch);
    while(!LogicEvent::queue.isEmpty())
    {
        eventTree->remove(LogicEvent::queue.dequeue());
    }
}

double CircuitBoard::step()
{
    if(!eventTree->isEmpty())// && time() <= stopTime)
    {
        double newTime = eventTree->peek()->time;
        if(time() < newTime)
            simulationTime = newTime;

        if(eventTree->peek()->target->name() == "_set0'")
        {
            eventTree->print();
            eventTree->executeFirst(&LogicEvent::operate);
            eventTree->print();
        }
        else
        {
            eventTree->executeFirst(&LogicEvent::operate);
        }
    }
    return simulationTime;
}

double CircuitBoard::timeStep()
{
    if(!eventTree->isEmpty())
    {
        double stepTo = eventTree->peek()->time;
        while(!eventTree->isEmpty() && eventTree->peek()->time == stepTo)
            step();
    }
    return simulationTime;
}

double CircuitBoard::timeStep(double stepSize)
{
    double stepTo = time() + stepSize;
    while(!eventTree->isEmpty() && eventTree->peek()->time <= stepTo)
        step();
    simulationTime = stepTo; 

    return simulationTime;

    //((MainWindow*)parentWidget())->setStatus(QString::number(stepTo,'f',3).append(" ns"));
}

void CircuitBoard::setBuildCFlags(int flags)
{
    if(newComponentFlag)
    {
        int key = typePart(newComponentFlag);
        scene()->removeItem(dComponent[key]);
    }
    if(flags)
    {
        int key = typePart(flags);
        //key = (key & (ANDF | ORF))?key:(key & ~7);
        //dComponent[key]->setPos(this->sceneRect().topLeft() - QPoint(1000,1000));

        switch(key)
        {
        case BUFF:
        case NOTF:
        ((LogicGate*)dComponent[key])->setWidth(widthPart(flags));
        break;

        case ANDF:
        case NANDF:
        case ORF:
        case NORF:
        ((LogicGate*)dComponent[key])->setWidth(widthPart(flags));
        ((LogicGate*)dComponent[key])->setPinCount(countPart(flags));
        break;

        case XORF:
        case XNORF:
        ((LogicGate*)dComponent[key])->setWidth(widthPart(flags));
        break;

        case BUTTONF:
        case SWITCHF:
        case GROUNDF:
        case SOURCEF:
        case CLOCKF:
        case BULBF:
        case LED_7F:
        case LED_14F:
        case LED_16F:
        break;

        case MERGERF:
        case SPLITTERF:
            ((LogicGate*)dComponent[key])->setWidth(widthPart(flags));
        break;

        case MULTIPLEXERF:
        case DECODERF:
        case ENCODERF:
            ((LogicGate*)dComponent[key])->setMuxWidth(countPart(flags));
            ((LogicGate*)dComponent[key])->setWidth(widthPart(flags));
        break;

        case REGISTERF:
        case COUNTERF:
        case L_COUNTERF:
        case ADDERF:
        case SL_REGISTERF:
        case SR_REGISTERF:
        case SB_REGISTERF:
            ((LogicGate*)dComponent[key])->setWidth(widthPart(flags));
        break;
        case D_FFF:
        case T_FFF:
        case J_K_FFF:;
        }

        scene()->addItem(dComponent[key]);
        dComponent[key]->show();
    }
    newComponentFlag = flags;
}

void CircuitBoard::unsetBuildCFlag()
{
    newComponentFlag = 0;
}

unsigned int CircuitBoard::propertyFlags()
{
    return propertyFlag;
}

void CircuitBoard::setPropertyFlagTo(int flag)
{
    setPropertyFlag(-1,false);
    setPropertyFlag(flag, true);
}

void CircuitBoard::setPropertyFlag(int flag, bool value)
{
    if(value)
        propertyFlag |= flag;
    else
        propertyFlag &= ~flag;

    if(flag & CircuitBoard::DISPLAY_NAMES)
    {
        if(value)
        {
            if(!inputPinTree->isEmpty() && !inputPinTree->peek()->labelVisible())
                inputPinTree->execute(&Pin::showLabel);
            if(!outputPinTree->isEmpty() && !outputPinTree->peek()->labelVisible())
                outputPinTree->execute(&Pin::showLabel);
        }
        else
        {
            if(!inputPinTree->isEmpty() && inputPinTree->peek()->labelVisible())
                inputPinTree->execute(&Pin::hideLabel);
            if(!outputPinTree->isEmpty() && outputPinTree->peek()->labelVisible())
                outputPinTree->execute(&Pin::hideLabel);
        }
    }
    if(flag & CircuitBoard::SHOW_I_O)
    {
        foreach(QGraphicsItem* item, scene()->items())
        {
            if(Pin* pin = qgraphicsitem_cast<Pin*>(item))
                pin->update();
        }
    }
    if(flag & CircuitBoard::DISPLAY_CONNECTIONS)
    {
        if(value)
        {
            if(!outputPinTree->isEmpty())
                outputPinTree->execute(&Pin::showConnections);
        }
        else
        {
            if(!outputPinTree->isEmpty())
                outputPinTree->execute(&Pin::hideConnections);
        }
    }
}

void CircuitBoard::setCurrent(OperatingComponent *current)
{
    this->currentOC = current;
}

int CircuitBoard::typePart(int f)
{
    return f >> 10;
}

int CircuitBoard::widthPart(int f)
{
    return ((f >> 5) & 31) + 1;
}

int CircuitBoard::countPart(int f)
{
    return (f & 31) + 1;
}

bool CircuitBoard::isPart(int f, int from)
{
    return f == typePart(from);
}

//w and c are assumed to be a value from 1 to 32
int CircuitBoard::createFlag(int t, int w, int c)
{
    int i = (t << 10) | ((w - 1) << 5) | (c - 1);
    return i;
}

int CircuitBoard::createFlag(OperatingComponent *oc)
{
    int t = 0, w = 1, c = 1;

    if(LogicGate *gate = qgraphicsitem_cast<LogicGate*>(oc))
    {
        w = gate->getWidth();
        switch(gate->logicType())
        {
        case LogicGate::BUFFER:
            t = BUFF;
            break;
        case LogicGate::NOT:
            t = NOTF;
            break;
        case LogicGate::AND:
            t = ANDF;
            c = gate->inputPinCount();
            break;
        case LogicGate::NAND:
            t = NANDF;
            c = gate->inputPinCount();
            break;
        case LogicGate::OR:
            t = ORF;
            c = gate->inputPinCount();
            break;
        case LogicGate::NOR:
            t = NORF;
            c = gate->inputPinCount();
            break;
        case LogicGate::XOR:
            t = XORF;
            break;
        case LogicGate::XNOR:
            t = XNORF;
            break;
        case LogicGate::MERGER:
            t = MERGERF;
            break;
        case LogicGate::SPLITTER:
            t = SPLITTERF;
            break;
        case LogicGate::COUNTER:
            t = COUNTERF;
            break;
        case LogicGate::REGISTER:
            t = REGISTERF;
            break;
        case LogicGate::MULTIPLEXER:
            t = MULTIPLEXERF;
            c = gate->getMuxWidth();
            break;
        case LogicGate::DECODER:
            t = DECODERF;
            c = gate->getMuxWidth();
            break;
        case LogicGate::D_FF:
            t = D_FFF;
            break;
        case LogicGate::J_K_FF:
            t = J_K_FFF;
            break;
        case LogicGate::T_FF:
            t = T_FFF;
            break;
        case LogicGate::L_COUNTER:
            t = L_COUNTERF;
            break;
        case LogicGate::ADDER:
            t = ADDERF;
            break;
        case LogicGate::SL_REGISTER:
            t = SL_REGISTERF;
            break;
        case LogicGate::SR_REGISTER:
            t = SR_REGISTERF;
            break;
        case LogicGate::SB_REGISTER:
            t = SB_REGISTERF;
            break;
        case LogicGate::ENCODER:
            t = ENCODERF;
            c = gate->getMuxWidth();
            break;
        }
    }
    else if(LogicSource *source = qgraphicsitem_cast<LogicSource*>(oc))
    {
        switch(source->sourceType())
        {//BUTTON, SWITCH, CLOCK, SOURCE, GROUND
        case LogicSource::BUTTON:
            t = BUTTONF;
            break;
        case LogicSource::SWITCH:
            t = SWITCHF;
            break;
        case LogicSource::CLOCK:
            t = CLOCKF;
            break;
        case LogicSource::SOURCE:
            t = SOURCEF;
            break;
        case LogicSource::GROUND:
            t = GROUNDF;
            break;
        }
    }
    else if(LED_Component *led = qgraphicsitem_cast<LED_Component*>(oc))
    {
        switch(led->lType())
        {
        case LED_Component::LED_7:
            t = LED_7F;
            break;
        case LED_Component::LED_14:
            t = LED_14F;
            break;
        case LED_Component::LED_16:
            t = LED_16F;
            break;
        }
    }
    else if(BulbComponent *bulb = qgraphicsitem_cast<BulbComponent*>(oc))
    {
        t = BULBF;
    }

    return createFlag(t,w,c);
}

OperatingComponent* CircuitBoard::createComponent(int flag, QPointF pos, int properties)
{
    OperatingComponent* component = 0;
    if(isPart(BUFF, flag))
    {
        component = new LogicGate(LogicGate::BUFFER,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(NOTF, flag))
    {
        component = new LogicGate(LogicGate::NOT,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(ANDF, flag))
    {
        component = new LogicGate(LogicGate::AND,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setPinCount(countPart(flag));
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(NANDF, flag))
    {
        component = new LogicGate(LogicGate::NAND,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setPinCount(countPart(flag));
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(ORF, flag))
    {
        component = new LogicGate(LogicGate::OR,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setPinCount(countPart(flag));
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(NORF, flag))
    {
        component = new LogicGate(LogicGate::NOR,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setPinCount(countPart(flag));
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(XORF, flag))
    {
        component = new LogicGate(LogicGate::XOR,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(XNORF, flag))
    {
        component = new LogicGate(LogicGate::XNOR,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(SOURCEF, flag))
    {
        component = new LogicSource(LogicSource::SOURCE,this);
        component->setPropertiesTo(properties);
    }
    else if(isPart(GROUNDF, flag))
    {
        component = new LogicSource(LogicSource::GROUND,this);
        component->setPropertiesTo(properties);
    }
    else if(isPart(SWITCHF, flag))
    {
        component = new LogicSource(LogicSource::SWITCH,this);
        component->setPropertiesTo(properties);
    }
    else if(isPart(BUTTONF, flag))
    {
        component = new LogicSource(LogicSource::BUTTON,this);
        component->setPropertiesTo(properties);
    }
    else if(isPart(CLOCKF, flag))
    {
        component = new LogicSource(LogicSource::CLOCK,this);
        component->setPropertiesTo(properties);
        ((LogicSource*)component)->operate(0);
    }
    else if(isPart(BULBF, flag))
    {
        component = new BulbComponent(this);
        component->setPropertiesTo(properties);
    }
    else if(isPart(LED_7F, flag))
    {
        component = new LED_Component(this, LED_Component::LED_7);
        component->setPropertiesTo(properties);
    }
    else if(isPart(LED_14F, flag))
    {
        component = new LED_Component(this, LED_Component::LED_14);
        component->setPropertiesTo(properties);
    }
    else if(isPart(LED_16F, flag))
    {
        component = new LED_Component(this, LED_Component::LED_16);
        component->setPropertiesTo(properties);
    }
    else if(isPart(MERGERF, flag))
    {
        component = new LogicGate(LogicGate::MERGER,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(SPLITTERF, flag))
    {
        component = new LogicGate(LogicGate::SPLITTER,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(MULTIPLEXERF, flag))
    {
        component = new LogicGate(LogicGate::MULTIPLEXER,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setMuxWidth(countPart(flag));
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(COUNTERF, flag))
    {
        component = new LogicGate(LogicGate::COUNTER,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(REGISTERF, flag))
    {
        component = new LogicGate(LogicGate::REGISTER,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(DECODERF, flag))
    {
        component = new LogicGate(LogicGate::DECODER,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setMuxWidth(countPart(flag));
    }
    else if(isPart(D_FFF, flag))
    {
        component = new LogicGate(LogicGate::D_FF,this);
        component->setPropertiesTo(properties);
    }
    else if(isPart(T_FFF, flag))
    {
        component = new LogicGate(LogicGate::T_FF,this);
        component->setPropertiesTo(properties);
    }
    else if(isPart(J_K_FFF, flag))
    {
        component = new LogicGate(LogicGate::J_K_FF,this);
        component->setPropertiesTo(properties);
    }
    else if(isPart(L_COUNTERF, flag))
    {
        component = new LogicGate(LogicGate::L_COUNTER,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(ADDERF, flag))
    {
        component = new LogicGate(LogicGate::ADDER,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(SL_REGISTERF, flag))
    {
        component = new LogicGate(LogicGate::SL_REGISTER,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(SR_REGISTERF, flag))
    {
        component = new LogicGate(LogicGate::SR_REGISTER,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(SB_REGISTERF, flag))
    {
        component = new LogicGate(LogicGate::SB_REGISTER,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setWidth(widthPart(flag));
    }
    else if(isPart(ENCODERF, flag))
    {
        component = new LogicGate(LogicGate::ENCODER,this);
        component->setPropertiesTo(properties);
        ((LogicGate*)component)->setMuxWidth(countPart(flag));
    }

    if(component)
    {
        component->setPropertiesTo(properties);
        component->setPos(pos);
        scene()->addItem(component);
    }

    return component;
}

void CircuitBoard::clear()
{
    setBuildCFlags(0);

    while(scene()->items().count())
    {
        QGraphicsItem *item = scene()->items().first();

        LogicGate* gate = qgraphicsitem_cast<LogicGate*>(item); //this is done this way because any class
        LogicSource* source = qgraphicsitem_cast<LogicSource*>(item); // derived from OperatingComponent's
        BulbComponent* bulb = qgraphicsitem_cast<BulbComponent*>(item); // parent class is casted as an OC
        LED_Component* led = qgraphicsitem_cast<LED_Component*>(item); // using qgraphicitem_cast, bad Qt

        if(OperatingComponent *oc = (OperatingComponent*)((long long)gate + (long long)source + (long long)bulb + (long long)led))
        {
                delete oc;
        }
    }

    setBuildCFlags(0);
}

void CircuitBoard::reset()
{
    simulationTime = 0;

    eventTree->removeAll();

    foreach(QGraphicsItem *item, scene()->items())
    {
        LogicGate* gate = qgraphicsitem_cast<LogicGate*>(item);
        LogicSource* source = qgraphicsitem_cast<LogicSource*>(item);
        BulbComponent* bulb = qgraphicsitem_cast<BulbComponent*>(item);
        LED_Component* led = qgraphicsitem_cast<LED_Component*>(item);

        if(OperatingComponent *oc = (OperatingComponent*)((long long)gate + (long long)source + (long long)bulb + (long long)led))
        {
            oc->reset();
        }
    }



}

QTextStream& operator >>(QTextStream& in, CircuitBoard& board)
{
   //qDebug() << in.device()->size();
   //qDebug() << in.device()->pos();

    board.clear();
    board.reset(); //

    int i;
    in >> i;
    board.setPropertyFlagTo(i);

    QString name;
    QChar ch;
    QPointF pos;
    int cFlag = 0, count = 0, id = 0, id2 = 0, c = 0;
    unsigned int props = 0;
    qreal angle = 0;
    double d, x, y;

    QString s;

    in >> cFlag;
    while(cFlag)
    {
        in >> x;
        in >> y;
        pos.setX(x);
        pos.setY(y);

        in >> props;
        OperatingComponent* oc = board.createComponent(cFlag,pos,props);
        in >> angle;
        oc->setRotation(angle);
        in >> d;
        oc->setDelay(d);

        oc->show();

        in >> count;
        for(int i = 0; i < count; i++)
        {
           //qDebug() << "IN";
            in >> c;
            in >> ch;
            name = "";
            for(int j = 0; j < c; j++)
            {
                in >> ch;
                name.append(ch);
            }
            //in >> name;
            in >> id;
            if(i < oc->inputPinCount())
            {
                oc->inputPins()[i]->setName(name);
                oc->inputPins()[i]->setPinID(id);
            }
        }
        in >> count;
        for(int i = 0; i < count; i++)
        {
           //qDebug() << "OUT";
            in >> c;
            in >> ch;
            name = "";
            for(int j = 0; j < c; j++)
            {
                in >> ch;
                name.append(ch);
            }

            in >> id;
            if(i < oc->outputPinCount())
            {
                oc->outputPins()[i]->setName(name);
                oc->outputPins()[i]->setPinID(id);
            }
        }
        in >> count;
        for(int i = 0; i < count; i++)
        {
           //qDebug() << "CH";
            in >> c;
            in >> ch;
            name = "";
            for(int j = 0; j < c; j++)
            {
                in >> ch;
                name.append(ch);
            }

            in >> id;
            if(i < oc->chooserPinCount())
            {
                oc->chooserPins()[i]->setName(name);
                oc->chooserPins()[i]->setPinID(id);
            }
        }

        in >> cFlag;
    }

    in >> count;
    for(int i = 0; i < count; i++)
    {
        in >> id;
        int count2;
        in >> count2;
        for(int j = 0; j < count2; j++)
        {
            in >> id2;
            Pin::connect(id, id2);
        }
    }

    return in;
}

QTextStream& operator <<(QTextStream& out, CircuitBoard& board)
{
    QList<Pin*> outputPins;

    out << board.propertyFlags() << "\n";
    foreach(QGraphicsItem* item, board.scene()->items())
    {
        LogicGate* gate = qgraphicsitem_cast<LogicGate*>(item);
        LogicSource* source = qgraphicsitem_cast<LogicSource*>(item);
        BulbComponent* bulb = qgraphicsitem_cast<BulbComponent*>(item);
        LED_Component* led = qgraphicsitem_cast<LED_Component*>(item);

        if(OperatingComponent *oc = (OperatingComponent*)((long long)gate + (long long)source + (long long)bulb + (long long)led))
        {
            if(oc->isDummy())
                continue;
            out << board.createFlag(oc) << "\n";
            out << oc->pos().x() << "\n";
            out << oc->pos().y() << "\n";
           //qDebug() << out.status();
            out << oc->getProperties() << "\n";
           //qDebug() << out.status();
            out << oc->rotation() << "\n";
            out << oc->delay() << "\n";
            out << oc->inputPinCount() << "\n";

           //qDebug() << board.createFlag(oc);
           //qDebug() << oc->pos();
           //qDebug() << oc->getProperties();
           //qDebug() << oc->rotation();
           //qDebug() << oc->delay();
           //qDebug() << oc->inputPinCount();
            foreach(Pin* pin, oc->inputPins())
            {
                out << pin->name().length() << "\n";
                out << pin->name() << "\n";
                out << pin->pinID() << "\n";

               //qDebug() << pin->name();
               //qDebug() << pin->pinID();
            }
            out << oc->outputPinCount() << "\n";

           //qDebug() << oc->outputPinCount();
            foreach(Pin* pin, oc->outputPins())
            {
                out << pin->name().length() << "\n";
                out << pin->name() << "\n";
                out << pin->pinID() << "\n";

               //qDebug() << pin->name();
               //qDebug() << pin->pinID();
            }
            out << oc->chooserPinCount() << "\n";

           //qDebug() << oc->chooserPinCount();
            foreach(Pin* pin, oc->chooserPins())
            {
                out << pin->name().length() << "\n";
                out << pin->name() << "\n";
                out << pin->pinID() << "\n";

               //qDebug() << pin->name();
               //qDebug() << pin->pinID();
            }
        }
        else if(Pin* p = qgraphicsitem_cast<Pin*>(item))
        {
            if(p->pinType() == Pin::OUTPUT)
                outputPins.append(p);
        }
    }
    out << 0 << "\n";
    out << outputPins.count() << "\n";

   //qDebug() << 0;
   //qDebug() << outputPins.count();
    foreach(Pin *p, outputPins)
    {
        out << p->pinID() << "\n";

       //qDebug() << "PID: " << p->pinID();
        int destinations = p->destination->count();
        out << destinations << "\n";
       //qDebug() << "CONNECTED TO: " << destinations;
        foreach(Pin *d, *(p->destination))
        {
            out << d->pinID() << "\n";
           //qDebug() << d->pinID();
        }

    }

    return out;
}

void CircuitBoard::enterEvent(QEvent *e)
{
    if(newComponentFlag)
    {
        dComponent[typePart(newComponentFlag)]->show();
        //setCursor(QCursor(Qt::BlankCursor)); //may be set at a later date, needs to handle key press changes
    }
    QGraphicsView::enterEvent(e);
}

void CircuitBoard::leaveEvent(QEvent *e)
{
    if(newComponentFlag)
    {
        dComponent[typePart(newComponentFlag)]->hide();
        //setCursor(QCursor());
    }
    QGraphicsView::leaveEvent(e);
}

void CircuitBoard::copy()
{

    if(scene()->selectedItems().isEmpty())
        return;
    copied.clear();
    QTextStream out(&copied, QIODevice::WriteOnly);

    copy(out);
}

void CircuitBoard::copy(QTextStream &out)
{
    QGraphicsItem *it = scene()->selectedItems().first();

    if(!((long long)qgraphicsitem_cast<LogicGate*>(it) + (long long)qgraphicsitem_cast<LogicSource*>(it) +
         (long long)qgraphicsitem_cast<BulbComponent*>(it) + (long long)qgraphicsitem_cast<LED_Component*>(it)))
        return;

    out << scene()->selectedItems().count() << "\n";

    QRectF rect(scene()->selectedItems().first()->boundingRect());
    QPointF tl(scene()->selectedItems().first()->mapToScene(0,0)), point;

    foreach(QGraphicsItem *item, scene()->selectedItems())
    {
        point = item->mapToScene(0,0);
        if(point.x() < tl.x())
            tl.setX(point.x());
        if(point.y() < tl.y());
            tl.setY(point.y());
    }

    out << tl.x() << "\n";
    out << tl.y() << "\n";

    QList<Pin*> inputPins;
    QList<OperatingComponent*> components;

    foreach(QGraphicsItem* item, scene()->selectedItems())
    {
        LogicGate* gate = qgraphicsitem_cast<LogicGate*>(item);
        LogicSource* source = qgraphicsitem_cast<LogicSource*>(item);
        BulbComponent* bulb = qgraphicsitem_cast<BulbComponent*>(item);
        LED_Component* led = qgraphicsitem_cast<LED_Component*>(item);

        if(OperatingComponent *oc = (OperatingComponent*)((long long)gate + (long long)source + (long long)bulb + (long long)led))
        {
            if(oc->isDummy())
                continue;

            components.append(oc);

            out << createFlag(oc) << "\n";
            out << oc->pos().x() << "\n";
            out << oc->pos().y() << "\n";
            out << oc->getProperties() << "\n";
            out << oc->rotation() << "\n";
            out << oc->delay() << "\n";

            foreach(Pin* pin, oc->inputPins())
            {
                inputPins.append(pin);
            }

            foreach(Pin* pin, oc->chooserPins())
            {
                inputPins.append(pin);
            }
        }
    }
    out << 0 << "\n";
    out << inputPins.count() << "\n";

    int parentIndex = 0, sourceParentIndex = 0, pinIndex = 0, index = 0;

    foreach(Pin *p, inputPins)
    {
        if(!p->source)
            continue;
        sourceParentIndex = components.indexOf(qgraphicsitem_cast<OperatingComponent*>(p->source->parentItem()));
        if(sourceParentIndex < 0)
            continue;

        parentIndex = components.indexOf(qgraphicsitem_cast<OperatingComponent*>(p->parentItem()));

        index = components[parentIndex]->inputPins().indexOf(p);
        if(index >= 0)
            pinIndex = index * 2 + 0;
        index = components[parentIndex]->chooserPins().indexOf(p);
        if(index >= 0)
            pinIndex = index * 2 + 1;

        out << parentIndex << "\n";
        out << pinIndex << "\n";
        out << sourceParentIndex << "\n";
        out << components[sourceParentIndex]->outputPins().indexOf(p->source) << "\n";
    }
}

void CircuitBoard::paste()
{
    if(copied.isEmpty())
        return;

    QTextStream in(&copied, QIODevice::ReadOnly);
    paste(in);
}

void CircuitBoard::paste(QTextStream &in)
{
    double top = 0, left = 0, w = 0, h = 0;
    int cFlag = 0, count = 0;

    in >> count;

    static QProgressBar *progress = 0;

    if(!progress)
    {
        progress = new QProgressBar();
        ((MainWindow*)this->window())->statusBar()->insertWidget(0,progress);
    }

    progress->setMaximum(count*4);
    progress->setValue(0);
    progress->show();

    in >> left;
    in >> top;
    //in >> w;
    //in >> h;

    //qDebug() << left << top;

    //QRectF r(left, top, w, h);
    QPointF pos, point(left,top);
    unsigned int props = 0;
    qreal angle = 0;
    double d, x, y;

    QList<OperatingComponent*> components;

    in >> cFlag;
    while(cFlag)
    {
        //qDebug() << cFlag;

        in >> x;
        in >> y;
        pos.setX(x);
        pos.setY(y);

        progress->setValue(progress->value() + 1);

        in >> props;
        OperatingComponent* oc = createComponent(cFlag,pos,props);

        progress->setValue(progress->value() + 1);

        in >> angle;
        oc->setRotation(angle);

        progress->setValue(progress->value() + 1);

        in >> d;
        oc->setDelay(d);

        progress->setValue(progress->value() + 1);

        oc->show();
        components.append(oc);

        in >> cFlag;
    }

    int parentIndex = 0, sourceParentIndex = 0, pinIndex = 0, sourceIndex = 0;

    in >> count;
    for(int i = 0; i < count; i++)
    {
        in >> parentIndex;
        in >> pinIndex;
        in >> sourceParentIndex;
        in >> sourceIndex;

        if(in.atEnd())
            break;

        switch(pinIndex % 2)
        {
        case 0:
            components[sourceParentIndex]->outputPins()[sourceIndex]->connect(components[parentIndex]->inputPins()[pinIndex / 2]);
            break;
        case 1:
            components[sourceParentIndex]->outputPins()[sourceIndex]->connect(components[parentIndex]->chooserPins()[pinIndex / 2]);
            break;
        }
    }

    pos = this->mapToScene(QPoint(0,0));
    qDebug() << pos;
    pos = pos - point;
    scene()->clearSelection();

    pos += QPoint(20,20);

    foreach(OperatingComponent* oc, components)
    {
        oc->moveBy(pos.x(), pos.y());
        oc->setSelected(true);
    }

    progress->hide();
}

void CircuitBoard::deleteSelected()
{
    foreach(QGraphicsItem *item, scene()->selectedItems())
    {
        if(Pin* pin = qgraphicsitem_cast<Pin*>(item))
        {
            pin->disconnect();
            //pin->setName("");
        }//pin cannot be selected at the same time as other item, so there is no need to check for others
        else if(OperatingComponent* oc = qgraphicsitem_cast<OperatingComponent*>(item))
        {
            oc->deleteKeyAction();
        }
    }
}

/*/ Rotates the selection about the center point of the selection,
    calculated as the average of all center points.
    This ensures that their relative positions remain the same/*/
void CircuitBoard::rotateSelected(int i)
{
    QList<OperatingComponent*> list;
    QPointF center(0,0);
    foreach(QGraphicsItem *item, scene()->selectedItems())
    {
        if(OperatingComponent* oc = qgraphicsitem_cast<OperatingComponent*>(item))
        {
            if(qgraphicsitem_cast<LogicGate*>(item)||qgraphicsitem_cast<LogicSource*>(item)||
               qgraphicsitem_cast<LED_Component*>(item)||qgraphicsitem_cast<BulbComponent*>(item))
            {
                list.append(oc);

                    center += oc->mapToScene(oc->center());
            }
        }
    }

    center /= list.count();

    QPointF difference, target;

    static const double pi = qAcos(-1);
    double angle, angleD, length;

    foreach(OperatingComponent* oc, list)
    {
        difference = oc->mapToScene(oc->center()) - center;
        length = sqrt(difference.y() * difference.y() +
                      difference.x() * difference.x());

        if(length < 0.5)
            length = 0;

        if(difference.x() != 0)
        {
            angle = qAtan(difference.y() / difference.x())
                    + ((difference.x() < 0)?pi:0);
        }
        else
        {
            if(qAbs(difference.y()) > 0.0001)
            {
                angle = pi/2 + ((difference.y() > 0)?0:pi);
            }
            else
                angle = 0;
        }

        angleD = angle * 180 / pi;

        target = QPointF(qCos(angle + (i - oc->rotation()) * pi / 180)*length,
                         qSin(angle + (i - oc->rotation()) * pi / 180)*length);



        if(difference != QPointF(0,0))
        {
            oc->setPos(oc->pos() - difference + target);
        }

        oc->setTransformOriginPoint(oc->center());
        oc->setRotation(i);
    }
}

void CircuitBoard::selectAll()
{
    foreach(QGraphicsItem *item, scene()->items())
    {
    //Due to inheritance of Component, pin can be mistakenly cast as an OperatingComponent,
        //so actual operating components must be checked
        if(qgraphicsitem_cast<LogicGate*>(item)||qgraphicsitem_cast<LogicSource*>(item)||
           qgraphicsitem_cast<LED_Component*>(item)||qgraphicsitem_cast<BulbComponent*>(item))
            item->setSelected(true);
    }
}
