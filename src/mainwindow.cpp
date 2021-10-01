#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QtConcurrent>

#include "base.h"
#include "utils/promise.h"

using namespace LotosSettings;
using namespace LotosHelper;
using namespace LotosAnimation;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    init();
    componentsManager();
    appearanceManager();

    test();
}

void MainWindow::test() {
    qDebug() << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QSslSocket::supportsSsl();

    connect(ui->tb1, &QPushButton::clicked, this, [=] {
        if (globalSettings.imghost[KeyMap.imghost.isAuthorized] == true) {
            HttpClient *client = nullptr;
            client = smms->getUploadHistory(1);
            connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *res) {
                qDebug() << res->getText();
                SMMS::Response data;
                SMMS::praseResponse(res->getJson(), data);
                for (const auto &obj : qAsConst(data.data)) {
                    SMMS::ImageInfomation info;
                    SMMS::praseImageInfomation(obj.toObject(), info);
                    qDebug() << timestamp2str(info.timestamp, "yyyy/MM/dd hh:mm:ss");
                    qDebug() << formatExternalLink(info.filename, info.url, ExternalLinkType::Markdown);
                }
                delete res;
            });
        }
    });

    connect(ui->tb2, &QPushButton::clicked, this, [=] {
        Promise<int> *promise = new Promise<int>(this);
        promise->setPromise([=](Promise<int>::Resolver resolve, Promise<int>::Rejector) {
            QThread::msleep(800);
            QMetaObject::invokeMethod(this, "addIconWidget", Qt::QueuedConnection,
                                      Q_ARG(QString, ":/res/lotos_icon.png"));
            resolve(100);
        });
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::appearanceManager() {
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

    ui->titleBar->setTitle(tr("   Lotos v") + APP_VERSION);

    maskFrame = new QFrame(this);
    maskFrame->setObjectName("mask");
    QRect tmpRect = rect();
    tmpRect.setTopLeft(QPoint(7, 7));
    tmpRect.setBottomRight(tmpRect.bottomRight() - QPoint(7, 7));
    maskFrame->setGeometry(tmpRect);
    maskFrame->setLayout(new QHBoxLayout);
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    maskFrame->setGraphicsEffect(effect);
    maskFrame->hide();

    QGridLayout *gridlayout = new QGridLayout;
    gridlayout->setContentsMargins(8, 8, 8, 8);
    gridlayout->setHorizontalSpacing(8);
    gridlayout->setVerticalSpacing(8);
    gridlayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->dragBoxContents->setLayout(gridlayout);

    ui->selectFilesButton->setProperty(StyleType.name, StyleType.button.primary);
    ui->uploadButton->setProperty(StyleType.name, StyleType.button.primary);
    ui->deleteAllButton->setProperty(StyleType.name, StyleType.button.danger);

    ui->hostResetButton->setProperty(StyleType.name, StyleType.button.normal);
    ui->hostLoginButton->setProperty(StyleType.name, StyleType.button.normal);

    QDir stylesDir(":/res/styles/");
    QStringList stylesFilter;
    stylesFilter << "*.css"
                 << "*.qss";
    stylesDir.setNameFilters(stylesFilter);
    const QStringList entryList = stylesDir.entryList();
    for (const QString &stylesheet : entryList) {
        loadQStyleSheet(stylesDir.path() + "/" + stylesheet);
    }
}

void MainWindow::init() {
    proxy = HttpClient::getNetworkProxyInstanse();
    smms = &SMMS::Instance();
    globalSettings.load(PATH_CONFIG);
    notify = &NotificationManager::Instance();
    notify->setNotifyParent(ui->viewport);
}

void MainWindow::componentsManager() {
    connect(ui->titleBar, &TitleBar::onMiniBtnClicked, this, &MainWindow::showMinimized);
    connect(ui->titleBar, &TitleBar::onCloseBtnClicked, this, &MainWindow::onMainProcessClosed);

    // set imghost upload page
    connect(ui->selectFilesButton, &QPushButton::clicked, this, &MainWindow::onButtonSelectFilesClicked);
    connect(ui->uploadButton, &QPushButton::clicked, this, &MainWindow::onButtonUploadClicked);
    connect(ui->deleteAllButton, &QPushButton::clicked, this, &MainWindow::delAllIconWidgets);

    connect(ui->dragBox, &PicturesContainer::acceptDragFileName, this, &MainWindow::addIconWidget);
    connect(this, &MainWindow::uploadStatusChanged, this, &MainWindow::onUploadStatusChanged);

    // set imghost dashbroad page
    ui->pageSwitchWidget->installEventFilter(this);
    ui->hostUserDiskLimit->hide();
    connect(ui->hostLoginButton, &QPushButton::clicked, this, &MainWindow::onHostLoginClicked);
    connect(ui->hostResetButton, &QPushButton::clicked, this, &MainWindow::onButtonHostResetClicked);

    // set pagebutton toggle signal&icon
    QList<PageButton *> PageButtons = ui->centralwidget->findChildren<PageButton *>();

    for (PageButton *pageButton : qAsConst(PageButtons)) {
        int index = pageButton->objectName().rightRef(1).toInt() - 1;

        pageButton->setIndex(index);
        loadPage(PAGE(index));
        connect(this, &MainWindow::widgetPageChanged, pageButton, &PageButton::setCurrentChosen);
        connect(pageButton, &PageButton::indexChanged, this, &MainWindow::loadPage);

        pageButton->setIconPath(iconPaths.at(index * 2), iconPaths.at(index * 2 + 1));
        pageButton->setIconSize(QSize(28, 28));
        pageButton->setIconOffset(18, pageButton->height() / 2 - 12);
        pageButton->drawPix(iconPaths[index * 2]);
    }
    ui->pageButton_1->setChecked(true);
    ui->stackedWidget->setCurrentIndex(UploadPage);
    loadPage(UploadPage);

    connect(ui->preferIPRadio, &QRadioButton::clicked, this,
            [=] { globalSettings.user[KeyMap.user.uploadWithToken] = false; });
    connect(ui->preferTokenRadio, &QRadioButton::clicked, this,
            [=] { globalSettings.user[KeyMap.user.uploadWithToken] = true; });
    connect(ui->SaveNameEdit, &QLineEdit::textChanged, this,
            [=](const QString &str) { globalSettings.user[KeyMap.user.clipSaveFileName] = str; });
    connect(ui->SaveFormatCombo, &QComboBox::currentTextChanged, this,
            [=](const QString &str) { globalSettings.user[KeyMap.user.clipSaveImageType] = str; });

    ui->dragBoxContents->installEventFilter(this);
    installEventFilter(this);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == this && event->type() == QEvent::Close) {
        onMainProcessClosed();
    }
    if (obj == ui->dragBoxContents && event->type() == QEvent::Paint) {
        if (!iconWidgets.size()) {
            QPainter painter(static_cast<QWidget *>(obj));
            QFont font = QFont("Microsoft YaHei", 36);

            QString tipstr = tr("Paste & Drag & Select") + tr(" Files Here...");

            painter.setFont(font);
            painter.setPen(QColor("#C0C4CC"));
            QRectF rect = ui->dragBoxContents->rect();
            rect.setRight(rect.width() / 4 * 3);
            rect.setLeft(rect.width() / 4);
            painter.drawText(rect, Qt::TextWordWrap | Qt::AlignCenter, tipstr);
        }

        return false;
    }

    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (currentPage == MainWindow::UploadPage) {
        if (event->matches(QKeySequence::Paste)) {
            QClipboard *board = QApplication::clipboard();
            if (board->mimeData()->hasUrls()) {
                QList<QUrl> urls = board->mimeData()->urls();
                for (const QUrl &url : qAsConst(urls)) {
                    QString fileName = url.toLocalFile();
                    if (QFile(fileName).exists()) {
                        addIconWidget(fileName);
                    };
                }
            }
            if (board->mimeData()->hasImage()) {
                const QImage &img = board->image();
                const QString formatTime = globalSettings.user[KeyMap.user.clipSaveFileName].toString();

                const QString filename = QDir::temp().absolutePath() + "/" +
                                         QDateTime::currentDateTime().toString(formatTime) + "." +
                                         globalSettings.user[KeyMap.user.clipSaveImageType].toString();

                if (img.save(filename)) {
                    addIconWidget(filename);
                } else {
                    notify->newNotify(NOTIFYS::ERROR, NOTIFYS::imageClipBoardNotSave(formatTime));
                }
            }
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        movingPoint = event->globalPos() - this->pos();
        if ((ui->titleBar->geometry().contains(event->pos()) ||
             ui->pageSwitchWidget->geometry().contains(event->pos()))) {
            mousePressed = true;
        }
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

void MainWindow::onMainProcessClosed() {
    globalSettings.save();
    qApp->closeAllWindows();
    qApp->quit();
}

void MainWindow::onHostLoginClicked() {
    if (!globalSettings.imghost[KeyMap.imghost.isAuthorized].toBool()) {
        msgBox = openMessagegBox();
        msgBox->setIcontype(MessageBox::WAIT);

        HttpClient *client = smms->getAPIToken(ui->hostUsername->text(), ui->hostPassword->text());

        connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *res) {
            SMMS::Response data;
            SMMS::praseResponse(res->getJson(), data);

            if (res->statusCode.toUInt() != 200) {
                msgBox->setIcontype(MessageBox::ERROR);
                ui->userInfoLabel->setText(tr("网络错误!"));
            } else if (data.success == false) {
                msgBox->setIcontype(MessageBox::ERROR);
                ui->userInfoLabel->setText(tr("<h4>登录失败 </h4>") + data.message);
            } else {
                msgBox->setIcontype(MessageBox::SUCCESS);
                QString token = data.data.at(0).toObject()["token"].toString();
                globalSettings.imghost.insert(KeyMap.imghost.token, token);
                globalSettings.imghost.insert(KeyMap.imghost.isAuthorized, true);
                globalSettings.save();
                onHostLoginClicked();
            }

            closeMessageBox(1000);
            delete res;
        });
    } else {
        smms->setToken(globalSettings.imghost[KeyMap.imghost.token].toString());

        ui->hostLoginButton->setEnabled(false);
        ui->hostLoginButton->setText(tr("已登录"));
        ui->tokenLabel->setText(smms->token());
        ui->userInfoLabel->setText(tr("正在获取用户信息..."));

        HttpClient *client = smms->getUserProfile();
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
            delete res;
        });
    }
}

