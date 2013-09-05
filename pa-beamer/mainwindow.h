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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXml/QtXml>
#include <QtNetwork/QtNetwork>
#include <QtGui>
#include <QDebug>
#include <QPainter>
#include "pabeamerdata.h"
#include "pabeamercom.h"
#include "pawidget.h"
#include "paguiid.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

    PaWidget *drawArea;

    QTcpServer *beamerServer;
    QList<PaBeamerCom*>* beamerComs;

    QTcpServer *beamerDataServer;
    QList<PaBeamerData*>* beamerDatas;

    PaGuiId* guiIds;

    void resizeEvent(QResizeEvent *event);

private slots:
    void handleGuiNewDataConnection();
    void broadcastNewData(PaData data);

    void handleGuiNewConnection();

signals:
    void newData(PaData);
};

#endif // MAINWINDOW_H
