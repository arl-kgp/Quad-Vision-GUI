#ifndef QUADGUI_H
#define QUADGUI_H

#include <QMainWindow>
#include <QList>
#include <QListWidgetItem>
#include <QPixmap>
#include <QMutex>
#include <QTime>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include "cameraworker.h"

namespace Ui {
class quadGUI;
}
class CameraWorker;

class quadGUI : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit quadGUI(QWidget *parent = 0);
    ~quadGUI();
public slots:
    void onCamImageReady(QPixmap *pm);
    void onTimeout();
signals:
    void colorChanged(int row);
    void stopCamThread();
private:
    Ui::quadGUI *ui;
    QMutex *camMutex;
    QThread *cameraThread;
    CameraWorker *cw;
};

#endif // QUADGUI_H
