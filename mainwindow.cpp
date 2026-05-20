#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include "baseparser.h"
#include "bigfraction.h"
#include "baseconverter.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    qDebug() << ui->btn_load->objectName();
    this->setStyleSheet("QMainWindow { background-color: #c0e0ff; }");

    connect(ui->btn_check_valid, &QPushButton::clicked, this, &MainWindow::onCheckValid);
    connect(ui->btn_calculate, &QPushButton::clicked, this, &MainWindow::onCalculate);
    connect(ui->btn_load, &QPushButton::clicked, this, &MainWindow::onLoadFile);
    connect(ui->btn_save, &QPushButton::clicked, this, &MainWindow::onSaveFile);
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
}


void MainWindow::onCalculate() {
    QString numberStr = ui->line_my_number->text();
    QString pStr = ui->line_p->text();
    QString qStr = ui->line_q->text();

    bool pOk, qOk;
    int p = pStr.toInt(&pOk);
    int q = qStr.toInt(&qOk);

    if (!pOk || p < 2 || p > 500) {
        ui->line_ans_number->setText("Ошибка: основание p должно быть целым от 2 до 500");
        return;
    }
    if (!qOk || q < 2 || q > 500) {
        ui->line_ans_number->setText("Ошибка: основание q должно быть целым от 2 до 500");
        return;
    }
    if (numberStr.isEmpty()) {
        ui->line_ans_number->setText("Ошибка: число не введено");
        return;
    }

    try {
        std::string inputStd = numberStr.toStdString();
        BigFraction fraction = BaseParser::parse(inputStd, p);
        std::pair<std::string, std::string> convResult = BaseConverter::convert(fraction, q);
        std::string result = convResult.first;
        std::string message = convResult.second;
        ui->line_ans_number->setText(QString::fromStdString(result));
        if (!message.empty()) {
            ui->line_ans_number->append(QString::fromStdString("\n" + message));
        }
    } catch (const std::exception& e) {
        ui->line_ans_number->setText(QString("Ошибка: ") + e.what());
    }
}

void MainWindow::onLoadFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл с данными", "",
                                                    "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }

    QTextStream in(&file);
    QString pStr = in.readLine().trimmed();
    QString qStr = in.readLine().trimmed();
    QString inputStr = in.readLine().trimmed();
    file.close();

    if (pStr.isEmpty() || qStr.isEmpty() || inputStr.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Файл должен содержать три непустые строки: p, q, число.");
        return;
    }

    ui->line_p->setText(pStr);
    ui->line_q->setText(qStr);
    ui->line_my_number->setText(inputStr);

    onCalculate();
}

void MainWindow::onSaveFile() {
    if (ui->line_ans_number->toPlainText().isEmpty()) {
        QMessageBox::information(this, "Нет данных", "Нечего сохранять. Сначала выполните конвертацию.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить результат", "",
                                                    "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать файл.");
        return;
    }

    QTextStream out(&file);
    out << ui->line_ans_number->toPlainText();
    file.close();

    QMessageBox::information(this, "Успех", "Результат сохранён.");
}
