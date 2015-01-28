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
