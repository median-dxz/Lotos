#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
<<<<<<< HEAD
<<<<<<< HEAD

    LoadQStyleSheet(":/res/styles/index.qss");
=======
    loadQStyleSheet(":/res/styles/index.qss");
    setWindowTitle(tr("Lotos"));
    componentsLayoutManager();
>>>>>>> 614284c (pref: 优化图标绘制，整理主窗口代码)
=======
    setWindowTitle(tr("Lotos"));
    qApp->setFont(QFont("Microsoft YaHei UI"));
    init();
>>>>>>> 79dc943 (feat: 添加api接口定义)

    test();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onHostLoginClicked() {
    HttpClient *client = smms->getAPIToken(ui->hostUsername->text(), ui->hostPassword->text());
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
            smms->setToken(token);
            globalSettings.imghost.insert("token", token);
            ui->tokenLabel->setText(token);
            ui->userInfoLabel->setText(tr("登录成功，正在获取用户信息..."));
            ui->hostInfoPage->setCursor(QCursor(Qt::WaitCursor));
        }
        el->quit();
        this->setCursor(QCursor(Qt::ArrowCursor));
        delete res;
    });
    el->exec();

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
        ui->hostInfoPage->setCursor(QCursor(Qt::ArrowCursor));
        delete res;
    });
}

void MainWindow::onHostResetClicked() {
    globalSettings.imghost.insert("token", "");
    globalSettings.imghost.insert("isAuthorized", false);
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

void MainWindow::init() {
    proxy = HttpClient::getNetworkProxyInstanse();
    smms = &SMMS::getInstance();

    globalSettings.load("/config.json");

    loadQStyleSheet(":/res/styles/index.qss");

    componentsLayoutManager();
}

void MainWindow::componentsLayoutManager() {
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
        connect(this, &MainWindow::widgetPageChanged, pageButton, &PageButton::setCurrentChosen);
        connect(pageButton, &PageButton::indexChanged, this, [=](int current_index) {
            emit widgetPageChanged(current_index);
            ui->stackedWidget->setCurrentIndex(current_index - 1);
        });

        pageButton->setIconPath(iconPaths[index * 2 - 2], iconPaths[index * 2 - 1]);
        pageButton->setIconSize(QSize(28, 28));
        pageButton->setIcon(QIcon(iconPaths[index * 2 - 2]));
        pageButton->installEventFilter(this);
    }

    // set imghost setting page
    ui->pageSwitchWidget->installEventFilter(this);
    if (globalSettings.imghost["isAuthorized"].toBool()) {
    } else {
        ui->tokenLabel->setText(tr("未登录"));
        ui->hostUserDiskLimit->hide();
        ui->userInfoLabel->setText(QString(tr("<div><b>用户组:</b> 未登录用户</div>")));
    }

    connect(ui->hostLogin, &QPushButton::clicked, this, &MainWindow::onHostLoginClicked);
    connect(ui->hostReset, &QPushButton::clicked, this, &MainWindow::onHostResetClicked);
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

void MainWindow::test() {
    qDebug() << QSslSocket::supportsSsl();
}
