#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Подключаем кнопки к слотам (используем правильные имена из .ui)
    connect(ui->btn_check_valid, &QPushButton::clicked, this, &MainWindow::onCheckValid);
    connect(ui->btn_calculate, &QPushButton::clicked, this, &MainWindow::onCalculate);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onCheckValid() {
    QString numberStr = ui->line_my_number->text();
    QString pStr = ui->line_p->text();
    QString qStr = ui->line_q->text();



    //qDebug() << "Проверка: число =" << numberStr << "p =" << pStr << "q =" << qStr;

    //QMessageBox::information(this, "Проверка", "Число введено (проверка ещё не реализована)");
}

void MainWindow::onCalculate() {
    QString numberStr = ui->line_my_number->text();
    QString pStr = ui->line_p->text();
    QString qStr = ui->line_q->text();

    //qDebug() << "Конвертация: число =" << numberStr << "p =" << pStr << "q =" << qStr;

    ui->line_ans_number->setText("Результат будет здесь");
}
