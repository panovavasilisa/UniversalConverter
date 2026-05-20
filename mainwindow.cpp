#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include "baseparser.h"
#include "bigfraction.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

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

    bool ok;
    int p = pStr.toInt(&ok);
    if (!ok || p < 2 || p > 500) {
        QMessageBox::warning(this, "Ошибка", "Основание p должно быть целым от 2 до 500");
        return;
    }

    try {
        string inputStd = numberStr.toStdString();
        BigFraction fr = BaseParser::parse(inputStd, p);
        QMessageBox::information(this, "Проверка", "Число корректно");
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка ввода", e.what());
    }


    //qDebug() << "Проверка: число =" << numberStr << "p =" << pStr << "q =" << qStr;

    //QMessageBox::information(this, "Проверка", "Число введено (проверка ещё не реализована)");
}

void MainWindow::onCalculate() {
    QString numberStr = ui->line_my_number->text();
    QString pStr = ui->line_p->text();
    QString qStr = ui->line_q->text();

    //qDebug() << "Конвертация: число =" << numberStr << "p =" << pStr << "q =" << qStr;
    //BigInteger i(123);
    BigFraction f(123, 1000);

    ui->line_ans_number->setText("Результат будет здесь");
}
