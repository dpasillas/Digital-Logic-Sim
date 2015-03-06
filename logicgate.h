#ifndef LOGICGATE_H
#define LOGICGATE_H
#include <QGraphicsItem>
#include <QGraphicsDropShadowEffect>
#include "operatingcomponent.h"
#include "pin.h"

/*/ A visual, and functional representation of Logic gates /*/

class CircuitBoard;
class QString;
//class QRectF;
class QPainterPath;
class QGraphicsSceneContextMenuEvent;

class LogicGate;

class LogicGate : public OperatingComponent
{
public:
    typedef void (LogicGate::*opFunc)(Pin*);
    typedef void (LogicGate::*voidFunc)();
    enum LogicGateType{BUFFER, NOT, AND, NAND, OR, NOR, XOR, XNOR, MERGER,
                       SPLITTER, MULTIPLEXER, DECODER, COUNTER, REGISTER, D_FF, T_FF, J_K_FF,
                        L_COUNTER, ADDER, SL_REGISTER, SR_REGISTER, SB_REGISTER, ENCODER};
    enum SStyle{IEEE, IEC};

    LogicGate(enum LogicGate::LogicGateType = BUFFER, CircuitBoard* board = 0, enum LogicGate::SStyle = IEEE);
    ~LogicGate();

/*/ setupXXX methods serve as constructors for different logic gate types /*/
    void setupStandard(int i);
    void setupStandard1();
    void setupStandard2();
    void setupBUFF();
    void setupNOT();
    void setupAND();
    void setupNAND();
    void setupOR();
    void setupNOR();
    void setupXOR();
    void setupXNOR();

    void setupMERG();
    void setupSPLI();
    void setupMULT();
    void setupDECO();
    void setupCOUN();
    void setupREGI();
    
    void setupD_FF();
    void setupT_FF();
    void setupJ_K_();
    
    void setupL_CO();
    void setupADDE();
    void setupSL_R();
    void setupSR_R();
    void setupSB_R();

    void setupENCO();
    /*/ End setup functions  /*/

    void setStyle(LogicGate::SStyle s);
    void setGraph(CircuitBoard* board);
    void setPinCount(int count);
    void setWidth(int w);
    void setMuxWidth(int w);
    int getWidth();
    int getMuxWidth();

    void reset();

    LogicGateType logicType();

    virtual
        void setPropertiesTo(int flag);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    CircuitBoard* getBoard();

    virtual
        void operate(Pin* source);

/*/ opXXXX functions are called when a logic gate operates, when a change in input is detected /*/
    void opBUFF(Pin* source);
    void opNOT(Pin* source);
    void opAND(Pin* source);
    void opNAND(Pin* source);
    void opOR(Pin* source);
    void opNOR(Pin* source);
    void opXOR(Pin* source);
    void opXNOR(Pin* source);

    void opMERG(Pin* source);
    void opSPLI(Pin* source);
    void opMULT(Pin* source);
    void opDECO(Pin* source);
    void opCOUN(Pin* source);
    void opREGI(Pin* source);

    void opD_FF(Pin* source);
    void opT_FF(Pin* source);
    void opJ_K_(Pin* source);

    void opL_CO(Pin* source);
    void opADDE(Pin* source);
    void opSL_R(Pin* source);
    void opSR_R(Pin* source);
    void opSB_R(Pin* source);

    void opENCO(Pin* source);
    /*/  End operations   /*/

    virtual
        QPoint center();
    unsigned int mask(int width);

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    friend class Pin;

protected:
    QAction* runContextMenu(QMenu* menu, QPoint screenPos);
    LogicGateType lgt;
    SStyle style;
    int width, muxWidth;

    QPoint centerPoint;

    //chooser is for the multiplexer

    QPainterPath path;

    static QMap<int, QPainterPath>gPath;
    static QString *iecLabel, gateName[11];

    static int typeCount[10];
    static voidFunc setupFunc[];
    static opFunc operateFunc[];
};

#endif // LOGICGATE_H
