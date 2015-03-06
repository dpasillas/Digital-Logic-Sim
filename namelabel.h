#ifndef NAMELABEL_H
#define NAMELABEL_H

#include <QGraphicsItem>
#include <QFont>

/*/ Used to display names of pins /*/

class Pin;

class NameLabel : public QGraphicsItem
{
public:
    NameLabel(QGraphicsItem *parent);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updateName();
    void setName(QString name);
    void moveLeft(double x);
    void moveRight(double x);
    void moveUpper(double y);
    void moveLower(double y);

    enum { Type = UserType + 4 };
    int type() const { return Type; }

private:
    QRectF rect;
    QPainterPath path;
    QFont font;
};

#endif // NAMELABEL_H
