#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    init();
    componentsLayoutManager();
    interfaceStyleManager();

    test();
}

void MainWindow::test() {
    qDebug() << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QSslSocket::supportsSsl();

    connect(ui->tb1, &QPushButton::clicked, this, [=] {
        if (globalSettings.imghost[KeyMap.imghost_isAuthorized] == true) {
            HttpClient *client = nullptr;
            client = smms->getUploadHistory(1);
            connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *res) {
                qDebug() << res->getText();
                SMMS::Response data;
                SMMS::praseResponse(res->getJson(), data);
                for (auto obj : qAsConst(data.data)) {
                    SMMS::ImageInfomation info;
                    SMMS::praseImageInfomation(obj.toObject(), info);
                    qDebug() << timestamp2str(info.timestamp, "yyyy/MM/dd hh:mm:ss");
                }

                delete res;
            });
        }
    });
    installEventFilter(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::interfaceStyleManager() {
    loadQStyleSheet(":/res/styles/index.css");
    loadQStyleSheet(":/res/styles/uploadpage.css");
    loadQStyleSheet(":/res/styles/hostdashboard.css");

    setWindowTitle(tr("Lotos"));
    setWindowIcon(QIcon(":/res/lotos_icon.png"));
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    qApp->setFont(QFont("Microsoft YaHei UI"));

    QGraphicsDropShadowEffect *box_shadow = new QGraphicsDropShadowEffect(this);
    box_shadow->setBlurRadius(12);
    box_shadow->setOffset(0, 1);
    box_shadow->setColor(QColor(0, 0, 0, 255 * 0.18));
    ui->viewport->setGraphicsEffect(box_shadow);

    QGraphicsDropShadowEffect *sider_bar_shadow = new QGraphicsDropShadowEffect(this);
    sider_bar_shadow->setBlurRadius(12);
    sider_bar_shadow->setOffset(2, 0);
    sider_bar_shadow->setColor(QColor(0, 0, 0, 255 * 0.18));
    ui->pageSwitchWidget->setGraphicsEffect(sider_bar_shadow);

    QLabel *mainIcon = new QLabel(this);
    QLabel *mainTitle = new QLabel(this);
    mainIcon->setScaledContents(true);
    mainIcon->setFixedSize(QSize(72, 72));
    mainIcon->setPixmap(QPixmap(":/res/lotos_icon.png"));

    mainTitle->setText(tr("Lotos"));
    mainTitle->setFont(QFont("Agency FB", 32));

    QVBoxLayout *sider_layout = dynamic_cast<QVBoxLayout *>(ui->pageSwitchWidget->layout());
    sider_layout->insertWidget(0, mainIcon, 0, Qt::AlignHCenter);
    sider_layout->insertWidget(1, mainTitle, 0, Qt::AlignHCenter);
}

void MainWindow::init() {
    proxy = HttpClient::getNetworkProxyInstanse();
    smms = &SMMS::Instance();
    globalSettings.load(PATH_CONFIG);
    notify = &NotificationManager::Instance();
}

void MainWindow::componentsLayoutManager() {
    connect(ui->titleBar, &TitleBar::onMiniBtnClicked, this, &MainWindow::showMinimized);
    connect(ui->titleBar, &TitleBar::onCloseBtnClicked, this, &MainWindow::onMainProcessClosed);

    // set imghost upload page
    QGridLayout *gridlayout = new QGridLayout;
    gridlayout->setContentsMargins(8, 8, 8, 8);
    gridlayout->setHorizontalSpacing(8);
    gridlayout->setVerticalSpacing(8);
    gridlayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->dragBoxContents->setLayout(gridlayout);

    connect(ui->selectFilesButton, &QPushButton::clicked, this, &MainWindow::onSelectFilesButtonClicked);
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
        pageButton->setIconOffset(18, pageButton->height() / 2 - 12);
        pageButton->drawPix(iconPaths[index * 2 - 2]);
    }
    ui->pageButton_1->setChecked(true);
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
    if (obj == this && event->type() == QEvent::Close) {
        onMainProcessClosed();
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
    if (mousePressed &&
        (ui->titleBar->geometry().contains(event->pos()) || ui->pageSwitchWidget->geometry().contains(event->pos()))) {
        QPoint move_pos = event->globalPos();
        move(move_pos - movingPoint);
    }
}

void MainWindow::onMainProcessClosed() {
    globalSettings.save();
    qApp->closeAllWindows();
    qApp->quit();
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

void MainWindow::onSelectFilesButtonClicked() {
    QDir dir = QDir::home();
    dir.cd("Pictures");
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("选择图片"), dir.absolutePath(),
                                                          tr("Support Images(*.png *.jpg *.jpeg *.bmp *webp *.gif);;"
                                                             "任意文件 (*.*)"));

    for (const QString &fileName : qAsConst(fileNames)) {
        QFile file(fileName);
        file.open(QFile::ReadOnly);
        QByteArray data = file.readAll();

        if (QImage().loadFromData(data)) {
            QString repeat = "";
            QString tmpHash = QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();
            for (IconWidget *iconwidget : qAsConst(iconWidgets)) {
                if (iconwidget->Hash() == tmpHash) {
                    repeat =
                        iconwidget->imageInfo().completeBaseName() + "." + iconwidget->imageInfo().completeSuffix();
                    break;
                }
            }
            if (repeat == QString("")) {
                if (SMMS::isSupportFormat(data)) {
                    if (iconWidgets.count() < UPLOAD_FILE_LIMIT) {
                        uploadBoxCols = (ui->dragBoxContents->width() - 18) / iconWidgetSize.width();

                        IconWidget *widget = new IconWidget(ui->dragBox);
                        widget->hide();  //减少闪烁

                        QGridLayout *gridLayout = ((QGridLayout *)ui->dragBoxContents->layout());
                        gridLayout->addWidget(widget, iconWidgets.count() / uploadBoxCols,
                                              iconWidgets.count() % uploadBoxCols);
                        widget->setFixedSize(iconWidgetSize);
                        widget->addImageFromFile(fileName);

                        widget->show();

                        iconWidgets.append(widget);

                        connect(widget, &IconWidget::onUploadBtnClicked, this, &MainWindow::uploadFromIconWidget);

                        connect(widget, &IconWidget::onViewBtnClicked, this, [=](IconWidget *obj) {
                            PictureViewWidget &view = PictureViewWidget::Instance();
                            view.setMainWidget(ui->stackedWidget);
                            view.showInfo(obj->imageData(), obj->imageInfo());
                            view.show();
                        });

                        connect(widget, &IconWidget::onDeleteBtnClicked, this, [=](IconWidget *obj) {
                            QList<IconWidget *>::iterator del_iter;
                            for (QList<IconWidget *>::iterator iter = iconWidgets.begin(); iter != iconWidgets.end();
                                 iter++) {
                                if (*iter == obj) {
                                    del_iter = iter;
                                    break;
                                }
                            }
                            iconWidgets.erase(del_iter);

                            while (!gridLayout->isEmpty()) {
                                delete gridLayout->takeAt(0);
                            }

                            obj->deleteLater();

                            uploadBoxCols = (ui->dragBoxContents->width() - 18) / iconWidgetSize.width();
                            int i = 0;
                            for (QList<IconWidget *>::iterator iter = iconWidgets.begin(); iter != iconWidgets.end();
                                 iter++, i++) {
                                gridLayout->addWidget(*iter, i / uploadBoxCols, i % uploadBoxCols);
                            }
                        });
                    } else {
                        notify->newNotify(this, tr("错误"),
                                          tr("待传区图片数量已达上限(limit: %1)").arg(UPLOAD_FILE_LIMIT), "#F56C6C",
                                          "#fff");
                    }
                } else {
                    notify->newNotify(this, tr("警告"), tr("不是受支持的格式"), "#E6A23C", "#fff");
                }
            } else {
                notify->newNotify(this, tr("提示"), tr("重复的文件\n%1").arg(repeat), "#FFFFFF", "#303133");
            }
        } else {
            notify->newNotify(this, tr("错误"), tr("不是有效的图片文件\n文件名: %1").arg(fileName), "#F56C6C", "#fff");
        }
    }
}

void MainWindow::onUploadButtonClicked() {
    bool noPending = true;
    for (auto i : qAsConst(iconWidgets)) {
        qDebug() << i->imageInfo().fileName();
        if (i->status() == IconWidget::PENDING || i->status() == IconWidget::FAILED) {
            uploadFromIconWidget(i);
            noPending = false;
        }
    }
    if (noPending) {
        notify->newNotify(this, tr("信息"), tr("没有文件可以上传"));
    }
}

void MainWindow::uploadFromIconWidget(IconWidget *iconwidget) {
    if (iconwidget->status() == IconWidget::UPLOADING) {
        notify->newNotify(this, tr("信息"), tr("正在上传中"));
        return;
    }
    if (iconwidget->status() == IconWidget::UPLOADED) {
        notify->newNotify(this, tr("信息"), tr("已上传"));
        return;
    }
    HttpClient *client = nullptr;

    if (globalSettings.imghost[KeyMap.imghost_isAuthorized] == true) {
        client = smms->upload(iconwidget->imageData(), iconwidget->imageInfo().fileName(), true);
    } else {
        client = smms->upload(iconwidget->imageData(), iconwidget->imageInfo().fileName(), false);
    }
    iconwidget->setStatus(IconWidget::UPLOADING);
    connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *r) {
        SMMS::Response data;
        SMMS::praseResponse(r->getJson(), data);

        if (data.success) {
            iconwidget->setStatus(IconWidget::UPLOADED);
            notify->newNotify(
                this, tr("上传成功"),
                iconwidget->imageInfo().completeBaseName() + "." + iconwidget->imageInfo().completeSuffix());
        } else {
            iconwidget->setStatus(IconWidget::FAILED);
            notify->newNotify(this, tr("上传失败"),
                              iconwidget->imageInfo().completeBaseName() + "." +
                                  iconwidget->imageInfo().completeSuffix() + "\n" + "message: " + data.message,
                              "#E6A23C", "#FFF");
        }

        qDebug() << r->getText();
        delete r;
    });

    connect(client, &HttpClient::uplpodProgress, iconwidget, &IconWidget::updateUploadProgress);
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
