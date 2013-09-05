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


//class for comunication with gui's


#ifndef PAGUICOM_H
#define PAGUICOM_H

#include <QObject>
#include <QtXml/QtXml>
#include <QtNetwork/QtNetwork>
#include <QtGui>
#include <QDebug>
#include <QPainter>

struct PaData{
    QPointF pos;
    QRectF rect;
    Qt::BrushStyle style;
    QRgb color;
    qreal width;
    qint16 time; //time in ms
    qint16 effect;
    qint16 touchId;
    qint32 guiId;
};


enum dataState{
    noneData,
    data
};

enum paEffects{
    POINT_LINE,
    LINE, //don't work at the moment
    POINT
};


class PaBeamerData : public QObject
{
    Q_OBJECT
public:
    explicit PaBeamerData(QObject *parent = 0);
    ~PaBeamerData();
    void setSocket(QTcpSocket* guiSocket);


public slots:
    void handleGuiNewData();
    void handleGuiDisconnect();
    void sendOtherData(PaData data);

signals:
    void newData(PaData);

private:

    QTcpSocket *beamerGuiSocket; //communication socket for data from gui
    QXmlStreamReader *readGui;
    QXmlStreamWriter *writeGui; //chan for broadcast data
    dataState readState;

    PaData* bufferdData;
    QPointF* bufferdPoint;

//    qint16 guiId;

};

#endif // PAGUICOM_H
