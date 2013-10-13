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


#include "framelistener.h"
#include <QDebug>
#include <QPoint>

FrameListener::FrameListener(QObject *parent) :
    Leap::Listener()
{
    qDebug()<< "hi";
}

void FrameListener::onFrame(const Leap::Controller &controller)
{
    paData point;
    Leap::FingerList fingers = controller.frame().fingers();
    if(!fingers.empty())
    {
        for(int i=0; i < fingers.count(); i++)
        {
            if ((fingers[i].touchZone() ==  Leap::Pointable::ZONE_HOVERING)||(fingers[i].touchZone() ==  Leap::Pointable::ZONE_TOUCHING))
            {
                point.pos.setX(0.5+(fingers[i].tipPosition().x/600));
                point.pos.setY(1-(fingers[i].tipPosition().y/500));
                point.touchId = i;
                emit newPoint(point);
            }
        }

    }

}

void FrameListener::onConnect(const Leap::Controller &controller)
{
     qDebug()<<"conncet";
}

void FrameListener::onInit(const Leap::Controller &controller)
{
    qDebug()<<"now init ^^";
}
