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


#include "pawidget.h"

PaWidget::PaWidget(QWidget *parent) :
    QGLWidget(parent)
{

    //black background
    //http://www.developer.nokia.com/Community/Wiki/Archived:Changing_QWidget_background_colour
    QPalette p(palette());
    // Set background colour to black
    p.setColor(QPalette::Background, Qt::black);
    setPalette(p);

//    QGLFormat GLOptions;
//    GLOptions.setSwapInterval(10);
//    QGLFormat::setDefaultFormat(GLOptions);


    time.start();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    timer->start(1000/30);//garant repaint, an a picture with 30 fps
}


void PaWidget::newData(PaData data)
{
    data.pos.setX(data.pos.x()*this->width());
    data.pos.setY(data.pos.y()*this->height());
    data.rect.setCoords(data.pos.x()-data.width/2,data.pos.y()-data.width/2,data.pos.x()+data.width/2,data.pos.y()+data.width/2);

    points.append(data);
}

void PaWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    bool found;

    for(int j=0; j<points.length(); j++)
    {
        switch (points.at(j).effect)
        {
            case POINT_LINE:
                drawBrush(painter,points.at(j));
                break;
            case LINE:
                found = false;
                for(int il=j+1; il<points.length(); il++ )
                {
                    if((points.at(il).guiId == points.at(j).guiId) && (points.at(il).touchId == points.at(j).touchId) && (points.at(il).effect == LINE ))
                    {
                        drawLine(painter,points.at(il),points.at(j));
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    drawBrush(painter,points.at(j));
                }
                break;
            case POINT:
                found = false;
                for(int ip = j + 1; ip<points.length(); ip++)
                {
                    if ((points.at(ip).guiId == points.at(j).guiId) && (points.at(ip).touchId == points.at(j).touchId))
                    {
                        //found a next point, so skip an dont draw this one
                        found = true;
                        break;
                    }
                }
                if (found)
                {
                    break;
                }else{
                    drawBrush(painter,points.at(j));
                    break;
                }
            }
    }

    //deleating timeout points
    int elapsed = time.restart(); //elapsed since last paintevent

    int i = 0;

    while(i<points.length() && (points.length()!=0))
    {
        if (0>(points[i].time -= elapsed))
        {
            points.removeAt(i);
            //ok the now the next one is on place i, so don't rise i an check this again.
        }else{
            i++; //ok this one are not out of time, ceck next one.
        }

    }
    if(points.length()==0)
    {
        points.clear(); //cleanup of memory
    }
}

void PaWidget::drawBrush(QPainter &painter,const PaData &point)
{
    QBrush brush;
    QRadialGradient grad;

    grad = QRadialGradient(point.pos,point.width/2,point.pos);
    grad.setColorAt(0,QColor(point.color));
    grad.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
    //brush.setColor(QColor(point.at(j)->color));
    //brush.setStyle(Qt::SolidPattern);
    brush = QBrush(grad);
    painter.fillRect(point.rect,brush);
    //painter.fillRect(point.at(j).rect,QColor(point.at(j).color));
}

void PaWidget::drawLine(QPainter &painter,const PaData &point2, const PaData &point1)
{
    QLineF line(point1.pos,point2.pos);
    PaData tmpPoint = point1;
    qreal dy = point1.width/4;
    qreal t = 0;
    qint32 i=1;
    while(t<1)
    {
        tmpPoint.pos = line.pointAt(t);
        tmpPoint.rect.setCoords(tmpPoint.pos.x()-tmpPoint.width/2,tmpPoint.pos.y()-tmpPoint.width/2,tmpPoint.pos.x()+tmpPoint.width/2,tmpPoint.pos.y()+tmpPoint.width/2);
        drawBrush(painter,tmpPoint);
        t = dy/line.length()*(i++);
    }

}
