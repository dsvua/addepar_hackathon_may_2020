TARGET = car_gui
DESTDIR = ../../build
BUILDDIR = ../../build

QT += widgets
QT += network
QT += opengl
QT += opengl

HEADERS += Conf.h
HEADERS += MainWindow.h
HEADERS += StreamRtpClient.h
HEADERS += ui_Conf.h
HEADERS += ui_MainWindow.h

SOURCES += main.cpp
SOURCES += Conf.cpp
SOURCES += MainWindow.cpp
SOURCES += StreamRtpClient.cpp

CONFIG += link_pkgconfig \
            c++11

PKGCONFIG += Qt5GStreamer-1.0 \
             Qt5GStreamerUi-1.0 \
             glib-2.0 \
             gobject-2.0 \
             gio-2.0 \
             gstreamer-1.0 \
             gstreamer-app-1.0 \
             gstreamer-pbutils-1.0
