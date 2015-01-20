#include "camlabel.h"

CamLabel::CamLabel(QWidget *parent) :
    QLabel(parent)
{
    prevx = 0;
    prevy = 0;
}

void CamLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->buttons() & Qt::LeftButton)
    {
//        qDebug() << "mouse signal sent.";
        emit mouseClicked(ev->x(), ev->y());
//        emit mousePressed(ev->x(), ev->y());
    }
    if(ev->buttons() & Qt::RightButton)
    {
        prevx = ev->x();
        prevy = ev->y();
    }
}

void CamLabel::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons() & Qt::LeftButton)
        emit mousePressed(ev->x(), ev->y());
    else if(ev->buttons() & Qt::RightButton)
    {
        emit mouseRightPressed(ev->x()-prevx, ev->y()-prevy);
        prevx = ev->x();
        prevy = ev->y();
    }
    emit mouseOnlyMove(ev->x(), ev->y());
}

void CamLabel::wheelEvent(QWheelEvent *ev)
{
    emit mouseScrollCam(ev->delta()/120);
}
