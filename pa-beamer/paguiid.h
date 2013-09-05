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

#ifndef PAGUIID_H
#define PAGUIID_H

#include <QObject>

class PaGuiId : public QObject
{
    Q_OBJECT
public:
    explicit PaGuiId(QObject *parent = 0);
    qint32 getId();
    
signals:
    
public slots:
    

private:
    qint32 idCount;
};

#endif // PAGUIID_H
