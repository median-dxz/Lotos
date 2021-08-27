#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    loadQStyleSheet(":/res/styles/index.qss");

    QList<PageButton *> PageButtons = ui->centralwidget->findChildren<PageButton *>();
    for (PageButton *pageButton : qAsConst(PageButtons)) {
        int index = pageButton->objectName().rightRef(1).toInt();
        pageButton->setIndex(index);
        connect(this, &MainWindow::widgetPageChanged, pageButton, &PageButton::setCurrentChosen);

        connect(pageButton, &PageButton::indexChanged, this, [=](int current_index) {
            emit widgetPageChanged(current_index);
            ui->stackedWidget->setCurrentIndex(current_index - 1);
        });
    }

    test();
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

void HttpAccessTest(MainWindow *p);

void MainWindow::test() {
    qDebug() << QSslSocket::supportsSsl();

    QStackedWidget *stackedWidget = ui->centralwidget->findChild<QStackedWidget *>();
    for (int i = 1; i <= 4; i++) {
        QPushButton *PageButton =
            ui->centralwidget->findChild<QPushButton *>(QString("pageButton_") + QString::number(i));
        connect(PageButton, &QPushButton::clicked, stackedWidget, [=]() { stackedWidget->setCurrentIndex(i - 1); });
    }
    connect(ui->uploadButton, &QPushButton::clicked, this, [=]() { QFileDialog::getOpenFileName(this, "选择图片"); });

    ImgButton *btn = new ImgButton(":/res/icons/page1.png", ":/res/icons/page2.png", ":/res/icons/page1_ig.png");
    btn->setParent(this);
    btn->move(0, 300);

    //    HttpAccessTest(this);
}

void MainWindow::HttpAccessTest(MainWindow *p) {
    // post
    HttpClient *c = new HttpClient();
    c->setUrl("https://sm.ms/api/v2/token");
    connect(c, &HttpClient::responseFinished, p, [=](Response *r) {
        qDebug() << r->getText();
        delete r;
    });
    QByteArray postData("username=median-dxz&password=mDJ37!R74jW6Xez");
    c->post(&postData);

    // get
    c = new HttpClient();
    connect(c, &HttpClient::responseFinished, p, [=](Response *r) {
        qDebug() << r->getText();
        delete r;
    });
    c->get(QUrl("https://sm.ms/api/v2/history"));

    c = new HttpClient();
    QMap<QString, QVariant> auth;
    auth["Authorization"] = QVariant("e4DKi2WtUWNJJlkFFHxH34BP7m3LV17Q");
    c->setHeaders(auth);
    c->setUrl("https://sm.ms/api/v2/upload");
    QFile *file = new QFile("E:\\Resource\\Backgrounds\\09.jpg");

    file->open(QIODevice::ReadOnly);
    postData = file->readAll();

    connect(c, &HttpClient::uplpodProgress, p, [=](qint64 a, qint64 b) { qDebug() << a << b; });
    connect(c, &HttpClient::responseFinished, p, [=](Response *r) {
        qDebug() << r->getText();
        delete r;
    });
    c->uploadFile(&postData, "smfile", "09.jpg");
}
