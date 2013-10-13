#-------------------------------------------------
#
# Project created by QtCreator 2013-08-12T11:55:03
#
#This file is part of pa (Project Anna).
#
#pa (Project Anna) is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#pa (Project Anna) is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with pa (Project Anna).  If not, see <http://www.gnu.org/licenses/>.
#
#
#
#-------------------------------------------------

QT       += core gui\
            xml\
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pa-Leap
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    framelistener.cpp \
    paleapcom.cpp

HEADERS  += mainwindow.h \
    framelistener.h \
    paleapcom.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    libLeap.so

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/release/ -lLeap
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/debug/ -lLeap
else:unix: LIBS += -L$$PWD/ -lLeap

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
