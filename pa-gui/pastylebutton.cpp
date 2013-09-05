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

#include "pastylebutton.h"
#include <QMenu>

PaStyleButton::PaStyleButton(paStyle style,qint16 touchId, QString shortCut, QWidget *parent) :
    QPushButton(parent)
{

    this->style = style;
    this->touchId = touchId;

    QPixmap px(100,100);
    px.fill(QColor(style.color));

    this->setIcon(QIcon(px));

    QString text = "width:"+QString::number(style.width)+"\n";
    text += "time:"+QString::number(style.time)+"\n";
    text += "effect:"+QString::number(style.effect)+"\n";
    text += "short:"+shortCut;
    this->setText(text);

    this->resize(100,50);

    this->setShortcut(QKeySequence(shortCut));


    connect(this,SIGNAL(clicked()),this,SLOT(onClick()));

}


void PaStyleButton::onClick()
{
    emit setNewStyle(style,touchId);
}
