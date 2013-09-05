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

#ifndef PASTYLEBUTTON_H
#define PASTYLEBUTTON_H

#include <QPushButton>
#include <QtGui>
#include "paguicom.h"

class PaStyleButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PaStyleButton(paStyle style, qint16 touchId,QString shortCut, QWidget *parent = 0);
signals:
    void setNewStyle(paStyle,qint16);

public slots:

private:
    paStyle style;
    qint16 touchId;

private slots:
    void onClick();
    
};

#endif // PASTYLEBUTTON_H