void MainWindow::onButtonHostResetClicked() {
    globalSettings.imghost.insert(KeyMap.imghost.token, "");
    globalSettings.imghost.insert(KeyMap.imghost.isAuthorized, false);
    globalSettings.save();

    ui->hostLoginButton->setEnabled(true);
    ui->hostLoginButton->setText(tr("登录"));
    ui->hostUsername->clear();
    ui->hostPassword->clear();

    ui->tokenLabel->setText(tr("未登录"));

    ui->hostUserDiskLimit->hide();
    ui->userInfoLabel->setText(QString(tr("<div><b>用户组:</b> 未登录用户</div>")));
}

void MainWindow::onButtonSelectFilesClicked() {
    QDir dir = QDir::home();
    dir.cd("Pictures");
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("选择图片"), dir.absolutePath(),
                                                          tr("Support Images(*.png *.jpg *.jpeg *.bmp *webp *.gif);;"
                                                             "任意文件 (*.*)"));

    for (const QString &fileName : qAsConst(fileNames)) {
        addIconWidget(fileName);
    }
}

void MainWindow::onButtonUploadClicked() {
    bool noPending = true;
    for (auto i : qAsConst(iconWidgets)) {
        if (i->status() == IconWidget::PENDING || i->status() == IconWidget::FAILED) {
            uploadFromIconWidget(i);
            noPending = false;
        }
    }
    if (noPending) {
        notify->newNotify(NOTIFYS::INFO, NOTIFYS::imageWidgetNoPending());
    }
}

