#ifndef LOGICEVENT_H
#define LOGICEVENT_H

#include <QDebug>
#include <QQueue>
#include "pin.h"
#include "logicstate.h"

/*/ All logic operations are represented by a logic event /*/
/*/ This allows events to be have a delay, and to be searched and removed /*/

class LogicEvent
{
public:
    LogicEvent(Pin *pin = 0, double t = 0, unsigned int v = 0, LogicState s = UNKNOWN):target(pin),time(t),value(v),state(s){}
    LogicEvent(Pin &pin, double t, unsigned int v, LogicState s):target(&pin),time(t),value(v),state(s){}
    LogicEvent(LogicEvent& e):target(e.target),time(e.time),value(e.value),state(e.state){}
    static void setToBeDeleted(Pin* pin){toBeDeleted = pin;}
    void enqueueIfMatch(){if(target == toBeDeleted)queue.enqueue(this);}
    friend
        QDebug& operator<<(QDebug d, LogicEvent& e){return d << (void*)(e.target) << " " << e.target->name() << " " << e.time;}
    bool operator<(LogicEvent& e){return (time == e.time)?target < e.target:time < e.time;}
    bool operator>(LogicEvent& e){return (time == e.time)?target > e.target:time > e.time;}
    bool operator==(LogicEvent& e){return time == e.time && target == e.target;}
    bool operator%(LogicEvent& e){return time == e.time && target == e.target;}
    void operate(){
        if(target->pinState() == state && target->pinValue() == value)
            return;
        if(state)
        {
            target->setValue(0);
            target->setState(state);
        }
        else target->setValue(value);
        target->update();
        target->updateNext();}

    static QQueue<LogicEvent*> queue;
    static Pin* toBeDeleted;
    Pin* target;
    double time;
    unsigned int value;
    LogicState state;

};

#endif // LOGICEVENT_H
