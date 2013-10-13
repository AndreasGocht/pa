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

#include "paguicom.h"

PaGuiCom::PaGuiCom(QObject *parent) :
    QObject(parent)
{
    readState = noneTouch;

    guiServer = new QTcpServer();
    guiServer->setMaxPendingConnections(1); //only one Device schoudl connect, after discnonect another device may connect
    guiServer->listen(QHostAddress::Any,15000);

    beamerComWrite = new QXmlStreamWriter;
    beamerComRead = new QXmlStreamReader;
    beamerDataWrite = new QXmlStreamWriter;

    smartRead = new QXmlStreamReader;

    //set the Com stuff
    guiBeamerComSocket = new QTcpSocket();
    beamerComWrite->setDevice(guiBeamerComSocket);
    beamerComRead->setDevice(guiBeamerComSocket);

    //set the data stuff
    guiBeamerDataSocket = new QTcpSocket();
    beamerDataWrite->setDevice(guiBeamerDataSocket);



    connect(guiServer,SIGNAL(newConnection()),this,SLOT(handleSmartNewConnection()));
    //connect(broadcastServer,SIGNAL(newConnection()),this,SLOT(handleBroadcastNewConnection()));

    defaultStyle.brushStyle = Qt::RadialGradientPattern;
    defaultStyle.color = QColor("white").rgb();
    defaultStyle.width = 50;
    defaultStyle.time = 1000;
    defaultStyle.effect = POINT_LINE;

    maxTouch = 0;

}

PaGuiCom::~PaGuiCom()
{
    //cleanup and destroy
    qDebug()<< "cleanup, and destroy";

    guiServer->close();
    guiServer->deleteLater();

    guiSmartSocket->deleteLater();
}


bool PaGuiCom::connectToBeamerServer(QHostAddress ip)
{
    //set the connection
    guiBeamerComSocket->connectToHost(ip,15001);

    qDebug()<<ip;
    if(!guiBeamerComSocket->waitForConnected())
    {
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setText(guiBeamerComSocket->errorString());
        msgBox->exec();
        return false;
    }
    qDebug()<<"conectetd to Beamer";


    beamerComWrite->setAutoFormatting(true);
    beamerComWrite->writeStartDocument();
    beamerComWrite->writeStartElement("pa_gui_beamer_com");
    beamerComWrite->writeTextElement("getGuiId","");


    initState state = noneInit;

    //wait and handle the init stuff
    guiBeamerComSocket->waitForReadyRead();
    while(!beamerComRead->atEnd())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = beamerComRead->readNext();
        qDebug()<<"token:"<<token;
        //starting xml
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if(token == QXmlStreamReader::StartElement){
            if(beamerComRead->name() == "guiId") {
                guiId = beamerComRead->readElementText().toInt();
                qDebug()<< "guiId"<<guiId;
                continue;
            }

        }


        if (token == QXmlStreamReader::EndElement){
        }

// TODO
//        if (beamerComRead->hasError() && (beamerComRead->error()==QXmlStreamReader::PrematureEndOfDocumentError))
//        {
//            //wait for new data, if not enough availabel
//            qDebug()<<"wait--------------------------------------------------------------------";
//            guiBeamerComSocket->waitForReadyRead();
//        }



    }
    //TODO
//    if (beamerComRead->hasError() )
//    {
//        QMessageBox *msgBox = new QMessageBox;
//        msgBox->setText(beamerComRead->errorString());
//        msgBox->exec();
//    }


    //the initalisation stuff is done ... ok now lets connet the data streams from the client to the beamer

    guiBeamerDataSocket->connectToHost(ip,15002);

    if(!guiBeamerDataSocket->waitForConnected())
    {
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setText(guiBeamerDataSocket->errorString());
        msgBox->exec();
        return false;
    }
    qDebug()<<"Data conectetd to Beamer etablised";

    beamerDataWrite->setAutoFormatting(true);
    beamerDataWrite->writeStartDocument();
    beamerDataWrite->writeStartElement("pa_gui_beamer_data");


    connect(this,SIGNAL(newData(paData)),this,SLOT(sendNewData(paData)));

    return true;
}

