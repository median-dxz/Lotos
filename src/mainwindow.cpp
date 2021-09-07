#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    loadQStyleSheet(":/res/styles/index.qss");
    setWindowTitle(tr("Lotos"));
    componentsLayoutManager();

    connect(ui->uploadButton, &QPushButton::clicked, this, [=]() {
    test1();  });
    test();


    QTimer * time =new QTimer;
    time->start(20);
    connect(time,&QTimer::timeout,this,[=](){
        if(iconwidget:: dele)
        {
            delpix();
            iconwidget:: dele = 0;
        }
    });

}
int MainWindow::j=5;
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

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj->inherits("PageButton") && event->type() == QEvent::HoverEnter) {
        PageButton *page_btn = (PageButton *)obj;
        page_btn->DrawPix(page_btn->getignitedIconPath());
        return true;
    }
    if (obj->inherits("PageButton") && event->type() == QEvent::HoverLeave) {
        PageButton *page_btn = (PageButton *)obj;
        page_btn->DrawPix(page_btn->getnormalIconPath());
        return  true;
    }
    return QWidget::eventFilter(obj, event);





}


void MainWindow::componentsLayoutManager() {
    // set pagebutton toggle signal&icon
    QList<PageButton *> PageButtons = ui->centralwidget->findChildren<PageButton *>();
//    QList<QString> iconPaths;


//    iconPaths.append(":/res/icons/page_1.png");
//    iconPaths.append(":/res/icons/page_1_ig.png");
//    iconPaths.append(":/res/icons/page_2.png");
//    iconPaths.append(":/res/icons/page_2_ig.png");
//    iconPaths.append(":/res/icons/page_3.png");
//    iconPaths.append(":/res/icons/page_3_ig.png");
//    iconPaths.append(":/res/icons/page_4.png");
//    iconPaths.append(":/res/icons/page_4_ig.png");

    for (PageButton *pageButton : qAsConst(PageButtons)) {
        int index = pageButton->objectName().rightRef(1).toInt();

        pageButton->setIndex(index);
        connect(this, &MainWindow::widgetPageChanged, pageButton, &PageButton::setCurrentChosen);
        connect(pageButton, &PageButton::indexChanged, this, [=](int current_index) {
            emit widgetPageChanged(current_index);
            ui->stackedWidget->setCurrentIndex(current_index - 1);
        });

        pageButton->setIconPath(iconPaths[index * 2 - 2], iconPaths[index * 2 - 1]);
//        pageButton->setIconSize(QSize(32, 32));
//        QIcon icon = QIcon(iconPaths[index * 2 - 2]);
//        pageButton->setIcon(QIcon(iconPaths[index * 2 - 2]));
        pageButton->DrawPix(iconPaths[index * 2 - 2]);
        pageButton->installEventFilter(this);
        pageButton->setStyleSheet("text-align:left");

//          QLabel *label = new QLabel(pageButton);
//          label->installEventFilter(this);
//          label->setPixmap(iconPaths[index*2-2]);
//          label->setScaledContents(true);
//          label->resize(QSize(35, 35));
//          label->move(5,25);
//          //label->setAttribute(Qt::WA_TransparentForMouseEvents);


    }

}

void MainWindow::test() {
    qDebug()<< QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QSslSocket::supportsSsl();


//    connect(ui->uploadButton, &QPushButton::clicked, this, [=]() { QFileDialog::getOpenFileName(this, "选择图片"); });

    //    HttpAccessTest(this);
}

void MainWindow::httpAccessTest(MainWindow *p) {
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

void MainWindow ::test1()
{

    iconwidget * l = new iconwidget(ui->dragBox);
    //l->setshadow();
    l->pixpath = QFileDialog::getOpenFileName(this, "选择图片");
    if(checkpix(l))
    {
        QPixmap pix(l->pixpath);
        l->setGeometry(j,6,200*pix.width()/pix.height() ,230);

        QFileInfo info(l-> pixpath);
        l->name = info.fileName();
        l->size = info.size();
        l->setdelbtn();
        l->show();
        j+=(200*pix.width()/pix.height())+5;
   }



}

void MainWindow ::delpix()
{
    j=5;
    QList<iconwidget *> widgets = ui->dragBox->findChildren<iconwidget *>();
    for(int i= 0;i<widgets.count(); i+=3    )
    {
    //int i = 0;
      qDebug()<<widgets[i]->name;
      QPixmap pix(widgets[i]->pixpath);
      widgets[i]->setGeometry(j,6,200*pix.width()/pix.height() ,230);
      j+=(200*pix.width()/pix.height() )+5;
    }
    //qDebug()<<widgets.at(6)->name;
    //qDebug()<<widgets.count();

}

int MainWindow ::checkpix(iconwidget *l)
{
    if(l->pixpath==nullptr)
    {
        delete  l;
        return 0;
    }
    QPixmap pix;
    int j = pix.load(l->pixpath);
    if(j)
        return j;
    else
    {
        delete  l;
        return 0;
    }


}


