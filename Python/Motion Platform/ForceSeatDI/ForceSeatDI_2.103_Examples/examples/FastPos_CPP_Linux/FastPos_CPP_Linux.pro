DEFINES += console
CONFIG += C++11
QT-= gui

DEFINES += NO_IMPORT_FORCESEAT_DI

SOURCES += $$PWD/main.cpp
SOURCES += $$PWD/../../code/ForceSeatDI_Loader.c

HEADERS += $$PWD/../../code/ForceSeatDI.h
HEADERS += $$PWD/../../code/ForceSeatDI_Structs.h
HEADERS += $$PWD/../../code/ForceSeatDI_Functions.h
HEADERS += $$PWD/../../code/ForceSeatDI_Defines.h

INCLUDEPATH += $$PWD/../../code

LIBS += -ldl
