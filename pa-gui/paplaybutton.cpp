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

#include "paplaybutton.h"

PaPlayButton::PaPlayButton(QString fileName ,QString shortCut ,QWidget *parent) :
    QPushButton(parent)
{
    player.loadFile(fileName);
    running = false;

    QString text = fileName +"\n";
    text += "short:"+shortCut;
    this->setText(text);

    this->resize(100,50);

    this->setShortcut(QKeySequence(shortCut));

    connect(&player,SIGNAL(ready()),this,SLOT(onReady()));
    connect(this,SIGNAL(clicked()),this,SLOT(onClicked()));

}

void PaPlayButton::setPaCom(PaGuiCom *com)
{
    player.setPaCom(com);
}

void PaPlayButton::onReady()
{
    running = false;
}

void PaPlayButton::onClicked()
{
    if (running)
    {
        player.stop();
    }else{
        player.start();
    }
}
