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

#include "parecorder.h"
#include "ui_parecorder.h"
#include <QDebug>

PaRecorder::PaRecorder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PaRecorder)
{
    ui->setupUi(this);
    timer = new QTimer();
}

PaRecorder::~PaRecorder()
{
    delete ui;
}

void PaRecorder::setPaCom(PaGuiCom *com)
{
    this->com = com;
    connect(this,SIGNAL(newData(paData)),this->com,SLOT(sendNewData(paData)));
}

void PaRecorder::on_Stop_clicked()
{
    disconnect(com,SIGNAL(newData(paData)),this,SLOT(recNewData(paData)));


// debug stuff
//    QFile file("/home/andreas/tmp/parec_debug");
//    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//         return;
//    QTextStream out(&file);
//    for(int i = 0; i< recStream.length();i++)
//    {
//        out << QString::number(recStream.at(i).timestamp)<<"\n";
//    }
//    file.close();

}

void PaRecorder::on_Start_clicked()
{

    recStream.clear();
    connect(com,SIGNAL(newData(paData)),this,SLOT(recNewData(paData)));
    time.start();

}

void PaRecorder::on_Play_clicked()
{

    on_StopPlay_clicked();

    connect(timer,SIGNAL(timeout ()),this,SLOT(sendData()));
    sendIndex = 0;
    sendData();
}

void PaRecorder::on_StopPlay_clicked()
{
    timer->stop();
    disconnect(timer,SIGNAL(timeout()),this,SLOT(sendData()));

}

void PaRecorder::recNewData(paData data)
{
    paRecData tmp;
    tmp.data = data;
    tmp.timestamp = time.restart();
    recStream.append(tmp);

}

void PaRecorder::sendData()
{
    do
    {
        emit newData(recStream.at(sendIndex).data);
        sendIndex++;
        if (sendIndex==recStream.length())
            break;
    }
    while(recStream.at(sendIndex).timestamp==0);

    if (sendIndex<recStream.length()){
        timer->start(recStream.at(sendIndex).timestamp);
    }else{
        on_StopPlay_clicked();
    }

}


void PaRecorder::on_Save_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),QDir::homePath(), tr("XML-Style Filde (*.xml)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
         return;
    QXmlStreamWriter write(&file);

    write.setAutoFormatting(true);
    write.writeStartDocument();
    write.writeStartElement("pa_rec_file");

    for(int i = 0; i< recStream.length();i++)
    {
        write.writeStartElement("rec_elem");
        write.writeTextElement("dt",QString::number(recStream.at(i).timestamp));
            write.writeStartElement("data");
                write.writeTextElement("dx",QString::number(recStream.at(i).data.pos.x()));
                write.writeTextElement("dy",QString::number(recStream.at(i).data.pos.y()));
                write.writeTextElement("id",QString::number(recStream.at(i).data.touchId));
                write.writeTextElement("style",QString::number(recStream.at(i).data.style));
                write.writeTextElement("color",QString::number(recStream.at(i).data.color));
                write.writeTextElement("width",QString::number(recStream.at(i).data.width));
                write.writeTextElement("time",QString::number(recStream.at(i).data.time));
                write.writeTextElement("effect",QString::number(recStream.at(i).data.effect));
            write.writeEndElement();
        write.writeEndElement();
    }
    write.writeEndDocument();
    file.close();

}
