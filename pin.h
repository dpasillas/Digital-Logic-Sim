#ifndef PIN_H
#define PIN_H

#include "component.h"
#include <QGraphicsItem>
#include "direction.h"
#include "logicstate.h"

/*/ All operating components interface with each other through the pin class /*/

class NameLabel;
class LogicGate;
class LogicSource;
class OperatingComponent;
class CircuitBoard;
class Connection;
class CompositeItem;

class Pin : public Component, public QGraphicsItem
{
    Q_INTERFACES(QGraphicsItem)
public:
    enum PinType{INPUT, OUTPUT};

    Pin(bool uniq = false);
    Pin(Direction o, PinType t, bool n = false, OperatingComponent *parent = 0);
    ~Pin();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updatePath();
    bool connect(Pin* pin);
    bool connect(int id);
    static bool connect(int i1, int i2);
    void connectCurrentIfMatch();
    void disconnect();
    int pinWidth();
    void setPinWidth(int w);
    unsigned int pinValue();
    LogicState pinState();
    PinType pinType();
    void updateNext();
    static void updatePin(Pin* pin);
    bool exists();
    void select(){setSelected(true);}

    virtual
        void setName(QString s, bool reconnnect = true);

    enum { Type = UserType + 2 };
    int type() const { return Type; }
    void setColor(QColor& c);
    void setState(LogicState s);
    void setValue(int i);
    Direction dir();
    void resetState();
    void showLabel();
    void hideLabel();
    bool labelVisible();
    void showConnections();
    void hideConnections();
    void setPinID(int id);
    int pinID();

    friend class LogicSource;
    friend class OperatingComponent;
    friend class Connection;
    friend class CircuitBoard;

    Pin* source;
    QList<Pin*> *destination;

protected:
    CircuitBoard* parentBoard();
    LogicGate* parentGate();
    OperatingComponent* parentComponent();
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    CompositeItem* compositeParent;
    Direction orientation;
    LogicState state;
    PinType pType;
    unsigned int value;
    int width, pathRotation, pid;
    bool notType, destructorActive;

    //Pin* source;
    //QList<Pin*> *destination;
    QList<Connection*> *connections;

    static QString zString, xString;
    QString **display;

    static QPainterPath *pinPath, *notPath;
    QPainterPath myPath;

    static int nextPinID;

    static Pin* current;
    static QMap<int, Pin*> pins;

    NameLabel *label;
};

#endif // PIN_H
