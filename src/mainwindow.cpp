#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    init();
    componentsLayoutManager();
    interfaceStyleManager();

    test();
}

void MainWindow::test() {
    qDebug() << QSslSocket::supportsSsl();
    connect(ui->t1, &QPushButton::clicked, this, [=] {
        notify->newNotify(
            ui->viewport, "提示提示",
            "提示啊啊啊啊啊啊\n"
            "22:01:34: Starting "
            "E:\\Project\\qt_curriculum_design\\build_lotos_Desktop_Qt_5_12_11_MinGW_64_bit_Debug\\debug\\lotos.exe",
            ("#fff"));
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onMainProcessClosed() {
    //    globalSettings.save();
}

void MainWindow::interfaceStyleManager() {
    loadQStyleSheet(":/res/styles/index.qss");

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
    notify = &NotificationManager::Instance();
    globalSettings.load(PATH_CONFIG);
}

void MainWindow::componentsLayoutManager() {
    connect(ui->titleBar, &TitleBar::onMiniBtnClicked, this, &MainWindow::showMinimized);
    connect(ui->titleBar, &TitleBar::onCloseBtnClicked, this, &MainWindow::close);
    connect(this, &MainWindow::destroyed, this, &MainWindow::onMainProcessClosed);

    // set imghost dashbroad page
    ui->pageSwitchWidget->installEventFilter(this);
    connect(ui->hostLogin, &QPushButton::clicked, this, &MainWindow::onHostLoginClicked);
    connect(ui->hostReset, &QPushButton::clicked, this, &MainWindow::onHostResetClicked);

    // set pagebutton toggle signal&icon
    QList<PageButton *> PageButtons = ui->centralwidget->findChildren<PageButton *>();
    QList<QString> iconPaths;

    iconPaths.append(":/res/icons/page_1.png");
    iconPaths.append(":/res/icons/page_1_ig.png");
    iconPaths.append(":/res/icons/page_2.png");
    iconPaths.append(":/res/icons/page_2_ig.png");
    iconPaths.append(":/res/icons/page_3.png");
    iconPaths.append(":/res/icons/page_3_ig.png");
    iconPaths.append(":/res/icons/page_4.png");
    iconPaths.append(":/res/icons/page_4_ig.png");

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

        pageButton->setIconPath(iconPaths[index * 2 - 2], iconPaths[index * 2 - 1]);
        pageButton->setIconSize(QSize(28, 28));
        pageButton->setIcon(QIcon(iconPaths[index * 2 - 2]));
        pageButton->installEventFilter(this);
    }
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
    if (obj->inherits("PageButton") && event->type() == QEvent::HoverEnter) {
        PageButton *page_btn = (PageButton *)obj;
        page_btn->drawIcon(true);
    }
    if (obj->inherits("PageButton") && event->type() == QEvent::HoverLeave) {
        PageButton *page_btn = (PageButton *)obj;
        page_btn->drawIcon(false);
    }
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
    if (qssFile.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(qssFile.readAll());
        qssFile.close();
        return true;
    } else {
        return false;
    }
}
