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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    com = new PaGuiCom(this);

    buttonWidget = new QWidget(this);
    this->setCentralWidget(buttonWidget);


}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadStyle(QString path)
{
    qDebug()<<"hi";


    QFile* styleFile= new QFile(path) ;
    if (!styleFile->open(QIODevice::ReadOnly))
    {
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setText("Style couldn't be loaded, Opening File went's' Wrong");
        msgBox->exec();
        return;
    }
    QXmlStreamReader styleRead;
    styleRead.setDevice(styleFile);

    styleState state = none;

    paStyle style;
    QColor tmpColor;
    qint16 touchId;
    QString shortcut;
    QPoint pos;
    QString recPath;
    PaStyleButton *styleButton;
    PaPlayButton *playButton;

    buttonWidget->deleteLater();
    buttonWidget = new QWidget(this);

    //delete old buttons
    for (int i=0; i<styleButtons.length();i++)
    {
        styleButtons[i]->deleteLater();
    }
    styleButtons.clear();
    for (int i=0; i<playButtons.length();i++)
    {
        playButtons[i]->deleteLater();
    }
    playButtons.clear();



    while(!styleRead.atEnd())
    {
        QXmlStreamReader::TokenType token = styleRead.readNext();

        //starting xml
        if(token == QXmlStreamReader::StartDocument) {
            //maybe init her the style
            continue;
        }

        if(token == QXmlStreamReader::StartElement) {
            qDebug()<<"startelement recived";
            qDebug()<<styleRead.name() ;

            if(styleRead.name() == "PaStyle") {
                qDebug()<< "PaStyleButton";
                continue;
            }


            if((styleRead.name() == "PaStyleButton") && (state == none)) {
                state = stateStyleButton;
                qDebug()<< "PaStyleButton";
                continue;
            }
            //stylebutton
                if((styleRead.name() == "brushStyle") && (state == stateStyleButton)) {
                    style.brushStyle = (Qt::BrushStyle)styleRead.readElementText().toInt();
                    continue;
                }
                if((styleRead.name() == "color") && (state == stateStyleButton)) {
                    tmpColor.setNamedColor(styleRead.readElementText());
                    style.color = tmpColor.rgb();
                    continue;
                }
                if((styleRead.name() == "time" ) && (state == stateStyleButton)) {
                    style.time = styleRead.readElementText().toInt();
                    continue;
                }
                if((styleRead.name() == "width" ) && (state == stateStyleButton)) {
                    style.width = styleRead.readElementText().toInt();
                    continue;
                }
                if((styleRead.name() == "effect" ) && (state == stateStyleButton)) {
                    style.effect = (paEffects)styleRead.readElementText().toInt();
                    continue;
                }
                if((styleRead.name() == "shortcut" ) && (state == stateStyleButton)) {
                    shortcut = styleRead.readElementText();
                    continue;
                }
                if((styleRead.name() == "touchId" ) && (state == stateStyleButton)) {
                    touchId = styleRead.readElementText().toInt();
                    continue;
                }
                if((styleRead.name() == "pos_x" ) && (state == stateStyleButton)) {
                    pos.setX(styleRead.readElementText().toInt());
                    continue;
                }
                if((styleRead.name() == "pos_y" ) && (state == stateStyleButton)) {
                    pos.setY(styleRead.readElementText().toInt());
                    continue;
                }
            // end Stylebutton

            if((styleRead.name() == "PaPlayButton") && (state == none)) {
                 state = statePlayButton;
                 qDebug()<< "PaPlayButton";
                 continue;
            }
            //playButton
                if((styleRead.name() == "shortcut" ) && (state == statePlayButton)) {
                    shortcut = styleRead.readElementText();
                    continue;
                }
                if((styleRead.name() == "pos_x" ) && (state == statePlayButton)) {
                    pos.setX(styleRead.readElementText().toInt());
                    continue;
                }
                if((styleRead.name() == "pos_y" ) && (state == statePlayButton)) {
                    pos.setY(styleRead.readElementText().toInt());
                    continue;
                }
                if((styleRead.name() == "path" ) && (state == statePlayButton)) {
                    recPath = styleRead.readElementText();
                    continue;
                }
            //end Playbutton


        }

        if (token == QXmlStreamReader::EndElement){
            if ((styleRead.name() == "PaStyleButton" ) && (state == stateStyleButton)){
                qDebug()<< "End PaStyleButton";
                state = none;
                styleButton = new PaStyleButton(style,touchId,shortcut,buttonWidget);
                styleButton->move(pos);

                styleButtons.append(styleButton);
                connect(styleButton,SIGNAL(setNewStyle(paStyle,qint16)),this,SLOT(changeStyle(paStyle,qint16)));
            }

            if ((styleRead.name() == "PaPlayButton" ) && (state == statePlayButton)){
                qDebug()<< "End PaPlayButton";
                state = none;
                playButton = new PaPlayButton(recPath, shortcut, buttonWidget);
                playButton->setPaCom(com);
                playButton->move(pos);

                playButtons.append(playButton);
            }

        }

    }
    if (styleRead.hasError())
    {
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setText(styleRead.errorString());
        msgBox->exec();
    }


    this->setCentralWidget(buttonWidget);

    qDebug()<<"end";

}

void MainWindow::changeStyle(paStyle style,qint16 touchId)
{
    com->setStyle(style,touchId);
}


//UI Interfaces
void MainWindow::Load_Style()
{
    lastStylePath= QFileDialog::getOpenFileName(this,tr("Open Style"), QDir::homePath(), tr("XML-Style Filde (*.xml)"));
    if (!lastStylePath.isEmpty())
    {
        this->loadStyle(lastStylePath);
    }
}

void MainWindow::Reload_Style()
{
    if (!lastStylePath.isEmpty())
    {
        this->loadStyle(lastStylePath);
    }
}

void MainWindow::Connect_to_Beamer()
{
    com->connectToBeamerServer(QHostAddress(settings.getBeamerIp()));
}

void MainWindow::Load_Settings()
{
    settings.show();
}

void MainWindow::Record()
{
    recorder.setPaCom(com);
    recorder.show();
}
