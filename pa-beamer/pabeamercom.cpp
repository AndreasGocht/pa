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


#include "pabeamercom.h"
#include <QMessageBox>

PaBeamerCom::PaBeamerCom(PaGuiId* id,QTcpSocket *guiComSocket,QObject *parent) :
    QObject(parent)
{
    this->guiIds = id;
    //set socet, write initdata to Gui and read Initdata from gui
    beamerGuiComSocket = guiComSocket;

    readGui = new QXmlStreamReader;
    writeGui = new QXmlStreamWriter;

    //inital read
    readGui->setDevice(beamerGuiComSocket);
    writeGui->setDevice(beamerGuiComSocket);

    writeGui->setAutoFormatting(true);
    writeGui->writeStartDocument();
    writeGui->writeStartElement("pa");

    qDebug()<< "conection opend";

    connect(beamerGuiComSocket,SIGNAL(readyRead()),this,SLOT(handleGuiNewData()));
    connect(beamerGuiComSocket,SIGNAL(disconnected()),this,SLOT(handleGuiDisconnect()));

}

PaBeamerCom::~PaBeamerCom()
{
    beamerGuiComSocket->deleteLater();
}


//this function handels all info requests witch are no position data. So This is the command interface.
void PaBeamerCom::handleGuiNewData()
{
    parseState state = noneParse;

    //wait and handle com requests
    while(!readGui->atEnd())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = readGui->readNext();
        qDebug()<<"token:"<<token;

        //starting xml
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }


        //handle some requests
        if(token == QXmlStreamReader::StartElement) {

            qDebug()<<"startelement recived";
            qDebug()<<readGui->name() ;

            // gui Needs for some reason a new Id, so lets give.
            if(readGui->name() == "getGuiId") {
                qDebug()<< "getGuiId";
                writeGui->writeTextElement("guiId",QString::number(guiIds->getId()));
                continue;
            }
        }


        if (token == QXmlStreamReader::EndElement){
        }


//        if (readGui->hasError() && (readGui->error()==QXmlStreamReader::PrematureEndOfDocumentError))
//        {
//            //wait for new data, if not enough availabel
//            qDebug()<<"wait--------------------------------------------------------------------";
//            beamerGuiComSocket->waitForReadyRead();
//        }



    }
//    if (readGui->hasError())
//    {
//        QMessageBox *msgBox = new QMessageBox;
//        msgBox->setText(readGui->errorString());
//        msgBox->exec();
//    }

}

void PaBeamerCom::handleGuiDisconnect()
{
    this->deleteLater();
}




