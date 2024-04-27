#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    thread = nullptr;
    manager = new QNetworkAccessManager();
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(managerFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete manager;
    if (thread)
    {
        thread->terminate();
        delete thread;
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open text file"), "", tr("Text Files (*.txt)"));
    ui->label->setText(fileName);
    ui->listWidget->clear();
    if (thread)
    {
        thread->terminate();
        delete thread;
    }
    thread = new CalculationThread(fileName, ui->checkBox->isChecked());
    thread->start();
    thread->wait();
    for (auto& item : thread->getFrMap())
    {
        ui->listWidget->addItem(QString::fromStdString(item.first) + " - " + QString::fromStdString(std::to_string(item.second)));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString urlString = ui->lineEdit->text();
    ui->label->setText(urlString);
    ui->listWidget->clear();
    request.setUrl(QUrl(urlString));
    manager->get(request);
}

void MainWindow::managerFinished(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }
    QString data = reply->readAll();
    QFile file("networkdata.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out.setGenerateByteOrderMark(false);
    out << data;
    file.close();
    if (thread)
    {
        thread->terminate();
        delete thread;
    }
    thread = new CalculationThread("networkdata.txt", ui->checkBox->isChecked());
    thread->start();
    thread->wait();
    for (auto& item : thread->getFrMap())
    {
        ui->listWidget->addItem(QString::fromStdString(item.first) + " - " + QString::fromStdString(std::to_string(item.second)));
    }
}
