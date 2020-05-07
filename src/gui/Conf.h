/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Conf.h
 * Author: serhiy
 *
 * Created on December 1, 2016, 7:51 PM
 */

#ifndef _CONF_H
#define _CONF_H

#include "ui_Conf.h"

class Conf : public QDialog {
    Q_OBJECT
public:
    Conf();
    virtual ~Conf();
private:
    Ui::Conf widget;
};

#endif /* _CONF_H */
