#ifndef BULBCOMPONENT_H
#define BULBCOMPONENT_H
#include "operatingcomponent.h"
#include <QObject>

class GradientItem;

class BulbComponent : public OperatingComponent
{
public:
    friend class CircuitBoard;
    BulbComponent(CircuitBoard* board);

    ~BulbComponent();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void operate(Pin* source);
    
    enum { Type = UserType + 5 };
    int type() const { return Type; }

private:
    bool on;
    GradientItem* gItem;
    static QPainterPath* bulbPath;
    QPainterPath* path;
};

#endif // OUTPUTCOMPONENT_H
