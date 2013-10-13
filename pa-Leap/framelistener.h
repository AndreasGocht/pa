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

#ifndef FRAMELISTENER_H
#define FRAMELISTENER_H

#include "Leap.h"
#include <QObject>
#include "paleapcom.h"

class FrameListener : public QObject, public Leap::Listener
{
    Q_OBJECT
public:
    explicit FrameListener(QObject *parent = 0);
    virtual void onFrame(const Leap::Controller &controller);
    virtual void onInit(const Leap::Controller &controller);
    virtual void onConnect(const Leap::Controller &controller);

signals:
    void newPoint(paData);

public slots:
    
};

#endif // FRAMELISTENER_H
