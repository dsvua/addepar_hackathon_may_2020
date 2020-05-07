/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MainWindow.h
 * Author: serhiy
 *
 * Created on November 25, 2016, 11:19 AM
 */

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include "ui_MainWindow.h"
#include <QKeyEvent>
#include <QTcpSocket>
#include <Qt>
#include <QDebug>
#include <QMouseEvent>
#include <QtConcurrent/QtConcurrentRun>
#include <typeinfo>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGst/Ui/GraphicsVideoSurface>
#include <QGst/Ui/GraphicsVideoWidget>
#include <QGLWidget>
#include <QGridLayout>
#include <QGst/Global>
#include <QGst/ElementFactory>
#include <QGst/Pipeline>
#include <QGst/Parse>
#include <QGst/Ui/VideoWidget>
#include "ui_Conf.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
    bool sendMessage(QString message);
    QString receiveMessage();
private:
    Ui::MainWindow widget;
    QTcpSocket socket_;
    QString host_;
    int port_;
    bool mouseGrabbed_;
    QCursor originalMouseCursor_;
    QPoint widgetCenter_;
    QPoint originalMousePoint_;
    int mouseBorder_;
    int mouseIgnoreMoveRadius_;
    char * caps_;
    QString pipeDescr_;
    QGst::PipelinePtr pipeline_;
    //QGst::Ui::VideoWidget *testWidget;
    
protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);
    
public slots:
    void connectToHost();
    void disconnectedFromHost();
    void readyRead();
    void configureIP();
};

#endif /* _MAINWINDOW_H */
