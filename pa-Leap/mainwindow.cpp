/*This file is part of pa (Project Anna).

pa (Project Anna) is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

pa (Project Anna) is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with pa (Project Anna).  If not, see <http://www.gnu.org/licenses/>.
*/


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "framelistener.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qRegisterMetaType<paData>("paData");


    int maxTouch = 20;
    ui->setupUi(this);

    com = new paLeapCom(maxTouch);
    listener = new FrameListener();

    connect(listener,SIGNAL(newPoint(paData)),com,SLOT(newPoint(paData)));

    com->connectToGui(QHostAddress("127.0.0.1"));

    qDebug() << controler.addListener(*listener);

//    while( !controler.isConnected()) //controller is a Controller object
//    {
//        qDebug()<<"waitung";
//    }
//    qDebug()<<"hi";
//    while(c.isConnected())
//    {
//        Leap::Frame frame = c.frame();
//        qDebug() << "frame";
//    }
//        Leap::Frame frame = c.frame(); //The latest frame
//        Leap::Frame previous = c.frame(1); //The previous frame


}

MainWindow::~MainWindow()
{
    delete ui;
}


