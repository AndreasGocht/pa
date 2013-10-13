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


#include "paleapcom.h"

paLeapCom::paLeapCom(qint16 maxTouch, QObject *parent) :
    QObject(parent)
{
    write = new QXmlStreamWriter();

    guiServer = new QTcpSocket();

    bufferdData = 0;
    bufferdPoint = 0;

    this->maxTouch = maxTouch;
}

paLeapCom::~paLeapCom()
{
    write->writeEndDocument();

    guiServer->disconnectFromHost();
}

bool paLeapCom::connectToGui(QHostAddress guiAdress)
{
    //connect to Gui Server, for data transmitting an reciving
    //retruns true if connection was sucessfull, and false, if something went wrong

    guiServer->connectToHost(guiAdress,15000);
    //"192.168.43.245"

    qDebug() << "conection";
    if(!guiServer->waitForConnected(1000))
    {
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setText(guiServer->errorString() + "\nPleas Restart the application");
        msgBox->exec();
        return false;
    }

    qDebug() <<"set device and write init data";

    //set Devices
    write->setDevice(guiServer);
    write->setAutoFormatting(true);

    //buid data connections
    //connect(guiServer,SIGNAL(readyRead()),this,SLOT(handleNewData()));
    connect(guiServer,SIGNAL(disconnected()),this,SLOT(handleDisconnect()));


    //write init data
    write->writeStartDocument();
    write->writeStartElement("pa_smart_gui");
        write->writeStartElement("init");
        write->writeTextElement("maxTouch",QString::number(maxTouch));
        write->writeEndElement();

    return true;

}

void paLeapCom::newPoint(paData point)
{
    //qDebug() << "out";
    //write the new position data into the xmlstream
    if (point.touchId<maxTouch)
    {
        write->writeStartElement("touch");
        write->writeTextElement("dx",QString::number(point.pos.x()));
        write->writeTextElement("dy",QString::number(point.pos.y()));
        write->writeTextElement("id",QString::number(point.touchId));
        write->writeEndElement();
    }

}

void paLeapCom::handleDisconnect()
{
    qDebug()<<"Disconnect";
}
