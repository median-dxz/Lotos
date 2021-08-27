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
    btn->move(0, 400);


    QLabel * label1 = new QLabel;
    label1->setParent(this);
    label1->setPixmap(QPixmap(":/res/icons/page1.png"));
    label1->setScaledContents(true);
    label1->resize(QSize(30,30));
    label1->move(5,48);
    label1->setAttribute(Qt::WA_TransparentForMouseEvents);

    QLabel * label2 = new QLabel;
    label2->setParent(this);
    label2->setPixmap(QPixmap(":/res/icons/page2.png"));
    label2->setScaledContents(true);
    label2->resize(QSize(30,30));
    label2->move(5,127);
    label2->setAttribute(Qt::WA_TransparentForMouseEvents);


    QLabel * label3 = new QLabel;
    label3->setParent(this);
    label3->setPixmap(QPixmap(":/res/icons/page2.png"));
    label3->setScaledContents(true);
    label3->resize(QSize(30,30));
    label3->move(5,207);
    label3->setAttribute(Qt::WA_TransparentForMouseEvents);

    QLabel * label4 = new QLabel;
    label4->setParent(this);
    label4->setPixmap(QPixmap(":/res/icons/page2.png"));
    label4->setScaledContents(true);
    label4->resize(QSize(30,30));
    label4->move(5,287);
    label4->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->pageButton_1->setStyleSheet("text-align:left");
    ui->pageButton_2->setStyleSheet("text-align:left");
    ui->pageButton_3->setStyleSheet("text-align:left");
    ui->pageButton_4->setStyleSheet("text-align:left");

}

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::loadQStyleSheet(const QString &fileName) {
    QFile qssFile(fileName);
    if (qssFile.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(qssFile.readAll());
        qssFile.close();
        return true;
    } else {
        return false;
    }
}
