#include "quadgui.h"
#include "ui_quadgui.h"
#include "camlabel.h"

quadGUI::quadGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::quadGUI)
{
    ui->setupUi(this);

    ///Camera Thread Setup
    cameraThread = new QThread;
    camMutex = new QMutex;
    cw = new CameraWorker;
    cw->setup(cameraThread, camMutex);
    connect(this, SIGNAL(stopCamThread()), cw, SLOT(onStop()));
    connect(ui->imgLabel, SIGNAL(mousePressed(int,int)), cw, SLOT(onMouseClicked(int,int)));
    connect(ui->imgLabel, SIGNAL(mouseClicked(int,int)), cw, SLOT(onMouseClicked(int,int)));
    connect(ui->imgLabel, SIGNAL(mouseRightPressed(int,int)), cw, SLOT(onRightMouseDragged(int,int)));
    connect(cw, SIGNAL(imageReady(QPixmap*)), this, SLOT(onCamImageReady(QPixmap*)));
    cw->moveToThread(cameraThread);
    cameraThread->start();
}

quadGUI::~quadGUI()
{
    emit stopCamThread();
    cameraThread->wait();
    delete cw;
    delete ui;
}

void quadGUI::onTimeout()
{

}

void quadGUI::onCamImageReady(QPixmap *pm)
{
    ros::spinOnce();
    camMutex->lock();
    ui->imgLabel->setPixmap(*pm);
    camMutex->unlock();
}

void quadGUI::on_takeoffButton_clicked()
{
    ROS_INFO("Flying ARdrone");
    takeOff = nh.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);
    twist = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    ros::Rate loop_rate(30);

    int cnt = 0;
    while( ros::ok()) {
        if(cnt < 5){
            cnt+=1;
            takeOff.publish(msgE);
            ros::spinOnce();
        }
        else
            break;
        loop_rate.sleep();
    }
}

void quadGUI::on_landButton_clicked()
{
    ROS_INFO("Landing ARdrone");
    land = nh.advertise<std_msgs::Empty>("/ardrone/land", 1);
    ros::Rate loop_rate(30);
    int cnt = 0;
    while( ros::ok()) {
        if(cnt < 5){
            cnt+=1;
            land.publish(msgE);
            ros::spinOnce();
        }
        else
            break;
        loop_rate.sleep();
    }
}

void quadGUI::on_togglecameraButton_clicked()
{
    ros::ServiceClient client = nh.serviceClient<std_srvs::Empty>("/quad/ardrone/togglecam");
    std_srvs::Empty service;
    if(client.call(service))
    {
        ROS_INFO("Camera toggled");
    }
}
