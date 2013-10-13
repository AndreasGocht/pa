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
    //points.clear();//only for testing


    data.pos.setX(data.pos.x()*this->width());
    data.pos.setY(data.pos.y()*this->height());
    data.rect.setCoords(data.pos.x()-data.width/2,data.pos.y()-data.width/2,data.pos.x()+data.width/2,data.pos.y()+data.width/2);


    //big nonsens :-) but nice efect XD
    //Problem: if use 2 fingers to much points ... the graficcarf of my pc is to bad for this :-)
//    if (data.efect==1)
//    {
//        if (points.length()>0)
//        {
//            PaData tmp = points.last();

//            //selber denken ist gerade ausverkauft :-)
//            //TODO: good mathematikal interpolation
//            QLineF line(data.pos,tmp.pos);
//            QPointF tmpPoint;
//            PaData tmpData;
//            tmpData = data;
//            qreal asdf = 0;
//            qreal asdf2 = 0;
//            asdf=asdf + tmp.width/4;
//            while (asdf<line.length())
//            {
//                asdf2=asdf/line.length();
//                tmpPoint = line.pointAt(asdf2);
//                tmpData.pos = tmpPoint;
//                tmpData.rect.setCoords(tmpData.pos.x()-tmpData.width/2,tmpData.pos.y()-tmpData.width/2,tmpData.pos.x()+tmpData.width/2,tmpData.pos.y()+tmpData.width/2);
//                points.append(tmpData);
//                asdf=asdf + tmpData.width/4;
//            }
//        }
//    }

    points.append(data);
    //this->update();
}

void PaWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QVector<qint16> tmp; //first is guiID second tocuh ID
    bool found;

    //for LINE effect
    QListIterator< PaData > lineI(points);
    lineI.toFront();


    //for POINT Effect
    QList< QVector<qint16> > drawedPoints; //first is guiID second tocuh ID


    for(int j=0; j<points.length(); j++)
    {
        switch (points.at(j).effect)
        {
            case POINT_LINE:
                drawBrush(painter,points.at(j));
                break;
            case LINE:
                tmp.clear();
                tmp.append(points.at(j).guiId);
                tmp.append(points.at(j).touchId);
                //if there is an other point with the id, draw a line from ther to this point, if not, draw only this point
                found = false;

                for(int i = j + 1; i<points.length();i++)
                {
                    if ((points.at(i).guiId == points.at(j).guiId) && (points.at(i).touchId == points.at(j).touchId) && (points.at(i).effect == LINE))
                    {
                        found = true;
                        drawLine(painter,points.at(i),points.at(j));
                        break;
                    }
                }
                if (!found)
                {
                    drawBrush(painter,points.at(j));
                }

                tmp.clear();
                break;
            case POINT:
                found = false;
                for(int i = j + 1; i<points.length(); i++)
                {
                    if ((points.at(i).guiId == points.at(j).guiId) && (points.at(i).touchId == points.at(j).touchId))
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
//                QListIterator< QVector<qint16> >pointI(drawedPoints);

//                tmp.clear();
//                tmp.append(points.at(j).guiId);
//                tmp.append(points.at(j).touchId);
//                //if no point from this id is drawn bevor draw the point and save it

//                if (!pointI.findNext(tmp))
//                {
//                    drawBrush(painter,points.at(j));
//                    drawedPoints.append(tmp);
//                }


            }
    }

    //deleating timeout points
    int elapsed = time.restart(); //elapsed since last paintevent

    for(int i=0;i<points.length();i++)
    {
        if (0>(points[i].time -= elapsed))
        {
            points.removeAt(i);
        }
    }
    if(points.length()==0)
    {
        points.clear(); //cleanup of memory
    }

    drawedPoints.clear();

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
    qint32 i;
    while(t<1)
    {
        tmpPoint.pos = line.pointAt(t);
        tmpPoint.rect.setCoords(tmpPoint.pos.x()-tmpPoint.width/2,tmpPoint.pos.y()-tmpPoint.width/2,tmpPoint.pos.x()+tmpPoint.width/2,tmpPoint.pos.y()+tmpPoint.width/2);
        drawBrush(painter,tmpPoint);
        t = dy/line.length()*(i++);
    }
    //drawBrush(painter,point2);

}
