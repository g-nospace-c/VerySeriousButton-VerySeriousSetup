CONFIG += qt resources release
QT += svg xml widgets core gui

HEADERS += VerySeriousSetup.h
HEADERS += BigGoButton.h
HEADERS += DeviceListDialog.h
HEADERS += KeyseqOptionsBox.h
HEADERS += VsbDeviceSelector.h
HEADERS += VsbModeSelector.h
HEADERS += SingleKeyOptionsBox.h

SOURCES += VerySeriousSetup.cpp
SOURCES += BigGoButton.cpp
SOURCES += DeviceListDialog.cpp
SOURCES += KeyseqOptionsBox.cpp
SOURCES += VsbDeviceSelector.cpp
SOURCES += VsbModeSelector.cpp 
SOURCES += SingleKeyOptionsBox.cpp
SOURCES += keyseq.cpp
SOURCES += vsbprog.cpp
SOURCES += main.cpp


RESOURCES += rc/images.qrc

win32 {
  LIBS += -lboost_locale-mt
  LIBS += -lboost_regex-mt
  CONFIG += static
  #LIBS += -L/usr/lib/mxe/usr/x86_64-w64-mingw32.static/qt5/lib/
  #LIBS += -lQt5Svg
  LIBS += -lhidapi
  LIBS += -lsetupapi
  QMAKE_CXXFLAGS += -static-libstdc++ -static-libgcc
  DEFINES += STATIC
  INCLUDEPATH += /usr/lib/mxe/usr/x86_64-w64-mingw32.static/include/hidapi
  }

unix {
  LIBS += -lboost_locale
  LIBS += -lboost_regex
  LIBS += -lhidapi-hidraw
  INCLUDEPATH += /usr/include/hidapi
  QMAKE_CXXFLAGS += -std=c++20
  }

TARGET = VerySeriousSetup
