#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    LoadQStyleSheet(":/res/styles/index.qss");

    qDebug() << QSslSocket::supportsSsl();
    QStackedWidget *stackedWidget = ui->centralwidget->findChild<QStackedWidget *>();
    for (int i = 1; i <= 4; i++) {
        QPushButton *PageButton = ui->centralwidget->findChild<QPushButton *>(QString("pageButton_") + QString::number(i));
        connect(PageButton, &QPushButton::clicked, stackedWidget, [=]() { stackedWidget->setCurrentIndex(i - 1); });
    }
    connect(ui->uploadButton, &QPushButton::clicked, this, [=]() { QFileDialog::getOpenFileName(this, "选择图片"); });

    ImgButton *btn = new ImgButton(":/res/icons/page1.png", ":/res/icons/page2.png", ":/res/icons/page1_ig.png");
    btn->setParent(this);
    btn->move(0, 300);
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
