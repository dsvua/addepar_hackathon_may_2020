/********************************************************************************
** Form generated from reading UI file 'Conf.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONF_H
#define UI_CONF_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_Conf
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *serverIPaddress;
    QLabel *labelServerIP;
    QLabel *labelServerPort;
    QLineEdit *serverPort;
    QString IPaddress;
    QString IPport;
    QString getIP(){
        return serverIPaddress->text();
    }
    QString getPort(){
        return serverPort->text();
    }
    void setupUi(QDialog *Conf)
    {
        if (Conf->objectName().isEmpty())
            Conf->setObjectName(QStringLiteral("Conf"));
        Conf->resize(400, 300);
        buttonBox = new QDialogButtonBox(Conf);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        serverIPaddress = new QLineEdit(Conf);
        serverIPaddress->setObjectName(QStringLiteral("serverIPaddress"));
        serverIPaddress->setText(IPaddress);
        serverIPaddress->setGeometry(QRect(120, 21, 251, 20));
        labelServerIP = new QLabel(Conf);
        labelServerIP->setObjectName(QStringLiteral("labelServerIP"));
        labelServerIP->setGeometry(QRect(10, 24, 111, 16));
        labelServerPort = new QLabel(Conf);
        labelServerPort->setObjectName(QStringLiteral("labelServerPort"));
        labelServerPort->setGeometry(QRect(46, 63, 71, 16));
        serverPort = new QLineEdit(Conf);
        serverPort->setObjectName(QStringLiteral("serverPort"));
        serverPort->setGeometry(QRect(120, 60, 71, 20));
        serverPort->setText(IPport);

        retranslateUi(Conf);
        QObject::connect(buttonBox, SIGNAL(accepted()), Conf, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Conf, SLOT(reject()));

        QMetaObject::connectSlotsByName(Conf);
    } // setupUi

    void retranslateUi(QDialog *Conf)
    {
        Conf->setWindowTitle(QApplication::translate("Conf", "Configuration", 0));
        labelServerIP->setText(QApplication::translate("Conf", "Server IP address", 0));
        labelServerPort->setText(QApplication::translate("Conf", "Server Port", 0));
    } // retranslateUi

};

namespace Ui {
    class Conf: public Ui_Conf {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONF_H
