#-------------------------------------------------
#
# Project created by QtCreator 2013-03-12T17:53:45
#
#
# This file is part of pa (Project Anna).
#
# pa (Project Anna) is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# pa (Project Anna) is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with pa (Project Anna).  If not, see <http://www.gnu.org/licenses/>.
#
#-------------------------------------------------

QT       += core gui\
            xml\
            network

TARGET = pa-gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    paguicom.cpp \
    pastylebutton.cpp \
    pasettings.cpp \
    parecorder.cpp \
    paplayer.cpp \
    paplaybutton.cpp

HEADERS  += mainwindow.h \
    paguicom.h \
    pastylebutton.h \
    pasettings.h \
    parecorder.h \
    paplayer.h \
    paplaybutton.h

FORMS    += mainwindow.ui \
    pasettings.ui \
    parecorder.ui

OTHER_FILES += \
    style.xml \
    TODO.txt
