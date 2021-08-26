 #include "mainwindow.h"
#include "pagebutton.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QString>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    for(int j = 0 ,x = 50,y = 50; j <= 3;j++){
    PageB[j].Pageb_1.setParent(this);
    PageB[j].Pageb_1.move(x, y);
    PageB[j].Setindex(j);
    x += 50; y += 50;
    }


    qDebug() << QString("Loading: Load style file %1").arg(LoadQStyleSheet(":/res/styles/index.qss"));
    QStackedWidget *stackedWidget = ui->centralwidget->findChild<QStackedWidget *>();
    for (int i = 1; i <= 4; i++) {
            QPushButton *PageButton = ui->centralwidget->findChild<QPushButton *>(QString("pageButton_") + QString::number(i));
            connect(PageButton, &QPushButton::clicked, stackedWidget, [=]() { stackedWidget->setCurrentIndex(i - 1); });
        }
    for(int i =0; i<=3; i++){
    connect(PageB + i,SIGNAL(mysignal(int ,bool)),this,SLOT(dealSlot(int,bool)));
    }



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

void MainWindow::dealSlot(int index,bool Chosen){
    if(Chosen == false){
        for(int t = 0; t < 4; t++ ){
            PageB[t].ChangeFalseChosen();
        }
        PageB[index].ChangeChosen();
        for(int t = 0; t < 4;t++)
        PageB[t].ChangeText();
    }


}