void MainWindow::addIconWidget(QString filename) {
    Promise<QByteArray> *promise = new Promise<QByteArray>(this);
    Promise<void> *promise1 = new Promise<void>(this);
    Promise<QString> *promise2 = new Promise<QString>(this);

    promise->onFinished([=](Promise<QByteArray>::result data) {
        promise1->setPromise([=](Promise<void>::Resolver, Promise<void>::Rejector reject) {
            if (QImage::fromData(data).isNull()) {
                reject();
            }
        });

        promise2->onFinished([=](Promise<QString>::result v) {
            QString tmpHash = v;
            QString repeat = "";
            for (IconWidget *iconwidget : qAsConst(iconWidgets)) {
                if (iconHashs[iconwidget] == tmpHash) {
                    repeat = iconwidget->imageInfo().fileName();
                    break;
                }
            }
            if (repeat == QString("")) {
                if (SMMS::supportFormat(data) != "") {
                    if (iconWidgets.count() < UPLOAD_FILE_LIMIT) {
                        if (!iconWidgets.size()) {
                            ui->dragBoxContents->update();
                        }
                        uploadBoxCols = (ui->dragBoxContents->width() - 18) / iconWidgetSize.width();

                        IconWidget *widget = new IconWidget(ui->dragBox);
                        widget->hide();  //减少闪烁

                        QGridLayout *gridLayout = static_cast<QGridLayout *>(ui->dragBoxContents->layout());
                        gridLayout->addWidget(widget, iconWidgets.count() / uploadBoxCols,
                                              iconWidgets.count() % uploadBoxCols);
                        widget->setFixedSize(iconWidgetSize);
                        widget->addImageFromFile(filename, data);

                        widget->show();

                        iconWidgets.append(widget);
                        iconHashs.insert(widget, tmpHash);
                        iconClients.insert(widget, nullptr);
                        emit uploadStatusChanged();

                        connect(widget, &IconWidget::onUploadBtnClicked, this, &MainWindow::uploadFromIconWidget);
                        connect(widget, &IconWidget::onViewBtnClicked, this, &MainWindow::previewFromIconWidget);
                        connect(widget, &IconWidget::onDeleteBtnClicked, this, &MainWindow::delIconWidget);
                    } else {
                        notify->newNotify(NOTIFYS::ERROR, NOTIFYS::imageWidgetLimit(UPLOAD_FILE_LIMIT));
                    }
                } else {
                    notify->newNotify(NOTIFYS::ERROR, NOTIFYS::imageNotSupported());
                }
            } else {
                notify->newNotify(NOTIFYS::ERROR, NOTIFYS::imageRepeated(filename));
            }
        });

        promise2->setPromise([=](Promise<QString>::Resolver resolve, Promise<QString>::Rejector) {
            resolve(QCryptographicHash::hash(data.left(1024 * 20), QCryptographicHash::Md5).toHex());
        });
    });

    promise->setPromise([=](Promise<QByteArray>::Resolver resolve, Promise<QByteArray>::Rejector reject) {
        QFile file(filename);
        if (!file.open(QFile::ReadOnly)) {
            reject();
        }
        resolve(file.readAll());
    });

    promise->onFailed([=] { notify->newNotify(NOTIFYS::ERROR, NOTIFYS::imageFileError(filename)); });
    promise1->onFailed([=] { notify->newNotify(NOTIFYS::ERROR, NOTIFYS::imageFileError(filename)); });
}

