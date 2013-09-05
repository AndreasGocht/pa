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

#include "pasettings.h"
#include "ui_pasettings.h"
#include "QDir"
#include "QtXml"

PaSettings::PaSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PaSettings)
{
    ui->setupUi(this);
    connect(this,SIGNAL(accepted()),this,SLOT(apply()));

    //load settings
    QString dir = QDir::homePath()+"/.pa/settings.xml";
    qDebug()<<dir;
    settingsFile = new QFile(dir);

    if(settingsFile->open(QIODevice::ReadOnly))
    {
        QXmlStreamReader read;
        read.setDevice(settingsFile);
        while(!read.atEnd())
        {
            QXmlStreamReader::TokenType token = read.readNext();
            if(token == QXmlStreamReader::StartDocument) {
                continue;
            }
            if(token == QXmlStreamReader::StartElement) {
                if(read.name() == "settings") {
                    continue;
                }
                if(read.name() == "beamerIP") {
                    beamerIP = read.readElementText();
                    continue;
                }
            }
        }
        if (read.hasError())
        {
            beamerIP = "127.0.0.1";
        }
        settingsFile->close();
    }
    else //not optimum
    {
        beamerIP = "127.0.0.1";
    }




}

PaSettings::~PaSettings()
{
    delete ui;
}

QString PaSettings::getBeamerIp()
{
    return beamerIP;
}

void PaSettings::apply()
{
    this->beamerIP= this->ui->beamerIPLineEdit->text();

    if(settingsFile->open(QIODevice::WriteOnly))
    {
        QXmlStreamWriter write;
        write.setDevice(settingsFile);
        write.setAutoFormatting(true);
        write.writeStartDocument();
        write.writeStartElement("settings");
            write.writeTextElement("beamerIP",beamerIP);
            write.writeEndElement();
        write.writeEndDocument();
    }
    else
    {
        qDebug()<<settingsFile->errorString();
    }
    settingsFile->close();
}
