#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    thread = new Thread();
    M = new PaintWindow(this);
    connect(M->Hide , &QPushButton::clicked,this,[=](){  M->hide();this->show();});
    connect(thread, SIGNAL(returnResult(int,QString ,QString)), this, SLOT(displayResult(int,QString ,QString)));
    connect(thread, &Thread::deleLoading , this, [=](){
//        M->hide();
//        this->show();
        M->show();//提示子线程完成操作
        this->hide();
        thread->wait();//thread->setCallback(this,M,cal);thread->start();写到需要打开子线程办事的信号槽里
    });
    connect(ui->pushButton_7 , &QPushButton::clicked , this , [=](){
        qDebug()<<"子线程正在帮您后台运行";
        thread->setCallback(this,M,thing);
        thread->start();
//        M->show();
//        this->hide();
        Painting(2,"这个现在没用" ,"用的是thing里的");//这个是切换动画，将参数改一下可以根据返回值来确定

    });

    init();
    componentsLayoutManager();
    interfaceStyleManager();

    test();
}

void MainWindow::test() {
    qDebug() << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QSslSocket::supportsSsl();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::displayResult(int result , QString text , QString tit)
{
    M->tit=tit;
    M->text=text;
    qDebug()<<"子线程返回值";
}

void MainWindow::onMainProcessClosed() {
    globalSettings.save();
    close();
}

void MainWindow::interfaceStyleManager() {
    loadQStyleSheet(":/res/styles/index.css");
    loadQStyleSheet(":/res/styles/uploadpage.css");
    loadQStyleSheet(":/res/styles/hostdashboard.css");

    setWindowTitle(tr("Lotos"));
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    qApp->setFont(QFont("Microsoft YaHei UI"));

    QGraphicsDropShadowEffect *box_shadow = new QGraphicsDropShadowEffect(this);
    box_shadow->setBlurRadius(12);
    box_shadow->setOffset(0, 1);
    box_shadow->setColor(QColor(0, 0, 0, 255 * 0.18));
    ui->viewport->setGraphicsEffect(box_shadow);

    ui->titleBar->setIcon(QPixmap(":/res/lotos_icon.png"));
    ui->titleBar->setTitle(tr("Lotos"));
}

void MainWindow::init() {
    proxy = HttpClient::getNetworkProxyInstanse();
    smms = &SMMS::getInstance();
    globalSettings.load(PATH_CONFIG);
}

void MainWindow::componentsLayoutManager() {
    connect(ui->titleBar, &TitleBar::onMiniBtnClicked, this, &MainWindow::showMinimized);
    connect(ui->titleBar, &TitleBar::onCloseBtnClicked, this, &MainWindow::onMainProcessClosed);

    // set imghost upload page
    QGridLayout *gridlayout = new QGridLayout;
    gridlayout->setMargin(8);
    gridlayout->setHorizontalSpacing(8);
    gridlayout->setVerticalSpacing(8);
    gridlayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->dragBoxContents->setLayout(gridlayout);

    connect(ui->uploadButton, &QPushButton::clicked, this, &MainWindow::onUploadButtonClicked);

    // set imghost dashbroad page
    ui->pageSwitchWidget->installEventFilter(this);
    connect(ui->hostLogin, &QPushButton::clicked, this, &MainWindow::onHostLoginClicked);
    connect(ui->hostReset, &QPushButton::clicked, this, &MainWindow::onHostResetClicked);

    // set pagebutton toggle signal&icon
    QList<PageButton *> PageButtons = ui->centralwidget->findChildren<PageButton *>();

    for (PageButton *pageButton : qAsConst(PageButtons)) {
        int index = pageButton->objectName().rightRef(1).toInt();

        pageButton->setIndex(index);
        loadPage(PAGE(index - 1));
        connect(this, &MainWindow::widgetPageChanged, pageButton, &PageButton::setCurrentChosen);
        connect(pageButton, &PageButton::indexChanged, this, [=](int current_index) {
            emit widgetPageChanged(current_index);
            ui->stackedWidget->setCurrentIndex(current_index - 1);
            loadPage(PAGE(current_index));
        });

        pageButton->setIconPath(iconPaths.at(index * 2 - 2), iconPaths.at(index * 2 - 1));
        pageButton->setIconSize(QSize(28, 28));
        pageButton->setIconOffset(18, pageButton->height() / 2 - 10);
        pageButton->drawPix(iconPaths[index * 2 - 2]);
    }

   // PageButtons.at(0)->setCurrentChosen(1);
    ui->stackedWidget->setCurrentIndex(UploadPage);
}

void MainWindow::loadPage(MainWindow::PAGE index) {
    switch (index) {
        case HostDashBoardPage:
            ui->hostUsername->clear();
            ui->hostPassword->clear();
            if (globalSettings.imghost["isAuthorized"].toBool()) {
                ui->hostLogin->click();
            } else {
                ui->tokenLabel->setText(tr("未登录"));
                ui->hostUserDiskLimit->hide();
                ui->userInfoLabel->setText(QString(tr("<div><b>用户组:</b> 未登录用户</div>")));
            }
            break;
        default:
            break;
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj->objectName() == "pageSwitchWidget" && event->type() == QEvent::Paint) {
        QWidget *page_widget = (QWidget *)obj;
        QPainter painter(page_widget);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(QColor(220, 223, 230), 2));
        painter.drawLine(page_widget->size().width(), page_widget->size().height() * 0.04, page_widget->size().width(),
                         page_widget->size().height() * 0.96);
    }
    return QWidget::eventFilter(obj, event);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        movingPoint = event->globalPos() - this->pos();
        mousePressed = true;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *) {
    mousePressed = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    //移动窗口
    if (mousePressed) {
        QPoint move_pos = event->globalPos();
        move(move_pos - movingPoint);
    }
}