void MainWindow::uploadFromIconWidget(IconWidget *iconwidget) {
    QPointer<HttpClient> &client = iconClients[iconwidget];
    if (globalSettings.imghost[KeyMap.imghost.isAuthorized] == true &&
        globalSettings.user[KeyMap.user.uploadWithToken] == true) {
        client = smms->upload(iconwidget->imageData(), iconwidget->imageInfo().fileName(), true);
    } else {
        client = smms->upload(iconwidget->imageData(), iconwidget->imageInfo().fileName(), false);
    }
    iconwidget->setStatus(IconWidget::UPLOADING);
    connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *r) {
        if (r->isSucceeded == false) {
            iconwidget->setStatus(IconWidget::FAILED);
            if (r->ERROR_INFO.toString() == "Operation canceled") {
                notify->newNotify(NOTIFYS::WARN, NOTIFYS::uploadCanceled());
            } else {
                notify->newNotify(NOTIFYS::ERROR,
                                  NOTIFYS::networkError(r->statusCode.toString(), r->ERROR_INFO.toString()));
            }

            return;
        }
        SMMS::Response data;
        SMMS::praseResponse(r->getJson(), data);

        if (data.success) {
            iconwidget->setStatus(IconWidget::UPLOADED);
            notify->newNotify(NOTIFYS::SUCCESS, NOTIFYS::uploadSucceed(iconwidget->imageInfo().fileName()));
        } else {
            iconwidget->setStatus(IconWidget::FAILED);
            notify->newNotify(NOTIFYS::WARN, NOTIFYS::uploadFailed(iconwidget->imageInfo().fileName(), data.message));
        }

        qDebug() << r->getText();
        delete r;
    });

    connect(client, &HttpClient::uplpodProgress, iconwidget, &IconWidget::updateUploadProgress);
}

