#include "cameraworker.h"
#include <QtCore>
#define CAMNUM 0

cv::Mat imageptr_cv;
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    qDebug() << " Finally called ";
  // sensor_msgs::CvBridge bridge;
  // namedWindow( "view", cv::WINDOW_AUTOSIZE );
  cv_bridge::CvImagePtr cv_ptr;
   try
   {
     cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
   }
   catch (cv_bridge::Exception& e)
   {
     ROS_ERROR("cv_bridge exception: %s", e.what());
     return;
   }
  ::imageptr_cv = cv_ptr->image;
  //cv::imshow("view", cv_ptr->image);
//  *ipl = cv_ptr->image;

}

CameraWorker::CameraWorker(QObject *parent) :
    QObject(parent)
{

    cv::namedWindow("view");
    cv::startWindowThread();

    image_transport::ImageTransport it(nh);
    sub = it.subscribe("ardrone/image_raw", 1000, imageCallback);

    isCamera = true;
    lHeight = 480;
    lWidth = 640;
    delx = dely = 0;
    capture = cvCaptureFromCAM(-1);

//    cvSetCaptureProperty(capture, CV_CAP_PROP_FPS, 30);
    IplImage *temp = cvLoadImage("/home/abhinav/Pictures/123.png");
    arenaFrame = cvCreateImage(cvSize(640,480), 8, 3);
    cvResize(temp, arenaFrame);
    if(!arenaFrame)
    {
        qDebug() << "Arena couln't be loaded.";
        return;
    }

//    qDebug() << "xOffset = " << frame->roi->xOffset << " yOffset = " << frame->roi->yOffset << " width= " << frame->roi->width << "height=" << frame->roi->height;
    myPixmap = NULL;
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
   // ros::spinOnce();
}

void CameraWorker::setup(QThread *cThread, QMutex *mutex)
{
    myThread = cThread;
    myMutex = mutex;
    connect(cThread, SIGNAL(started()), this, SLOT(onEntry()));
}

void CameraWorker::onTimeout()
{
    qDebug() << "Called" ;
    if(ros::ok())
        ros::spinOnce();

    QImage qimg((uchar*)imageptr_cv.data, imageptr_cv.cols, imageptr_cv.rows, imageptr_cv.step, QImage::Format_RGB888);
    myMutex->lock();
    if(myPixmap)
        delete myPixmap;
    myPixmap = new QPixmap(QPixmap::fromImage(qimg));
    myMutex->unlock();
    emit imageReady(myPixmap);
    timer->setSingleShot(true);
    timer->start(10);
}

void CameraWorker::onEntry()
{
    qDebug() << "Entered Camera Worker.";
    timer->setSingleShot(true);
    timer->start(10);
}

void CameraWorker::onStop()
{
    if(capture)
        cvReleaseCapture(&capture);
    myMutex->lock();
    if(myPixmap)
        delete myPixmap;
    myMutex->unlock();
    qDebug() << "Stopped camera thread.";
    myThread->exit();
}

void CameraWorker::onRightMouseDragged(int dx, int dy)
{
    if(isArenaCalib)
        return;
    roi->xOffset-=dx/(zoom+1);
    roi->yOffset-=dy/(zoom+1);
    if(roi->xOffset < 0)
        roi->xOffset = 0;
    else if(roi->xOffset + roi->width > lWidth)
        roi->xOffset = lWidth - roi->width;
    if(roi->yOffset < 0)
        roi->yOffset = 0;
    else if(roi->yOffset + roi->height > lHeight)
        roi->yOffset = lHeight - roi->height;
}
