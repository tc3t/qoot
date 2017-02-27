TEMPLATE = app

win32 {
   QMAKE_CXXFLAGS += -FIw32pragma.h  
}
CONFIG += qt warn_on thread

INCLUDEPATH += $(ROOTSYS)/include
win32:LIBS += -L$(ROOTSYS)/lib -llibCore -llibCint -llibRIO \
        -llibHist -llibGraf -llibGraf3d -llibGpad -llibTree \
        -llibRint -llibMatrix \
        -llibGui -llibRGL 
else:LIBS += -L$(ROOTSYS)/lib -lCore -lCint -lRIO \
        -lHist -lGraf -lGraf3d -lGpad -lTree \
        -lRint -lMatrix \
        -lGui -lRGL 

QT += core gui widgets

HEADERS += canvas.h
SOURCES += canvas.cxx graphDrawingPerformanceExample.cpp


