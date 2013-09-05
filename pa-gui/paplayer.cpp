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

#include "paplayer.h"

PaPlayer::PaPlayer(QObject *parent) :
    QObject(parent)
{
    timer = new QTimer;
    guiId = 0; // no com is set, so id is not importend ... data wouldn't send

    connect(timer,SIGNAL(timeout ()),this,SLOT(sendData()));

}

PaPlayer::~PaPlayer()
{
    disconnect(this,SIGNAL(newData(paData)),this->com,SLOT(sendNewData(paData)));
    disconnect(timer,SIGNAL(timeout()),this,SLOT(sendData()));
}

void PaPlayer::setPaCom(PaGuiCom *com)
{
    this->com = com;
    this->guiId = com->getNewGuiId();
    connect(this,SIGNAL(newData(paData)),this->com,SLOT(sendNewData(paData)));
}

void PaPlayer::loadFile(QString fileName)
{
    readState state = none;
    paRecData tmp;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;
    QXmlStreamReader read(&file);
    while(!read.atEnd())
    {
        QXmlStreamReader::TokenType token = read.readNext();
        //qDebug()<<"token:"<<token;

        if(token == QXmlStreamReader::StartElement)
        {
            if((read.name() == "rec_elem") && (state==none))
            {
                state=recElem;
                continue;
            }
            if((read.name() == "dt") && (state == recElem))
            {
                tmp.timestamp = read.readElementText().toInt();
                continue;
            }
            if((read.name() == "data") && (state == recElem))
            {
                state = data;
                continue;
            }
            if((read.name() == "dx") && (state == data))
            {
                tmp.data.pos.setX(read.readElementText().toDouble());
                continue;
            }
            if((read.name() == "dy") && (state == data))
            {
                tmp.data.pos.setY(read.readElementText().toDouble());
                continue;
            }
            if((read.name() == "id") && (state == data))
            {
                tmp.data.touchId = read.readElementText().toInt();
                continue;
            }
            if((read.name() == "style") && (state == data))
            {
                tmp.data.style = (Qt::BrushStyle)read.readElementText().toInt();
                continue;
            }
            if((read.name() == "color") && (state == data))
            {
                tmp.data.color = (QRgb)read.readElementText().toUInt();
                continue;
            }
            if((read.name() == "width") && (state == data))
            {
                tmp.data.width = read.readElementText().toInt();
                continue;
            }
            if((read.name() == "time") && (state == data))
            {
                tmp.data.time = read.readElementText().toInt();
                continue;
            }
            if((read.name() == "effect") && (state == data))
            {
                tmp.data.effect = read.readElementText().toInt();
            }
        }
        if(token == QXmlStreamReader::EndElement)
        {
            if((read.name() == "data") && (state == data))
            {
                state = recElem;
                continue;
            }
            if((read.name() == "rec_elem") && (state == recElem))
            {
                recStream.append(tmp);

                state = none;

                continue;
            }
        }
    }
    if (read.hasError())
    {
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setText(read.errorString());
        msgBox->exec();
    }
    file.close();

}

void PaPlayer::sendData()
{
    paData tmp;
    do
    {
        tmp = recStream.at(sendIndex).data;
        tmp.guiId = this->guiId; // when reading the file, the id is maby not known. so set here.
        emit newData(tmp);
        sendIndex++;
        if (sendIndex==recStream.length())
            break;
    }
    while(recStream.at(sendIndex).timestamp==0);

    if (sendIndex<recStream.length()){
        timer->start(recStream.at(sendIndex).timestamp);
    }else{
        stop();
    }

}


void PaPlayer::start()
{
    sendIndex = 0;
    sendData();
}
void PaPlayer::stop()
{
    timer->stop();
    emit ready();
}