void MainWindow::loadPage(int index) {
    emit widgetPageChanged(index);
    ui->stackedWidget->setCurrentIndex(index);
    switch (MainWindow::PAGE(index)) {
        case UploadPage:
            emit uploadStatusChanged();
            break;
        case HostDashBoardPage:
            ui->hostUsername->clear();
            ui->hostPassword->clear();
            if (globalSettings.imghost[KeyMap.imghost.isAuthorized].toBool()) {
                ui->hostLoginButton->click();
            } else {
                ui->tokenLabel->setText(tr("未登录"));
                ui->hostUserDiskLimit->hide();
                ui->userInfoLabel->setText(QString(tr("<div><b>用户组:</b> 未登录用户</div>")));
            }
            break;
        case SettingsPage:
            ui->SaveFormatCombo->setCurrentText(globalSettings.user[KeyMap.user.clipSaveImageType].toString());
            ui->SaveNameEdit->setText(globalSettings.user[KeyMap.user.clipSaveFileName].toString());
            if (globalSettings.user[KeyMap.user.uploadWithToken].toBool()) {
                ui->preferTokenRadio->setChecked(true);
            } else {
                ui->preferIPRadio->setChecked(true);
            }
            break;
        default:
            break;
    }
}

void MainWindow::delAllIconWidgets() {
    QGridLayout *l = static_cast<QGridLayout *>(ui->dragBoxContents->layout());
    for (auto iw : qAsConst(iconWidgets)) {
        if (!iconClients[iw].isNull()) {
            iconClients[iw]->cancel();
        }
        iw->setGraphicsEffect(nullptr);
        iw->deleteLater();
        l->removeWidget(iw);
    }
    iconWidgets.clear();
    iconHashs.clear();
    iconClients.clear();
    ui->dragBoxContents->update();
    emit uploadStatusChanged();
}

void MainWindow::delIconWidget(IconWidget *obj) {
    QGridLayout *gridLayout = static_cast<QGridLayout *>(ui->dragBoxContents->layout());

    if (!iconClients[obj].isNull()) {
        iconClients[obj]->cancel();
    }
    obj->setGraphicsEffect(nullptr);
    obj->deleteLater();

    iconWidgets.removeOne(obj);
    iconHashs.remove(obj);
    iconClients.remove(obj);

    while (!gridLayout->isEmpty()) {
        delete gridLayout->takeAt(0);
    }

    //    uploadBoxCols = (ui->dragBoxContents->width() - 18) / iconWidgetSize.width();
    int i = 0;
    for (QList<IconWidget *>::iterator iter = iconWidgets.begin(); iter != iconWidgets.end(); iter++, i++) {
        gridLayout->addWidget(*iter, i / uploadBoxCols, i % uploadBoxCols);
    }
    ui->dragBoxContents->update();
    emit uploadStatusChanged();
}

void MainWindow::previewFromIconWidget(IconWidget *obj) {
    PictureViewWidget &view = PictureViewWidget::Instance();
    view.setMainWidget(ui->stackedWidget);
    view.showInfo(obj->imageData(), obj->imageInfo());
    view.show();
}

void MainWindow::onUploadStatusChanged() {
    ui->uploadStatusLabel->setText(
        QString(tr("待传区文件数: %1 \n 上传模式: %2"))
            .arg(iconWidgets.size())
            .arg(globalSettings.user[KeyMap.user.uploadWithToken].toBool() ? "优先使用Token" : "基于IP"));
}

MessageBox *MainWindow::openMessagegBox() {
    MessageBox *msgBox = new MessageBox(this);
    maskFrame->layout()->addWidget(msgBox);
    maskFrame->stackUnder(msgBox);
    maskFrame->show();

    QGraphicsOpacityEffect *maskEffect = static_cast<QGraphicsOpacityEffect *>(maskFrame->graphicsEffect());
    maskEffect->setOpacity(0);
    fade(maskEffect, maskFrame)->start(QAbstractAnimation::DeleteWhenStopped);

    msgBox->show();
    return msgBox;
}

void MainWindow::closeMessageBox(int msecs) {
    QGraphicsOpacityEffect *maskEffect = static_cast<QGraphicsOpacityEffect *>(maskFrame->graphicsEffect());
    QSequentialAnimationGroup *g = new QSequentialAnimationGroup(this);
    g->addAnimation(fade(maskEffect, maskFrame, 100, false));
    g->addPause(msecs);
    g->start(QAbstractAnimation::DeleteWhenStopped);

    connect(g, &QAbstractAnimation::finished, maskFrame, [=] {
        maskFrame->hide();
        msgBox->close();
    });
}
