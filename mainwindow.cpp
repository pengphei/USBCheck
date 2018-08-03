#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DiskCheck.h"
#include <QLineEdit>
#include <QTextCharFormat>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    check = DiskCheck::getInstance();
    statusLabel = new QLabel(statusBar());
    statusBar()->addWidget(statusLabel);
    connect(check, SIGNAL(notifyGenerate(int,QString)), this, SLOT(onNotifyGenerate(int,QString)));
    connect(check, SIGNAL(notifyCheck(int,QString,int)), this, SLOT(onNotifyCheck(int,QString,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_genButton_clicked()
{
    QString udiskPath = ui->udiskLabel->text();
    QString md5Path = ui->md5Label->text();

    check->startGenerate(udiskPath, md5Path);
}

void MainWindow::onNotifyCheck(int flag, const QString &tag, int percent)
{
    switch (flag)
    {
    case DISK_CHECK_INFO:
        statusLabel->setPalette(QPalette(Qt::gray));
        break;
    case DISK_CHECK_ERR0R:
        statusLabel->setPalette(QPalette(Qt::red));
        break;
    case DISK_CHECK_FINI:
        statusLabel->setPalette(QPalette(Qt::green));
        break;
    default:
        break;
    }
    statusLabel->setText(tag);
    qDebug() << tag<< "\r\n";
}

void MainWindow::onNotifyGenerate(int flag, const QString &tag)
{
    switch (flag)
    {
    case DISK_GENE_INFO:
        statusLabel->setPalette(QPalette(Qt::gray));
        break;
    case DISK_GENE_ERR0R:
        statusLabel->setPalette(QPalette(Qt::red));
        break;
    case DISK_GENE_FINI:
        statusLabel->setPalette(QPalette(Qt::green));
        break;
    default:
        break;
    }
    statusLabel->setText(tag);
    qDebug() << tag<< "\r\n";
}

void MainWindow::on_md5SelectButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory();
    ui->md5Label->setText(path+"/md5.txt");
}

void MainWindow::on_udiskSelectButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory();
    ui->udiskLabel->setText(path);
}

void MainWindow::on_chkButton_clicked()
{
    QString md5Path = ui->md5Label->text();
}
