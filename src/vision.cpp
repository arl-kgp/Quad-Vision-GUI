#include <QApplication>
#include "quadgui.h"
#include <ros/ros.h>

int main(int argc, char *argv[])
{
    ros::init(argc,argv, "imageListener");
    QApplication a(argc, argv);
    quadGUI w;
    w.show();
    
    return a.exec();
}
