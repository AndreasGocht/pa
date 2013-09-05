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


#ifndef PAWIDGET_H
#define PAWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <QTime>
#include "pabeamerdata.h"

class PaWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit PaWidget(QWidget *parent = 0);
    
signals:
    
public slots:
    void newData(PaData data);

private:
    QList<PaData> points;
    QTime time;
    QTimer *timer; //timer for repaint

    void paintEvent(QPaintEvent *);
    void drawBrush(QPainter &painter, const PaData &point);
    void drawLine(QPainter &painter, const PaData &point2, const PaData &point1);

};

#endif // PAWIDGET_H
