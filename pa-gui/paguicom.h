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


#ifndef PAGUICOM_H
#define PAGUICOM_H

#include <QObject>
#include <QtXml/QtXml>
#include <QtNetwork/QtNetwork>
#include <QtGui>
#include <QDebug>
#include <QPainter>


enum initState{
    noneInit,
    init,
    portScreen
};

enum touchState{
    noneTouch,
    touch
};

enum paEffects{
    POINT_LINE,
    LINE, //don't work at the moment
    POINT
};

//paStyle save the styleinformation witch sould applyed to the touchpoints
struct paStyle{
    //QPixmap px; TODO: transmitt pixmaps --> maybe performens upgrades
    Qt::BrushStyle brushStyle;
    QRgb color;
    qreal width;
    qint16 time; //time in ms
    paEffects effect;
};

//paData saves the whole point, wiht information
//its equivalent to paData from pa-beamer
struct paData{
    QPointF pos;
//    QRectF rect; //ony necesary in pa-beamer
    Qt::BrushStyle style; //not implemented on pa-beamer at the moment
    QRgb color;
    qreal width;
    qint16 time; //time in ms
    qint16 effect;
    qint16 touchId;
    qint16 guiId;
};

class PaGuiCom : public QObject
{
    Q_OBJECT
public:
    explicit PaGuiCom(QObject *parent = 0);
    ~PaGuiCom();
    bool connectToBeamerServer(QHostAddress ip = QHostAddress("127.0.0.1"));
    void setStyle(paStyle newStyle, qint16 touchId);
    qint32 getNewGuiId(); //get id's z.B. for recorded files

signals:

signals:
    void newData(paData);

public slots:

private:
    QXmlStreamReader *smartRead;
    QTcpServer *guiServer;
    QTcpSocket *guiSmartSocket; //the only socket to comunicate withe the smartphone !!ONLY ONE SMARTPHONE PER PC!!
    touchState readState;
    QList<QString> bufferdReadData; //first dx, second dy, third id

    QTcpSocket *guiBeamerComSocket; //command interface
    QXmlStreamReader *beamerComRead;  //send requests
    QXmlStreamWriter *beamerComWrite; //recive answers


    QTcpSocket *guiBeamerDataSocket;
//    QXmlStreamReader *beamerDataRead; //read the Broadcasting data
    QXmlStreamWriter *beamerDataWrite; //send data

    qint16 maxTouch;
    paStyle defaultStyle;
    QList<paStyle> styles;

    qint16 guiId;

private slots:
    //smartphone stuff
    void handleSmartNewConnection();
    void handleSmartNewData();
    void handleSmartDisconnect();

    //data stuff
//    void handleBroadcastNewConnection();
//    void handleBroadcastNewData();
//    void handleBroadcastNewDisconnect();

    //com stuff

    //void handleBeamerNewData();
    void handleBeamerDisconnect();

    void sendNewData(paData data);
};

#endif // PAGUICOM_H
