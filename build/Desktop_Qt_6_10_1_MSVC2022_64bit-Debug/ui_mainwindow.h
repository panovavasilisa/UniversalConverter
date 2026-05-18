/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *btn_check_valid;
    QLineEdit *line_p;
    QLineEdit *line_my_number;
    QLineEdit *line_ans_number;
    QCommandLinkButton *btn_calculate;
    QLineEdit *line_q;
    QLabel *label_3;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *label_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        btn_check_valid = new QPushButton(centralwidget);
        btn_check_valid->setObjectName("btn_check_valid");
        btn_check_valid->setGeometry(QRect(130, 270, 81, 18));
        line_p = new QLineEdit(centralwidget);
        line_p->setObjectName("line_p");
        line_p->setGeometry(QRect(120, 160, 31, 20));
        line_my_number = new QLineEdit(centralwidget);
        line_my_number->setObjectName("line_my_number");
        line_my_number->setGeometry(QRect(130, 230, 211, 19));
        line_ans_number = new QLineEdit(centralwidget);
        line_ans_number->setObjectName("line_ans_number");
        line_ans_number->setGeometry(QRect(470, 230, 211, 20));
        btn_calculate = new QCommandLinkButton(centralwidget);
        btn_calculate->setObjectName("btn_calculate");
        btn_calculate->setGeometry(QRect(390, 220, 31, 41));
        line_q = new QLineEdit(centralwidget);
        line_q->setObjectName("line_q");
        line_q->setGeometry(QRect(430, 170, 31, 20));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(130, 90, 551, 61));
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(130, 20, 551, 61));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        label_2 = new QLabel(horizontalLayoutWidget);
        label_2->setObjectName("label_2");
        label_2->setMouseTracking(true);

        horizontalLayout->addWidget(label_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 18));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btn_check_valid->setText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\321\201\321\202\320\270", nullptr));
        btn_calculate->setText(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">  \320\236\321\201\320\275\320\276\320\262\320\260\320\275\320\270\321\217 \321\201\320\270\321\201\320\270\321\202\320\265\320\274 \321\201\321\207\320\270\321\201\320\273\320\265\320\275\320\270\321\217 </p></body></html>", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">\320\230\321\201\321\205\320\276\320\264\320\275\320\276\320\265 \321\207\320\270\321\201\320\273\320\276</p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202</p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
