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


#ifndef PALEAPCOM_H
#define PALEAPCOM_H

#include <QObject>
#include <QtXml/QtXml>
#include <QtNetwork/QtNetwork>
#include <QtGui>
#include <QDebug>


//outcommente used only in the pther parts
struct paData{
    QPointF pos;
//    QRectF rect; //ony necesary in pa-beamer
//    Qt::BrushStyle style; //not implemented on pa-beamer at the moment
//    QRgb color;
//    qreal width;
//    qint16 time; //time in ms
//    qint16 effect;
    qint16 touchId;
//    qint16 guiId;
};


class paLeapCom : public QObject
{
    Q_OBJECT
public:
    explicit paLeapCom(qint16 maxTouch, QObject *parent = 0);
    ~paLeapCom();
    bool connectToGui(QHostAddress);

    QXmlStreamWriter *write;
    QTcpSocket *guiServer;

    paData* bufferdData;
    QPointF* bufferdPoint;

    qint16 maxTouch; //maxtouches this app or device supports. I don't know how to calculate the max touches a device supports, but i think, necassitas supports only 3 at the moment


signals:
    
private slots:
    void handleDisconnect();

    void newPoint(paData);
    
};

#endif // PALEAPCOM_H
