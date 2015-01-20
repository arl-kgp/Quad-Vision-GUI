#ifndef CAMLABEL_H
#define CAMLABEL_H

#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QtCore>

class CamLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CamLabel(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);
signals:
    void mouseClicked(int x, int y);
    void mousePressed(int x, int y);
    void mouseRightPressed(int dx, int dy);
    void mouseScrollCam(int ticks);
    void mouseOnlyMove(int x, int y);
public slots:
private:
    int prevx, prevy;
};

#endif // CAMLABEL_H
