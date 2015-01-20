#ifndef CAMERAWORKER_H
#define CAMERAWORKER_H

#include <QObject>
#include <QtCore>
#include <QThread>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QImage>
#include <QPixmap>
#include <QMutex>
#include <vector>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

//IplImage* ipl;
class CameraWorker : public QObject
{
    Q_OBJECT
public:
    explicit CameraWorker(QObject *parent = 0);
    void setup(QThread *cThread, QMutex *mutex);
signals:
    void imageReady(QPixmap *pm);
    void markBGR(int b, int g, int r);
public slots:
    void onRightMouseDragged(int dx, int dy);
    void onTimeout();
    void onEntry();
    void onStop();
    //void imageCallback(const sensor_msgs::ImageConstPtr& msg);
private:
    bool isArenaCalib;
    int delx, dely;
    int zoom;
    int lHeight, lWidth;
    IplROI *roi;
    bool isCamera;
    IplImage *arenaFrame;
    IplImage *frame;
    IplImage *calibFrame;
    IplImage *displayFrame, *displayCamFrame, *displayArenaFrame;
    IplImage *blobImage;
    QMutex *myMutex, *lutMutex;
    QThread *myThread;
    QTimer *timer;
    CvCapture *capture;
    QPixmap *myPixmap;
    ros::NodeHandle nh;
    image_transport::Subscriber sub;
    cv_bridge::CvImagePtr cv_ptr;
};

#endif // CAMERAWORKER_H
