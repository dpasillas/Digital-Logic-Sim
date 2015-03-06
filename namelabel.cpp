#include "namelabel.h"
#include "pin.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

NameLabel::NameLabel(QGraphicsItem *parent): QGraphicsItem(parent), font(QFont("Arial",6,QFont::Normal)), rect(0,0,1,1)
{
    font.setPixelSize(10);
    setFlags(//QGraphicsItem::ItemIgnoresTransformations |
             QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);
    updateName();
    setZValue(5);
}

QRectF NameLabel::boundingRect() const
{
    return rect;
}

QPainterPath NameLabel::shape() const
{
    return path;
}

void NameLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setClipRect(option->exposedRect);
    //painter->setBrush(Qt::transparent);
    //painter->setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap,Qt::RoundJoin));

    //painter->strokePath(path,painter->pen());
    painter->setFont(font);
    painter->setPen(Qt::darkMagenta);

    painter->drawText(rect, Qt::AlignVCenter, ((Pin*)parentItem())->name());
}

void NameLabel::setName(QString name)
{
    ((Pin*)parentItem())->setName(name);
    updateName();
}

void NameLabel::updateName()
{
    QFontMetrics fm(font);
    double left = rect.left(), right = rect.right();
    rect = fm.boundingRect(((Pin*)parentItem())->name());
    rect.adjust(-1,-1,2,2);
    rect.translate(2,-2);

    switch(((Pin*)parentItem())->dir())
    {
    case LEFT: //rectangle expands to the left only
        rect.moveRight(right);
        break;
    case RIGHT: // rectangle expands to the right only
    case UP:
    case DOWN:
        rect.moveLeft(left);
    }

    path = QPainterPath();
    path.addRect(rect);

    prepareGeometryChange();
}

void NameLabel::moveLeft(double x)
{
    rect.moveLeft(x);
}

void NameLabel::moveRight(double x)
{
    rect.moveRight(x);
}

void NameLabel::moveUpper(double y)
{
    rect.moveTop(y);
}

void NameLabel::moveLower(double y)
{
    rect.moveBottom(y);
}
