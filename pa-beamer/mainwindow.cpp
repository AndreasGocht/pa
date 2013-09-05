/*
This file is part of pa (Project Anna).

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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //ui->setupUi(this);

    drawArea = new PaWidget(this);
    drawArea->resize(this->size());

    this->showFullScreen();

    beamerServer = new QTcpServer();
    beamerServer->listen(QHostAddress::Any,15001);

    beamerDataServer = new QTcpServer();
    beamerDataServer->listen(QHostAddress::Any,15002);


    beamerComs = new QList<PaBeamerCom*>;
    beamerDatas = new QList<PaBeamerData*>;

    guiIds = new PaGuiId;

    connect(beamerDataServer,SIGNAL(newConnection()),this,SLOT(handleGuiNewDataConnection()));
    connect(beamerServer,SIGNAL(newConnection()),this,SLOT(handleGuiNewConnection()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    drawArea->resize(this->size());
    return QMainWindow::resizeEvent(event);
}

void MainWindow::handleGuiNewDataConnection()
{
    //handle new Data connections, and build an new Data objeckt, for each new connect
    //data handling works in the connection intfaces PaGutData
    qDebug()<<"new Data Connection";
    QTcpSocket* guiSocket = beamerDataServer->nextPendingConnection();

    PaBeamerData* guiData = new PaBeamerData(this); //id is necesary, for identification of communication
    beamerDatas->append(guiData);

    //build the message Paths
    connect(guiData,SIGNAL(newData(PaData)),drawArea,SLOT(newData(PaData)));
    connect(guiData,SIGNAL(newData(PaData)),this,SLOT(broadcastNewData(PaData)));
    connect(this,SIGNAL(newData(PaData)),guiData,SLOT(sendOtherData(PaData))); //using cosequently message pathing for communication

    guiData->setSocket(guiSocket);

}

void MainWindow::broadcastNewData(PaData data)
{
    //this function is only for konsequent message pathing
    //maybe this is not the efficients way to do this.
    emit newData(data);
}

void MainWindow::handleGuiNewConnection()
{
    //handle new Com connection, and build an objekt. This channel is only used for konfigruration etc.
    qDebug()<<"new Data Connection";
    QTcpSocket* guiSocket = beamerServer->nextPendingConnection();

    PaBeamerCom* guiCom = new PaBeamerCom(guiIds,guiSocket, this); //id is necesary, for identification of communication
    beamerComs->append(guiCom);
}
