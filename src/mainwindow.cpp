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
            HttpClient *client = smms->getUploadHistory(1);
            connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *res) {
                SMMS::Response data;
                SMMS::praseResponse(res->getJson(), data);
                for (const auto &obj : qAsConst(data.data)) {
                    SMMS::ImageInfomation info;
                    SMMS::praseImageInfomation(obj.toObject(), info);
                    info.token_with = true;

                    ui->imagesList->addData(info.toQVariantMap());
                }
                delete res;
            });
        }
        HttpClient *client = smms->getTemporaryUploadHistory();
        connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *res) {
            SMMS::Response data;
            qDebug() << res->getText();
            SMMS::praseResponse(res->getJson(), data);
            for (const auto &obj : qAsConst(data.data)) {
                SMMS::ImageInfomation info;
                SMMS::praseImageInfomation(obj.toObject(), info);
                info.token_with = false;

                ui->imagesList->addData(info.toQVariantMap());
            }
            delete res;
        });
    });

    connect(ui->tb2, &QPushButton::clicked, this, [=] {
        //        Promise<int> *promise = new Promise<int>(this);
        //        promise->setPromise([=](Promise<int>::Resolver resolve, Promise<int>::Rejector) {
        //            QThread::msleep(800);
        //            QMetaObject::invokeMethod(this, "addIconWidget", Qt::QueuedConnection,
        //                                      Q_ARG(QString, ":/res/lotos_icon.png"));
        //            resolve(100);
        //        });
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
    shadowGenerator(box_shadow, 0.18, 0, 1, 12);
    ui->viewport->setGraphicsEffect(box_shadow);

    QGraphicsDropShadowEffect *sider_bar_shadow = new QGraphicsDropShadowEffect(this);
    shadowGenerator(sider_bar_shadow, 0.18, 2, 0, 12);
    ui->pageSwitchWidget->setGraphicsEffect(sider_bar_shadow);

    QLabel *mainIcon = new QLabel(this);
    QLabel *mainTitle = new QLabel(this);
    mainIcon->setScaledContents(true);
    mainIcon->setFixedSize(QSize(96, 96));
    mainIcon->setPixmap(QPixmap(":/res/lotos_icon.png"));
    QGraphicsDropShadowEffect *icon_shadow = new QGraphicsDropShadowEffect(this);
    shadowGenerator(icon_shadow, 0.28, 0, 0, 20);
    mainIcon->setGraphicsEffect(icon_shadow);

    mainTitle->setText(tr("Lotos"));
    mainTitle->setFont(QFont("Agency FB", 32));

    QVBoxLayout *sider_layout = dynamic_cast<QVBoxLayout *>(ui->pageSwitchWidget->layout());
    sider_layout->insertWidget(0, mainIcon, 0, Qt::AlignHCenter);
    sider_layout->insertWidget(1, mainTitle, 0, Qt::AlignHCenter);

    ui->titleBar->setTitle(tr("Lotos v") + APP_VERSION);

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
    ui->uploadBoxContents->setLayout(gridlayout);
    ui->uploadBox->getContentsLayout();

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
    for (const QString &stylesheet : stylesDir.entryList()) {
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

    // set upload page
    connect(ui->selectFilesButton, &QPushButton::clicked, this, &MainWindow::onButtonSelectFilesClicked);
    connect(ui->uploadButton, &QPushButton::clicked, this, &MainWindow::onButtonUploadClicked);
    connect(ui->deleteAllButton, &QPushButton::clicked, ui->uploadBox, &PicturesContainer::delAllIconWidgets);

    connect(this, &MainWindow::uploadStatusChanged, this, &MainWindow::onUploadStatusChanged);

    connect(ui->uploadBox, &PicturesContainer::iconWidgetsChanged, this, &MainWindow::onUploadStatusChanged);
    connect(ui->uploadBox, &PicturesContainer::uploadImage, this, &MainWindow::uploadImage);

    // set gallery page

    // set dashbroad page
    ui->pageSwitchWidget->installEventFilter(this);
    ui->hostUserDiskLimit->hide();
    connect(ui->hostLoginButton, &QPushButton::clicked, this, &MainWindow::onHostLoginClicked);
    connect(ui->hostResetButton, &QPushButton::clicked, this, &MainWindow::onButtonHostResetClicked);

    // set settings page
    connect(ui->preferIPRadio, &QRadioButton::clicked, this,
            [=] { globalSettings.user[KeyMap.user.uploadWithToken] = false; });
    connect(ui->preferTokenRadio, &QRadioButton::clicked, this,
            [=] { globalSettings.user[KeyMap.user.uploadWithToken] = true; });
    connect(ui->SaveNameEdit, &QLineEdit::textChanged, this,
            [=](const QString &str) { globalSettings.user[KeyMap.user.clipSaveFileName] = str; });
    connect(ui->SaveFormatCombo, &QComboBox::currentTextChanged, this,
            [=](const QString &str) { globalSettings.user[KeyMap.user.clipSaveImageType] = str; });

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

    ui->uploadBoxContents->installEventFilter(this);
    installEventFilter(this);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == this && event->type() == QEvent::Close) {
        onMainProcessClosed();
    }
    if (obj == ui->uploadBoxContents && event->type() == QEvent::Paint) {
        if (!ui->uploadBox->count()) {
            QPainter painter(static_cast<QWidget *>(obj));
            QFont font = QFont("Microsoft YaHei", 36);

            QString tipstr = tr("Paste & Drag & Select") + tr(" Files Here...");

            painter.setFont(font);
            painter.setPen(QColor("#C0C4CC"));
            QRectF rect = ui->uploadBoxContents->rect();
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
                QStringList list;
                for (const QUrl &url : qAsConst(urls)) {
                    list << url.toLocalFile();
                }
                ui->uploadBox->addIconWidgets(list);
            }
            if (board->mimeData()->hasImage()) {
                const QImage &img = board->image();
                const QString formatTime = globalSettings.user[KeyMap.user.clipSaveFileName].toString();

                const QString filename = QDir::temp().absolutePath() + "/" +
                                         QDateTime::currentDateTime().toString(formatTime) + "." +
                                         globalSettings.user[KeyMap.user.clipSaveImageType].toString();

                if (img.save(filename)) {
                    ui->uploadBox->addIconWidget(filename);
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
    ui->uploadBox->addIconWidgets(fileNames);
}

void MainWindow::onButtonUploadClicked() {
    int pending = 0;
    for (IconWidget *i : ui->uploadBox->widgets()) {
        if (i->status() == IconWidget::PENDING || i->status() == IconWidget::FAILED) {
            uploadImage(i);
            pending++;
        }
    }
    if (!pending) {
        notify->newNotify(NOTIFYS::INFO, NOTIFYS::imageWidgetNoPending());
    }
}

void MainWindow::uploadImage(IconWidget *obj) {
    QPointer<HttpClient> &client = ui->uploadBox->client(obj);
    if (globalSettings.imghost[KeyMap.imghost.isAuthorized] == true &&
        globalSettings.user[KeyMap.user.uploadWithToken] == true) {
        client = smms->upload(obj->imageData(), obj->imageInfo().fileName(), true);
    } else {
        client = smms->upload(obj->imageData(), obj->imageInfo().fileName(), false);
    }
    obj->setStatus(IconWidget::UPLOADING);
    connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *r) {
        if (r->isSucceeded == false) {
            obj->setStatus(IconWidget::FAILED);
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
            obj->setStatus(IconWidget::UPLOADED);
            notify->newNotify(NOTIFYS::SUCCESS, NOTIFYS::uploadSucceed(obj->imageInfo().fileName()));
        } else {
            obj->setStatus(IconWidget::FAILED);
            notify->newNotify(NOTIFYS::WARN, NOTIFYS::uploadFailed(obj->imageInfo().fileName(), data.message));
        }

        qDebug() << r->getText();
        delete r;
    });

    connect(client, &HttpClient::uplpodProgress, obj, &IconWidget::updateUploadProgress);
}

void MainWindow::loadPage(int index) {
    emit widgetPageChanged(index);
    ui->stackedWidget->setCurrentIndex(index);
    switch (MainWindow::PAGE(index)) {
        case UploadPage:
            emit uploadStatusChanged();
            break;
        case GalleryPage:
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

void MainWindow::onUploadStatusChanged() {
    ui->uploadStatusLabel->setText(
        QString(tr("待传区文件数: %1 \n 上传模式: %2"))
            .arg(ui->uploadBox->count())
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

void MainWindow::closeMessageBox(int pause) {
    QGraphicsOpacityEffect *maskEffect = static_cast<QGraphicsOpacityEffect *>(maskFrame->graphicsEffect());
    QSequentialAnimationGroup *g = new QSequentialAnimationGroup(this);
    g->addPause(pause);
    g->addAnimation(fade(maskEffect, maskFrame, 100, false));
    g->start(QAbstractAnimation::DeleteWhenStopped);

    connect(g, &QAbstractAnimation::finished, maskFrame, [=] {
        maskFrame->hide();
        msgBox->close();
    });
}
