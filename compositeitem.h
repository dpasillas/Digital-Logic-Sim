#ifndef COMPOSITEITEM_H
#define COMPOSITEITEM_H

#include <QGraphicsItem>

/*/ This is a stub.  It is intended to encapsulate a customizable network   /*/
/*/ of logic gates as a black box. (i.e. represent a user-created chip)     /*/
/*/     Not currently use                                                   /*/

class CircuitBoard;
class LogicGate;
class Pin;

class CompositeItem : public QGraphicsItem
{
public:
    /*/ The original purpose of this was to make all components
        using only AND, OR, and NOT gates.  However, it may be repurposed to
        create components from any other already-existing components /*/
    enum CompositeType{MULTIPLEXER, N_WIDE_MUX, DECODER, ADDER};
    CompositeItem(enum CompositeType ct, CircuitBoard* board = 0);

private:
    CompositeType cType;
};

#endif // COMPOSITEITEM_H
