#ifndef CONNECTION_H
#define CONNECTION_H
#include "pin.h"

/*/ This class serves as a graphical representation of connections ONLY /*/
/*/ Operating components can easily function without it /*/

class Connection : public QGraphicsItem
{
    Q_INTERFACES(QGraphicsItem)
public:
    Connection(Pin* s, Pin* d);
    ~Connection();
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updatePath();
private:
    Pin *source, *destination;
    QPainterPath *path;
};

#endif // CONNECTION_H
