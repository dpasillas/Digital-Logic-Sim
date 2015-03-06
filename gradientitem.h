#ifndef GRADIENTITEM_H
#define GRADIENTITEM_H

#include <QGraphicsItem>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

/*/ This is used to represent the light on bulb components /*/

class GradientItem : public QGraphicsItem
{
    Q_INTERFACES(QGraphicsItem)
public:
    GradientItem(QGraphicsItem* parent):QGraphicsItem(parent)
    {
        grad.setColorAt(0,QColor(255,255,128,255));

        /*/ Qt on linux cannot handle composition modes /*/
#ifdef Q_OS_LINUX
        grad.setColorAt(1,QColor(0,0,0,0));
#else
        grad.setColorAt(1,QColor(0,0,0,255));
#endif
        grad.setRadius(32);
        //setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    }
    QRectF boundingRect() const{int r = grad.radius(); return QRect(-r,-r,2*r,2*r);}
    //QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        painter->setClipRect(option->exposedRect);

        /*QPixmap pix(64,64);
        pix.fill(Qt::black);
        QPainter pPix(&pix);
        grad.setCenter(32,32);
        grad.setFocalPoint(32,32);
        pPix.fillRect(QRect(0,0,64,64),grad);*/
#ifndef Q_OS_LINUX
        painter->setCompositionMode(QPainter::CompositionMode_ColorDodge);
#endif
        painter->fillRect(boundingRect(),grad);
        //painter->drawPixmap(QPoint(-32,-32),pix);//QPixmap::fromImage(img));
    }

private:
    QRadialGradient grad;
};

#endif // GRADIENTITEM_H
