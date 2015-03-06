#ifndef OPERATINGCOMPONENT_H
#define OPERATINGCOMPONENT_H

#include <QGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QStyleOptionGraphicsItem>
#include <QMenu>
#include <QInputDialog>
#include "component.h"
#include "circuitboard.h"
#include "logicstate.h"
#include "pin.h"
#include "connection.h"
#include "gradientitem.h"

/*/ Every Logic gate inherits Operating Component, which interfaces with the circuit board/*/

class OperatingComponent: public Component, public QGraphicsItem
{
    Q_INTERFACES(QGraphicsItem)
public:
    friend class Pin;
    enum LogicGateProperty{VariableShape = 0x00000001, IBussable = 0x00000002, OBussable = 0x00000004,
                           SingleOutput = 0x00000008, MergingPins = 0x00000010, Mux = 0x00000020, Bussed = 0x00000040,
                           DUMMY = 0x00000080};

    OperatingComponent(CircuitBoard* board):graph(board)
    {
        properties = 0;
        propagationDelay = 0;
        invisible = false;

        setZValue(3);
        setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
        setCacheMode(DeviceCoordinateCache);
    }

    void destructFunc()
    {
        Pin* p;
        while(input.count())
        {
            p = input.last();
            input.removeLast();
            delete p;
        }
        while(output.count())
        {
            p = output.last();
            output.removeLast();
            delete p;
        }
        while(chooser.count())
        {
            p = chooser.last();
            chooser.removeLast();
            delete p;
        }

        //if(scene())
        //    scene()->removeItem(this);
    }

    CircuitBoard* getBoard(){return graph;}

    void setInvisible(bool b = true){invisible = b;}
    virtual
        void operate(Pin* source) = 0;
    virtual
        void deleteKeyAction(){delete this;}
    virtual
        void setDefaultStateValue()
        {
            if(defaultState)
                foreach(Pin* o, output)
                    o->setState(defaultState);
            else
                foreach(Pin* o, output)
                    o->setValue(defaultValue);}
    virtual
        QPoint center(){return QPoint(16,16);}
    virtual
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
        {
            if(!invisible)
            {
                painter->setClipRect(option->exposedRect);

                QBrush brush(Qt::white);
                QPen pen(Qt::black,1,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);

                if(isSelected())
                {
                    brush.setColor(QColor(0,121,221,128));
                    pen.setColor(QColor(0,121,221).darker());
                    pen.setStyle(Qt::DotLine);
                    foreach(QGraphicsItem* pin, childItems())
                        pin->update(); //if the item is selected, child pins reflect the change
                    foreach(QGraphicsItem *item, scene()->selectedItems())
                        if(Pin* pin = qgraphicsitem_cast<Pin *>(item))
                            pin->setFlag(ItemIsSelectable,false);
                }
                else
                {
                    foreach(QGraphicsItem* pin, childItems())
                        pin->update(); //likewise, if the current item is no longer selected, child reflect the change
                    foreach(QGraphicsItem *item, scene()->items())
                        if(Pin* pin = qgraphicsitem_cast<Pin *>(item))
                            pin->setFlag(ItemIsSelectable,true);
                }

                painter->setPen(pen);
                painter->setBrush(brush);
                customPaint(painter, option, widget);
            }
        }

    virtual
        void customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;

    double delay(){return propagationDelay;}
    bool hasProperties(unsigned int p) const {return (properties & p) == p;}
    unsigned int getProperties(){return properties;}

    void setDummy()
    {
        properties |= DUMMY;
        setFlag(QGraphicsItem::ItemIsSelectable, false);
        setFlag(QGraphicsItem::ItemIsFocusable, false);
        foreach(Pin* pin, input)
            graph->removePin(pin);
        foreach(Pin* pin, output)
            graph->removePin(pin);
        foreach(Pin* pin, chooser)
            graph->removePin(pin);
    }

    virtual void setPropertiesTo(int flag)
    {
        if(flag != 0)
            properties = flag;
    }

    bool isDummy()
    {
        return hasProperties(DUMMY);
    }

    virtual
    void reset()
    {
        foreach(Pin* pin, output)
        {
            pin->updateNext();
        }
    }

    QList<Pin*> outputPins(){return output;}
    QList<Pin*> inputPins(){return input;}
    QList<Pin*> chooserPins(){return chooser;}

    int outputPinCount()
    {
        return output.count();
    }

    int inputPinCount()
    {
        return input.count();
    }

    int chooserPinCount()
    {
        return chooser.count();
    }
    virtual
        void setDelay(double d)
    {
        propagationDelay = d;
    }

protected:
    virtual
        QVariant itemChange(GraphicsItemChange change, const QVariant &value)
        {
            if(scene())
            {
                if(change == QGraphicsItem::ItemPositionHasChanged ||
                   change == QGraphicsItem::ItemTransformHasChanged ||
                   change == QGraphicsItem::ItemTransformChange)
                    foreach(QGraphicsItem* item, childItems())
                        if(Pin* pin = qgraphicsitem_cast<Pin*>(item))
                            foreach(Connection* c, *(pin->connections))
                                c->updatePath();
            }
            return QGraphicsItem::itemChange(change,value);
        }
    void postEvent(unsigned int v, LogicState s)
    {
        double targetTime = graph->time() + delay();
        graph->addEvent(output[0],targetTime,v,s);
    }

    void postEvent(unsigned int v, LogicState s, Pin* pin)
    {
        double targetTime = graph->time() + delay();
        graph->addEvent(pin,targetTime,v,s);
    }

    virtual
        QAction* runContextMenu(QMenu* menu, QPoint screenPos){return menu->exec(screenPos);}

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
    {
        if(hasProperties(DUMMY))
        {
            event->ignore();
            return;
        }

        //OperatingComponent::getBoard()->setCurrent(this);

        QMenu menu;
        QAction *deleteAction = menu.addAction(QString("Delete")),
                *rotateAction = menu.addAction(QString("Rotate..."));

        QAction *executed = runContextMenu(&menu, event->screenPos());

        if(executed)
        {
            if(executed == deleteAction)
            {
                if(!isSelected())
                {
                    foreach(QGraphicsItem* item, scene()->selectedItems())
                        item->setSelected(false);
                    setSelected(true);
                }
                graph->deleteSelected();
                return;
            }
            else if(executed == rotateAction)
            {
                bool b = false;
                int degrees = QInputDialog::getInt(getBoard()->parentWidget(),
                                                   QString("Rotate"), QString("Set Angle: "),rotation(),-360,360,1,&b);
                //setTransformOriginPoint(center());
                if(b)
                {
                    //setRotation(degrees);
                    if(!isSelected())
                    {
                        foreach(QGraphicsItem* item, scene()->selectedItems())
                            item->setSelected(false);
                        setSelected(true);
                    }
                    graph->rotateSelected(degrees);

                    moveBy(1,1); // call itemChange(QVariant), issue fixed in Qt 4.7
                    moveBy(-1,-1);
                }
            }
        }
    }


    QList<Pin*> input, output, chooser;
    bool invisible;
    unsigned int defaultValue, properties;
    LogicState defaultState;
    CircuitBoard* graph;
    double propagationDelay;
};

#endif // OPERATINGCOMPONENT_H
