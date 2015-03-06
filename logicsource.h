#ifndef LOGICSOURCE_H
#define LOGICSOURCE_H

#include <QGraphicsItem>
#include <QPointF>
#include "operatingcomponent.h"

/*/ Logic Sources represent logic gates with no input pins /*/

class Pin;
class CircuitBoard;

class LogicSource : public OperatingComponent
{
public:
    enum InputType{BUTTON, SWITCH, CLOCK, SOURCE, GROUND, CUSTOM};
    LogicSource(enum LogicSource::InputType it = BUTTON, CircuitBoard* board = 0);
    ~LogicSource();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual
        void operate(Pin* source);
    virtual
        void setDefaultStateValue();

    enum { Type = UserType + 3 };
    int sourceType() {return inputType;}
    int type() const { return Type; }

    //void reset();
    void setDelay(double d);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    QAction* runContextMenu(QMenu *menu, QPoint screenPos);

    InputType inputType;
    bool checkable, checked;

    static QPointF *mousePressPos;
    static QString inputName[6];
    static int typeCount[6];
    static QPainterPath *path, *clockPath;
};

#endif // LOGICSOURCE_H