void MainWindow::onHostLoginClicked() {
    HttpClient *client = nullptr;
    if (!globalSettings.imghost["isAuthorized"].toBool()) {
        client = smms->getAPIToken(ui->hostUsername->text(), ui->hostPassword->text());
        this->setCursor(QCursor(Qt::WaitCursor));
        QEventLoop *el = new QEventLoop(this);
        connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *res) {
            SMMS::Response data;
            SMMS::praseResponse(res->getJson(), data);

            if (res->statusCode.toUInt() != 200) {
                ui->userInfoLabel->setText(tr("网络错误!"));
            } else if (data.success == false) {
                ui->userInfoLabel->setText(tr("<h4>登录失败 </h4>") + data.message);
            } else {
                QString token = data.data.at(0).toObject()["token"].toString();
                globalSettings.imghost.insert("token", token);
                globalSettings.imghost.insert("isAuthorized", true);
                globalSettings.save();
            }
            el->quit();
            this->setCursor(QCursor(Qt::ArrowCursor));
            delete res;
        });
        el->exec();
    }

    if (globalSettings.imghost["isAuthorized"].toBool()) {
        smms->setToken(globalSettings.imghost["token"].toString());

        ui->hostLogin->setEnabled(false);
        ui->hostLogin->setText(tr("已登录"));
        ui->tokenLabel->setText(smms->token());
        ui->userInfoLabel->setText(tr("正在获取用户信息..."));
        ui->hostDashBoardPage->setCursor(QCursor(Qt::WaitCursor));

        client = smms->getUserProfile();
        connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *res) {
            SMMS::Response data;
            SMMS::UserProfile profile;
            SMMS::praseResponse(res->getJson(), data);
            SMMS::praseUserProfile(data.data.at(0).toObject(), profile);

            if (res->statusCode.toUInt() != 200) {
                ui->userInfoLabel->setText(tr("网络错误!"));
            } else if (data.success == false) {
                ui->userInfoLabel->setText(tr("<h4>获取用户信息失败 </h4>") + data.message);
            } else {
                ui->userInfoLabel->setText(QString(tr("<div><b>用户名:</b> %1</div>"
                                                      "<div><b>用户组:</b> %2</div>"
                                                      "<div><b>用户组剩余时间:</b> %3</div>"
                                                      "<div><b>邮箱:</b> %4</div>"
                                                      "<div><b>邮箱验证:</b> %5</div>"))
                                               .arg(profile.username, profile.role, profile.group_expire, profile.email,
                                                    profile.email_verified ? tr("已验证") : tr("未验证")));
                ui->hostUserDiskLimit->show();
                ui->hostUserDiskLimit->setValue(profile.disk_usage_raw / profile.disk_limit_raw * 100);
                ui->hostUserDiskLimit->setFormat(profile.disk_usage + "/" + profile.disk_limit);
            }
            ui->hostDashBoardPage->setCursor(QCursor(Qt::ArrowCursor));
            delete res;
        });
    }
}

