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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paguicom.h"
#include "pastylebutton.h"
#include "paplaybutton.h"
#include "pasettings.h"
#include "parecorder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    enum styleState{
        none,
        stateStyleButton,
        statePlayButton
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    PaGuiCom *com;

    QList<PaStyleButton*> styleButtons;
    QList<PaPlayButton*> playButtons;

    QWidget* buttonWidget;
    QString lastStylePath;
    PaSettings settings;
    PaRecorder recorder;

    void loadStyle(QString path);

public slots:
    void changeStyle(paStyle, qint16 touchId);
    void Load_Style();
    void Reload_Style();
    void Connect_to_Beamer();
    void Load_Settings();
    void Record();




};

#endif // MAINWINDOW_H
