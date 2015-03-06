#ifndef CIRCUITBOARD_H
#define CIRCUITBOARD_H

#include <QGraphicsView>
#include "component.h"
#include "binarysearchtree.h"
#include "logicevent.h"
//#include <QTextStream>

class QWheelEvent;
class LogicGate;
class Pin;

/*/ The CircuitBoard class handles graphical representation of logic gates,
    event dispatching, and general user interface functionality/*/

class CircuitBoard : public QGraphicsView
{
public:
    enum {BUFF = 1, NOTF, ANDF, NANDF, ORF, NORF, XORF, XNORF, BUTTONF, SWITCHF, GROUNDF, SOURCEF,
            CLOCKF, BULBF, LED_7F, LED_14F, LED_16F, MERGERF, SPLITTERF, MULTIPLEXERF,
            DECODERF, REGISTERF, COUNTERF, D_FFF, T_FFF, J_K_FFF, L_COUNTERF, ADDERF, SL_REGISTERF,
            SR_REGISTERF, SB_REGISTERF, ENCODERF}; // component creation flags
    enum {SHOW_I_O = 1, DISPLAY_NAMES = 2, DISPLAY_CONNECTIONS = 4}; //displayFlags

    CircuitBoard(QWidget* parent);

    void insertPin(Pin* pin);
    void removePin(Pin* pin);
    void passConnect(Pin* pin);
    bool contains(Pin *pin, double time);
    bool contains(Pin *pin);
    bool containsOne(Pin *pin);
    bool containsOne(QString s);
    void remove(LogicGate* gate);
    void addEvent(Pin* pin, double targetTime, unsigned int value, LogicState state);
    double step();
    double timeStep();
    double timeStep(double stepSize);
    void removeEvent(Pin* pin, double targetTime);
    void removeEvents(Pin* pin);
    void setBuildCFlags(int flag);
    void unsetBuildCFlag();
    void setPropertyFlag(int flag, bool value);
    void setPropertyFlagTo(int flag);
    unsigned int propertyFlags();
    void setCurrent(OperatingComponent* current);
    int typePart(int f);
    int widthPart(int f);
    int countPart(int f);
    bool isPart(int f, int from);
    static int createFlag(int t, int w, int c);
    static int createFlag(OperatingComponent* oc);
    OperatingComponent* createComponent(int flag, QPointF pos, int properties = 0);
    void clear();
    void reset();
    void copy(QTextStream &out);
    void paste(QTextStream &in);
    void copy();
    void paste();
    void deleteSelected();
    void rotateSelected(int i);
    void selectAll();

    bool appendSelection, ctrlHeld;
    double time();

    friend
        QTextStream& operator >>(QTextStream &in, CircuitBoard &board);

    friend
        QTextStream& operator <<(QTextStream &out, CircuitBoard &board);

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void scaleView(qreal scaleFactor);
    void drawBackground(QPainter *painter, const QRectF &rect);
    //void drawForeground(QPainter *painter, const QRectF &rect);

    int gridSize, newComponentFlag, propertyFlag;

    QList<QGraphicsItem*> selection;

    BinarySearchTree<Pin> *inputPinTree, *outputPinTree;
    BinarySearchTree<LogicEvent> *eventTree;

    Pin* currentPin;
    OperatingComponent* currentOC;
    QMap<int, OperatingComponent*> dComponent;

    QString copied;

    double simulationTime;

};

#endif // CIRCUITBOARD_H