void MainWindow::onHostResetClicked() {
    globalSettings.imghost.insert("token", "");
    globalSettings.imghost.insert("isAuthorized", false);
    globalSettings.save();

    ui->hostLogin->setEnabled(true);
    ui->hostLogin->setText(tr("登录"));
    ui->hostUsername->clear();
    ui->hostPassword->clear();

    ui->tokenLabel->setText(tr("未登录"));

    ui->hostUserDiskLimit->hide();
    ui->userInfoLabel->setText(QString(tr("<div><b>用户组:</b> 未登录用户</div>")));
}

bool MainWindow::loadQStyleSheet(const QString &fileName) {
    QFile qssFile(fileName);
    QString stylesheet = qApp->styleSheet();
    if (qssFile.open(QFile::ReadOnly)) {
        stylesheet += qssFile.readAll();
        qApp->setStyleSheet(stylesheet);
        qssFile.close();
        return true;
    } else {
        return false;
    }
}

void MainWindow::onUploadButtonClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择图片"));
    QPixmap p;

    if (p.load(fileName)) {
        int cols = (ui->dragBoxContents->width() - 18) / iconWidgetSize.width();
        int count = iconWidgets.count();
        IconWidget *widget = new IconWidget(ui->dragBox);

        QGridLayout *l = ((QGridLayout *)ui->dragBoxContents->layout());
        l->addWidget(widget, count / cols, count % cols);
        widget->setFixedSize(iconWidgetSize);
        widget->setImage(QImage(fileName));

        iconWidgets.append(widget);

        connect(widget, &IconWidget::onDeleteBtnClicked, this, [=](IconWidget *obj) {
            QList<IconWidget *>::iterator del_iter;
            for (QList<IconWidget *>::iterator iter = iconWidgets.begin(); iter != iconWidgets.end(); iter++) {
                if (*iter == obj) {
                    del_iter = iter;
                }
            }
            iconWidgets.erase(del_iter);

            while (!((QGridLayout *)ui->dragBoxContents->layout())->isEmpty()) {
                delete ((QGridLayout *)ui->dragBoxContents->layout())->takeAt(0);
            }

            obj->deleteLater();

            int c = 0;
            int cols = (ui->dragBoxContents->width() - 18) / iconWidgetSize.width();
            for (QList<IconWidget *>::iterator iter = iconWidgets.begin(); iter != iconWidgets.end(); iter++) {
                l->addWidget(*iter, c / cols, c % cols);
                c++;
            }
        });
    } else {
        qDebug() << "ERR: 错误的图片选择";
    }
}

void MainWindow::Painting(int i ,QString text ,QString tit)
{
    M->reSetTime();
    M->pTimer->setInterval(100);
    switch (i) {
    case 0:
        M->Animation=0;
        M->pTimer->start(0.1);
        break;
     case 1:
        M->Animation=1;
        M->pTimer->start(2);
        break;
      case 2:
        M->Animation=2;
        M->pTimer->start(3);
        break;
    }
//    M->text = text;
//    M->tit = tit;
}

int MainWindow::thing(MainWindow *m ,PaintWindow *M)
{
    M->text = "text";//回调文字
    M->tit = "tit";

     //主线程要干的事搬到这干，不影响主线程操作 如：
    int result = 0;
    for(int i = 0; i < 2000000000; i++)
        result += i;
    return result;

}

