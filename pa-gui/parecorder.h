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

#ifndef PARECORDER_H
#define PARECORDER_H

#include <QDialog>
#include <QByteArray>
#include <QtXml/QtXml>
#include <QList>
#include "paguicom.h"
#include "paplayer.h"

namespace Ui {
class PaRecorder;
}


class PaRecorder : public QDialog
{
    Q_OBJECT
    
public:
    explicit PaRecorder(QWidget *parent = 0);
    ~PaRecorder();

    void setPaCom(PaGuiCom* com);
    
private slots:
    void on_Stop_clicked();

    void on_Start_clicked();

    void on_Play_clicked();

    void on_StopPlay_clicked();

    void recNewData(paData data);
    void sendData();




    void on_Save_clicked();

private:
    Ui::PaRecorder *ui;

    QList<paRecData> recStream;
    QXmlStreamReader recorder;
    qint64 sendIndex;

    PaGuiCom* com;

    QTime time; //time since last newData event
    QTimer *timer;

signals:
    void newData(paData);

};

#endif // PARECORDER_H
