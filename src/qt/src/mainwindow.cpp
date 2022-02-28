#include <stdexcept>

#include <parser/parser.cpp>
#include <functions/functions.cpp>

#include "../include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calcderb_clicked()
{
    try {
        std::string text(ui->formtext->toPlainText().toStdString());
        simpleparser::Parser pr(text);
        Func* f = pr.Parse();
        Func* d = f->Der();
        std::string drep(d->repr());
        ui->derfl->setText(QString::fromStdString(drep));
    }
    catch (std::runtime_error e) {
        ui->derfl->setText(QString::fromStdString(e.what()));
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->formtext->setPlainText(ui->derfl->text());
}

