#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    //隐藏自带标题栏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    //qDebug()<<QSslSocket:: sslLibraryBuildVersionString();
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
    minibutton=new QPushButton(this);
    closebutton=new QPushButton(this);
    minibutton->setGeometry(wide-35,5,32,32);
    closebutton->setGeometry(wide-67,5,32,32);
    minibutton->installEventFilter(this);
    closebutton->installEventFilter(this);
    minibutton->setToolTip(tr("最小化"));
    closebutton->setToolTip(tr("关闭"));
    connect(minibutton,&QPushButton::clicked,this,&MainWindow::showMinimized);
    connect(closebutton,&QPushButton::clicked,this,&MainWindow::close);
    minibutton->setStyleSheet("background-color: rgba(64,158,255,0.1)");
    closebutton->setStyleSheet("background-color: rgba(245,108,108,0.1)");




    test();
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
void MainWindow::mouseReleaseEvent(QMouseEvent *){
    mouse_press = false;
}
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    //移动窗口
    if(mouse_press){
        QPoint move_pos = event->globalPos();
        move(move_pos-move_point);
    }
}
bool MainWindow::eventFilter(QObject *watched, QEvent *event){
    if( watched == minibutton &&event->type()==QEvent::Paint){
        drawminibutton();
    }
    else if( watched == closebutton &&event->type()==QEvent::Paint){
        drawclosebutton();
    }
    else if(watched == minibutton && event->type()==QEvent::Enter){
        changeminibutton(1);
    }
    else if(watched == minibutton && event->type()==QEvent::Leave){
        changeminibutton(2);
    }
    else if(watched == closebutton && event->type()==QEvent::Enter){
       changeclosebutton(1);
    }
    else if(watched == closebutton && event->type()==QEvent::Leave){
       changeclosebutton(2);
    }
    return QWidget::eventFilter(watched,event);
}
void MainWindow::drawminibutton(){
    QPen pen;
    pen.setColor("background-color: #909399");
    pen.setWidth(1.5);
    QPainter painter(minibutton);
    painter.setPen(pen);
    painter.drawLine(4,16,28,16); //画直线
}
void MainWindow::drawclosebutton(){
    QPen pen;
    pen.setColor("background-color: #909399");
    pen.setWidth(1.5);
    QPainter painter2(closebutton);
    painter2.setPen(pen);
    painter2.drawLine(4,4,28,28);//画关闭符号
    painter2.drawLine(28,4,4,28);
}
void MainWindow::changeminibutton(int a){
   if(a==1) minibutton->setStyleSheet("background-color: rgba(64,158,255,1)");
   if(a==2) minibutton->setStyleSheet("background-color: rgba(64,158,255,0.1)");
}
void MainWindow::changeclosebutton(int a){
    if(a==1)closebutton->setStyleSheet("background-color: rgba(245,108,108,1)");
    if(a==2)closebutton->setStyleSheet("background-color: rgba(245,108,108,0.1)");
}
