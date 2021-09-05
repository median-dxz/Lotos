#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    loadQStyleSheet(":/res/styles/index.qss");

    setAttribute(Qt::WA_QuitOnClose);

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
    int wide=this->width();
    QPushButton *MinButton=new QPushButton(this);
    QPushButton *CloseButton=new QPushButton(this);
    MinButton->setText("1");
    CloseButton->setText("2");
    MinButton->setGeometry(wide-65,5,20,20);
    CloseButton->setGeometry(wide-25,5,20,20);
    //鼠标移动至按钮上提示信息
    MinButton->setToolTip(tr("最小化"));
    CloseButton->setToolTip(tr("关闭"));
    connect(MinButton,&QPushButton::clicked,this,&MainWindow::minwindow);
    connect(CloseButton,&QPushButton::clicked,this,&MainWindow::closewindow);


    test();
}

void MainWindow::minwindow(){
    this->showMinimized();
}
void MainWindow::closewindow(){
    this->onmainwindowclosed();
    this->close();

}
void MainWindow::onmainwindowclosed(){
   qDebug()<<"test";
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

    //    ImgButton *btn = new ImgButton(":/res/icons/page1.png", ":/res/icons/page2.png", ":/res/icons/page1_ig.png");
    //    btn->setParent(this);
    //    btn->move(0, 300);
    QLabel *label1 = new QLabel;
    label1->setParent(this);
    label1->setPixmap(QPixmap(":/res/icons/page1.png"));
    label1->setScaledContents(true);
    label1->resize(QSize(30, 30));
    label1->move(5, 48);
    label1->setAttribute(Qt::WA_TransparentForMouseEvents);

    QLabel *label2 = new QLabel;
    label2->setParent(this);
    label2->setPixmap(QPixmap(":/res/icons/page2.png"));
    label2->setScaledContents(true);
    label2->resize(QSize(30, 30));
    label2->move(5, 127);
    label2->setAttribute(Qt::WA_TransparentForMouseEvents);

    QLabel *label3 = new QLabel;
    label3->setParent(this);
    label3->setPixmap(QPixmap(":/res/icons/page2.png"));
    label3->setScaledContents(true);
    label3->resize(QSize(30, 30));
    label3->move(5, 207);
    label3->setAttribute(Qt::WA_TransparentForMouseEvents);

    QLabel *label4 = new QLabel;
    label4->setParent(this);
    label4->setPixmap(QPixmap(":/res/icons/page2.png"));
    label4->setScaledContents(true);
    label4->resize(QSize(30, 30));
    label4->move(5, 287);
    label4->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->pageButton_1->setStyleSheet("text-align:left");
    ui->pageButton_2->setStyleSheet("text-align:left");
    ui->pageButton_3->setStyleSheet("text-align:left");
    ui->pageButton_4->setStyleSheet("text-align:left");
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
void MainWindow::mousePressEvent(QMouseEvent *event){
    //只实现了左键移动
    if(event->button() == Qt::LeftButton)
        mouse_press = true;
    move_point=event->globalPos() - this->pos();
    qDebug()<<"pos()"<<this->pos().x()<<" "<<this->pos().y();
    qDebug()<<"globalPos"<<event->globalPos().x()<<" "<<event->globalPos().y();
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    mouse_press = false;
}
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    //移动窗口
    if(mouse_press){
        QPoint move_pos = event->globalPos();
        move(move_pos-move_point);
    }
}
