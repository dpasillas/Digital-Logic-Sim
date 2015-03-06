#ifndef LED_COMPONENT_H
#define LED_COMPONENT_H

#include "operatingcomponent.h"

/*/  A 7, 14, or 16-segment LED display/*/

class LED_Component : public OperatingComponent
{
public:
    enum LED_Type{LED_7 = 0, LED_14 = 1, LED_16 = 2};
    LED_Component(CircuitBoard* board, LED_Type lt = LED_7);

    ~LED_Component();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual
        void operate(Pin* source);
    virtual
        QPoint center();

    enum { Type = UserType + 6 };
    int lType(){return ledType;}
    int type() const { return Type; }

protected:
    void createSegs();

    static QList<QList<QPainterPath> > segPrototypes;
    QList<QColor> segments;
    QPainterPath* path;
    LED_Type ledType;
};

#endif // LED_COMPONENT_H
