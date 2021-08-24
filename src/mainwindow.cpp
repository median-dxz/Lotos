#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QPainter>
#include <pushbuttonfunc1.h>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    qDebug() << QString("Loading: Load style file %1").arg(LoadQStyleSheet(":/res/styles/index.qss"));

    QStackedWidget *stackedWidget = ui->centralwidget->findChild<QStackedWidget *>();
    for (int i = 1; i <= 4; i++) {
        QPushButton *PageButton = ui->centralwidget->findChild<QPushButton *>(QString("pageButton_") + QString::number(i));
        connect(PageButton, &QPushButton::clicked, stackedWidget, [=]() { stackedWidget->setCurrentIndex(i - 1); });

    }

//    connect(ui->uploadButton , &QPushButton::clicked,[=](){
//        QString path = QFileDialog::getOpenFileName(this , "选择图片" , "C:\\Users\\lenovo\\Pictures");
//        QPainter painter(ui->dragBox);
//        painter.drawPixmap(20,0,QPixmap(path));
//    });

    Pushbuttonfunc1 * btn =new Pushbuttonfunc1(":/res/icons/page1.png",":/res/icons/page2.png",":/res/icons/page1_ig.png");
    btn->setParent(this);
    btn->move(0,300);



}

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::LoadQStyleSheet(const QString &fileName) {
    QFile qssFile(fileName);
    if (qssFile.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(qssFile.readAll());
        qssFile.close();
        return true;
    } else {
        return false;
    }
}