void PaGuiCom::setStyle(paStyle newStyle,qint16 touchId)
{
    if ((touchId<maxTouch) && (touchId>=0))
    {
        styles[touchId] = newStyle;
    }else if (touchId==-1) //if id = -1 style is for all touchpints
    {
        for (int i=0; i<styles.length();i++)
        {
            styles[i] = newStyle;
        }
    }else
    {
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setText("!!!TouchID error!!! Check the ID You want to set. Style is not Set");
        msgBox->exec();
    }
}

qint32 PaGuiCom::getNewGuiId()
{
    qint32 tmpGuiId = 0;
    beamerComWrite->writeTextElement("getGuiId","");

    guiBeamerComSocket->waitForReadyRead();
    //wait and handle the init stuff
    while(!beamerComRead->atEnd())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = beamerComRead->readNext();
        qDebug()<<"token:"<<token;
        //starting xml
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if(token == QXmlStreamReader::StartElement){
            if(beamerComRead->name() == "guiId") {
                tmpGuiId = beamerComRead->readElementText().toInt();
                qDebug()<< "tmpGuiId"<<tmpGuiId;
                continue;
            }

        }


        if (token == QXmlStreamReader::EndElement){
        }

//TODO
//        if (beamerComRead->hasError() && (beamerComRead->error()==QXmlStreamReader::PrematureEndOfDocumentError))
//        {
//            //wait for new data, if not enough availabel
//            qDebug()<<"wait--------------------------------------------------------------------";
//            guiBeamerComSocket->waitForReadyRead();
//        }

    }

    //TODO
//    if (beamerComRead->hasError())
//    {
//        QMessageBox *msgBox = new QMessageBox;
//        msgBox->setText(beamerComRead->errorString());
//        msgBox->exec();
//    }

    return tmpGuiId;
}


void PaGuiCom::handleSmartNewConnection()
{
    //handle new connection from smartphone, parse init data and etablish connection
    //back to smartphone
    //connection to smatphone is necesarry to push the data from the server to the Smartphone

    //int i = 0;
    guiSmartSocket = guiServer->nextPendingConnection();

    //inital reado
    smartRead->setDevice(guiSmartSocket);
    qDebug()<< "conection Smart opend";


    initState state = noneInit;

    //wait and handle the init stuff
    while(!smartRead->atEnd() || (smartRead->hasError()&&(smartRead->error()==QXmlStreamReader::PrematureEndOfDocumentError)))
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = smartRead->readNext();
        //qDebug()<<"token:"<<token;

        //starting xml
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        //parsing the init stuff
        //work with state machine, we can't be secure all initdata arraive at once

        if(token == QXmlStreamReader::StartElement) {
            if(smartRead->name() == "init") {
                state = init;
                qDebug()<< "init";
                continue;
            }
            if(smartRead->name() == "maxTouch") {
                state = init;
                maxTouch = smartRead->readElementText().toInt();
                for (int i=0; i<maxTouch;i++)
                {
                    styles.append(defaultStyle);
                }
                qDebug()<< "maxTouch:"<<maxTouch  ;
                continue;
            }

        }


        if (token == QXmlStreamReader::EndElement){
            if ((smartRead->name() == "init") && (state == init)){
                state = noneInit;
                //qDebug() << "end init / end loop";
                break;
            }
        }



        //qDebug()<< i++;
        if (smartRead->hasError() && (smartRead->error()==QXmlStreamReader::PrematureEndOfDocumentError))
        {
            //wait for new data, if not enough availabel
            //qDebug()<<"wait--------------------------------------------------------------------";
            guiSmartSocket->waitForReadyRead();
        }



    }
    if (smartRead->hasError())
    {
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setText(smartRead->errorString());
        msgBox->exec();
    }

    connect(guiSmartSocket,SIGNAL(readyRead()),this,SLOT(handleSmartNewData()));
    connect(guiSmartSocket,SIGNAL(disconnected()),this,SLOT(handleSmartDisconnect()));

    //TODO way back of data
    //connect(broadcastSocket,SIGNAL(readyRead()),this,SLOT(handleBroadcastNewData()));
}


