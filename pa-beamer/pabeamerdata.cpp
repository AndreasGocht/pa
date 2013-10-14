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


#include "pabeamerdata.h"

PaBeamerData::PaBeamerData(QObject *parent) :
    QObject(parent)
{
    readState = noneData;
    bufferdData = 0; //pointer mit 0 initialisieren
    bufferdPoint = 0;

    readGui = new QXmlStreamReader();
    writeGui = new QXmlStreamWriter();
}

PaBeamerData::~PaBeamerData()
{
}

void PaBeamerData::handleGuiDisconnect()
{
    //cleanup
    //beamerGuiSocket->deleteLater();
    this->deleteLater();
}

void PaBeamerData::handleGuiNewData()
{
    //qDebug()<<"handle new data";
    while(!readGui->atEnd())
    {
        QXmlStreamReader::TokenType token = readGui->readNext();
        //qDebug()<<"token:"<<token;


        if(token == QXmlStreamReader::StartElement)
        {
//            qDebug()<<"startElemnt recived";
            if((readGui->name()=="data")&& (readState == noneData))
            {
//                qDebug()<<"new Data Tag";
                readState = data;
                if (bufferdData != 0)
                {
                    //if ther is still a bufferd data something went wrong with the last statemant
                    //howeever, the old one is deleted an an new one is build.
                    delete bufferdData;
                }
                if (bufferdPoint != 0)
                {
                    delete bufferdPoint;
                }

                bufferdData = new PaData;
                bufferdPoint = new QPointF();

//                qDebug()<<"data";
                continue;
            }
            if((readGui->name() == "dx") && (readState == data))
            {
//                qDebug()<<"dx tag";
                bufferdPoint->setX(readGui->readElementText().toDouble());
                continue;
            }
            if((readGui->name() == "dy") && (readState == data))
            {
//                qDebug()<<"dy tag";
                bufferdPoint->setY(readGui->readElementText().toDouble());
                continue;
            }
            if((readGui->name() == "style") && (readState == data))
            {
//                qDebug()<<"style tag";
                //cast as style, becaus no conversion from int to style :-)
                bufferdData->style = (Qt::BrushStyle)readGui->readElementText().toInt();
                continue;
            }
            if((readGui->name() == "color") && (readState == data))
            {
//                qDebug()<<"color tag";
                bufferdData->color = readGui->readElementText().toUInt();
                continue;
            }
            if((readGui->name() == "width") && (readState == data))
            {
//                qDebug()<<"dx width";
                bufferdData->width = readGui->readElementText().toDouble();
                continue;
            }
            if((readGui->name() == "effect") && (readState == data))
            {

                bufferdData->effect = (paEffects)readGui->readElementText().toInt();
//                qDebug()<<"effect"<<bufferdData->effect;
                continue;
            }
            if((readGui->name() == "time") && (readState == data))
            {
//                qDebug()<<"time tag";
                bufferdData->time = readGui->readElementText().toInt();
                continue;
            }
            if((readGui->name() == "id") && (readState == data))
            {
//                qDebug()<<"time tag";
                bufferdData->touchId = readGui->readElementText().toInt();
                continue;
            }
            if((readGui->name() == "guiId") && (readState == data)){
                bufferdData->guiId = readGui->readElementText().toInt();
            }


        }
        if(token == QXmlStreamReader::EndElement)
        {
//            qDebug()<<"End element";
            if(readGui->name() == "data" && (readState == data))
            {
                //qDebug()<<"end init";

                bufferdData->pos = *bufferdPoint;
                bufferdData->rect = QRectF(); //filld later in paint method

                emit newData(*bufferdData);

                if (bufferdData != 0)
                {
                    delete bufferdData;
                    bufferdData = 0;

                }
                if (bufferdPoint != 0)
                {
                    delete bufferdPoint;
                    bufferdPoint = 0;
                }

                readState = noneData;
            }
        }
    }

}

void PaBeamerData::setSocket(QTcpSocket *guiSocket)
{
    //set socet, write initdata to Gui and read Initdata from gui
    beamerGuiSocket = guiSocket;

    //inital read
    readGui->setDevice(beamerGuiSocket);
    writeGui->setDevice(beamerGuiSocket);

    connect(beamerGuiSocket,SIGNAL(readyRead()),this,SLOT(handleGuiNewData()));
    connect(beamerGuiSocket,SIGNAL(disconnected()),this,SLOT(handleGuiDisconnect()));

    qDebug()<< "connection etablished";



}

void PaBeamerData::sendOtherData(PaData data)
{
//    TODO
//    writeGui->writeStartElement("data");
//        writeGui->writeTextElement("dx",QString::number(data.pos.x()));
//        writeGui->writeTextElement("dy",QString::number(data.pos.y()));
//        writeGui->writeTextElement("style",QString::number(data.style));
//        writeGui->writeTextElement("color",QString::number(data.color));
//        writeGui->writeTextElement("width",QString::number(data.width));
//        writeGui->writeTextElement("time",QString::number(data.time));
//    writeGui->writeEndElement();
}
