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


#ifndef PAPLAYBUTTON_H
#define PAPLAYBUTTON_H

#include <QPushButton>
#include <QtGui>
#include "paplayer.h"

class PaPlayButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PaPlayButton(QString fileName,QString shortCut, QWidget *parent = 0);
    
    void setPaCom(PaGuiCom * com);

private:
    PaPlayer player;
    bool running;

signals:
    
public slots:

private slots:

    void onReady();
    void onClicked();

};

#endif // PAPLAYBUTTON_H