void PaGuiCom::handleSmartNewData()
{
    //the information from the smartphone will be parsed, own settings like color will be added and the wohle stuff will be pushed to the Beamer.
    //if data is in more then on pacages, it reads data of the first, und continue reading if more data availibal


    //parse data witch are in
    while(!smartRead->atEnd())
    {
        QXmlStreamReader::TokenType token = smartRead->readNext();
        //qDebug()<<"token:"<<token;

        if(token == QXmlStreamReader::StartElement)
        {
            if((smartRead->name() == "touch") && (readState == noneTouch))
            {
                readState = touch;
                bufferdReadData.clear();
                //qDebug()<< "touch";
                continue;
            }
            if((smartRead->name() == "dx") && (readState == touch))
            {
                //qDebug()<<"dx";
                bufferdReadData.append(smartRead->readElementText());
                continue;
            }
            if((smartRead->name() == "dy") && (readState == touch))
            {
                //qDebug()<<"dy";
                bufferdReadData.append(smartRead->readElementText());
                continue;
            }
            if((smartRead->name() == "id") && (readState == touch))
            {
                //qDebug()<<"id";
                bufferdReadData.append(smartRead->readElementText());
                continue;
            }

        }
        if(token == QXmlStreamReader::EndElement)
        {
            if((smartRead->name() == "touch") && (readState == touch))
            {
                //save the data in paData and send to all how are interested ^^

                paData data;
                qint16 tmpId = bufferdReadData.at(2).toInt(); //TODO: maybe her is a securitiy Issue, but the device scould know how much toches it had, and send it with the initalisation stuff

                data.pos.setX(bufferdReadData[0].toFloat());//first should be dx
                data.pos.setY(bufferdReadData[1].toFloat());//second should be dy
                data.touchId = bufferdReadData[2].toInt();//third should be id
                data.style = styles.at(tmpId).brushStyle;
                data.color = styles.at(tmpId).color;
                data.width = styles.at(tmpId).width;
                data.time = styles.at(tmpId).time;
                data.effect = styles.at(tmpId).effect;
                data.guiId = guiId;

                emit newData(data);

                //reset the Stuff fot the next touchelement
                bufferdReadData.clear();
                readState = noneTouch;


            }
        }
    }
    if (smartRead->hasError() && (smartRead->error()!=QXmlStreamReader::PrematureEndOfDocumentError))
    {
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setText(smartRead->errorString());
        msgBox->exec();
    }
}

void PaGuiCom::handleSmartDisconnect()
{
    //cleanup for new connection

    qDebug() << "disconnect Smart, cleanup";

    guiSmartSocket->deleteLater();

    //TODO
    //disconnect(broadcastSocket,SIGNAL(readyRead()),this,SLOT(handleBroadcastNewData()));
}

//void PaGuiCom::handleBroadcastNewConnection()
//{
//    broadcastSocket = broadcastServer->nextPendingConnection();
//    connect(broadcastSocket,SIGNAL(disconnected()),this,SLOT(handleBroadcastNewDisconnect()));
//}

//void PaGuiCom::handleBroadcastNewData()
//{
//    //push data from beamer to smartphon
//    guiSmartSocket->write(broadcastSocket->readAll());
//}

//void PaGuiCom::handleBroadcastNewDisconnect()
//{
//    broadcastSocket->deleteLater();
//}


void PaGuiCom::handleBeamerDisconnect()
{
    //connection from server died why ever.
    guiBeamerComSocket->deleteLater();
    guiBeamerDataSocket->deleteLater();
}

void PaGuiCom::sendNewData(paData data)
{
    //qDebug()<<"send the stuff";
    //sendening the stuff to the server

    beamerDataWrite->writeStartElement("data");
        beamerDataWrite->writeTextElement("dx",QString::number(data.pos.x()));
        beamerDataWrite->writeTextElement("dy",QString::number(data.pos.y()));
        beamerDataWrite->writeTextElement("id",QString::number(data.touchId));
        beamerDataWrite->writeTextElement("style",QString::number(data.style));
        beamerDataWrite->writeTextElement("color",QString::number(data.color));
        beamerDataWrite->writeTextElement("width",QString::number(data.width));
        beamerDataWrite->writeTextElement("time",QString::number(data.time));
        beamerDataWrite->writeTextElement("effect",QString::number(data.effect));
        beamerDataWrite->writeTextElement("guiId",QString::number(data.guiId));
    beamerDataWrite->writeEndElement();

}

