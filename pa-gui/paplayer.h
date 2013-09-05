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


#ifndef PAPLAYER_H
#define PAPLAYER_H

#include <QObject>
#include <QByteArray>
#include <QtXml/QtXml>
#include <QList>
#include "paguicom.h"

struct paRecData{
    paData data;
    qint64 timestamp;
};


class PaPlayer : public QObject
{
    Q_OBJECT
    enum readState{
        none,
        recElem,
        data
    };

public:
    explicit PaPlayer(QObject *parent = 0);
    ~PaPlayer();

    void setPaCom(PaGuiCom* com);

    void loadFile(QString fileName);
    
signals:
    void newData(paData);
    void ready();
    
private slots:
    void sendData();

public slots:
    void start();
    void stop();

private:
    QList<paRecData> recStream;
    qint64 sendIndex;

    qint32 guiId;

    PaGuiCom* com;
    QTimer *timer;

};

#endif // PAPLAYER_H
