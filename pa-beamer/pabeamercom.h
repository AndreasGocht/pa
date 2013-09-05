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

#ifndef PABEAMERCOM_H
#define PABEAMERCOM_H

#include <QObject>
#include <QtXml/QtXml>
#include <QtNetwork/QtNetwork>
#include "paguiid.h"

class PaBeamerCom : public QObject
{
    Q_OBJECT

    enum parseState{
        noneParse
    };


public:
    explicit PaBeamerCom(PaGuiId* id, QTcpSocket *guiComSocket, QObject *parent = 0);
    ~PaBeamerCom();
    
signals:

public slots:
    void handleGuiNewData();
    void handleGuiDisconnect();

private:
    PaGuiId* guiIds;

    QTcpSocket *beamerGuiComSocket; //communication socket for commands
    QXmlStreamReader *readGui;
    QXmlStreamWriter *writeGui;

};

#endif // PABEAMERCOM_H
