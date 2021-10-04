#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFontDatabase>
#include <QVersionNumber>
#include <QtConcurrent>

#include "base.h"
#include "utils/promise.h"

using namespace LotosSettings;
using namespace LotosHelper;
using namespace LotosAnimation;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    qApp->setDesktopFileName(tr("Lotos"));

    init();
    componentsManager();
    appearanceManager();

    ui->galleryFooter->setVisible(false);
#ifdef QT_DEBUG
    test();
#endif
}

void MainWindow::test() {
    ui->galleryFooter->setVisible(true);
    qDebug() << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QSslSocket::supportsSsl();

    connect(ui->tb1, &QPushButton::clicked, this, [=] {
        LinkCopyBox *msgBox = new LinkCopyBox(this);
        openMessagegBox(msgBox);
        closeMessageBox(msgBox, 5000);
    });

    connect(ui->tb2, &QPushButton::clicked, this, [=] {
        NetworkResponseBox *msgBox = new NetworkResponseBox(this);
        openMessagegBox(msgBox);
        msgBox->setTip(tr("请稍等啊!!ksdfhno;ksdhnfilukegdiud"));
        msgBox->setIcontype(MessageBox::IconType::WAIT);
        msgBox->setProgress(50);
        closeMessageBox(msgBox, 5000);
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::init() {
    proxy = HttpClient::getNetworkProxyInstanse();
    smms = &SMMS::Instance();
    globalSettings.load(PATH_CONFIG);
    notify = &NotificationManager::Instance();
    notify->setNotifyParent(ui->viewport);
}

void MainWindow::appearanceManager() {
    QDir fontsDir(":/res/fonts");
    fontsDir.setNameFilters(QStringList() << "*.ttf");
    for (const QString &fontname : fontsDir.entryList()) {
        QFontDatabase::addApplicationFont(fontsDir.path() + "/" + fontname);
    }

    QFont f("Roboto Mono,Microsoft YaHei UI", 12);
    f.setStyleStrategy(QFont::PreferAntialias);
    qApp->setFont(f);

    setWindowTitle(tr("Lotos"));
    setWindowIcon(QIcon(":/res/lotos_icon.png"));
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

#if (QT_VERSION > QT_VERSION_CHECK(5, 9, 8))
    QGraphicsDropShadowEffect *box_shadow = new QGraphicsDropShadowEffect(this);
    shadowGenerator(box_shadow, 0.18, 0, 1, 12);
    ui->viewport->setGraphicsEffect(box_shadow);
#endif

    QGraphicsDropShadowEffect *sider_bar_shadow = new QGraphicsDropShadowEffect(this);
    shadowGenerator(sider_bar_shadow, 0.18, 2, 0, 12);
    ui->pageSwitchWidget->setGraphicsEffect(sider_bar_shadow);

    QLabel *mainIcon = new QLabel(this);
    QLabel *mainTitle = new QLabel(this);
    mainIcon->setScaledContents(true);
    mainIcon->setFixedSize(QSize(96, 96));
    mainIcon->setPixmap(QPixmap(":/res/lotos_icon.png"));
    mainTitle->setFixedSize(QSize(96, 48));
    mainTitle->setPixmap(QPixmap(":/res/lotos_title.png").scaledToWidth(96, Qt::SmoothTransformation));

#if (QT_VERSION > QT_VERSION_CHECK(5, 9, 8))
    QGraphicsDropShadowEffect *icon_shadow = new QGraphicsDropShadowEffect(this);
    shadowGenerator(icon_shadow, 0.28, 0, 0, 20);
    mainIcon->setGraphicsEffect(icon_shadow);

    QGraphicsDropShadowEffect *title_shadow = new QGraphicsDropShadowEffect(this);
    shadowGenerator(title_shadow, 0.28, 0, 0, 20);
    mainTitle->setGraphicsEffect(title_shadow);
#endif

    QVBoxLayout *sider_layout = dynamic_cast<QVBoxLayout *>(ui->pageSwitchWidget->layout());
    sider_layout->insertWidget(0, mainIcon, 0, Qt::AlignHCenter);
    sider_layout->insertWidget(1, mainTitle, 0, Qt::AlignHCenter);

    ui->titleBar->setTitle(tr("Lotos v") + LOTOS_VERSION);

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

    ui->syncGalleryButton->setProperty(StyleType.name, StyleType.button.normal);
    ui->deleteSelectedButton->setProperty(StyleType.name, StyleType.button.normal);

    ui->hostResetButton->setProperty(StyleType.name, StyleType.button.normal);
    ui->hostLoginButton->setProperty(StyleType.name, StyleType.button.normal);
    ui->hostUserDiskLimit->setProperty(StyleType.name, StyleType.progressbar.normal);

    ui->aboutButton->setProperty(StyleType.name, StyleType.button.normal);

    ui->versionLabel->setText(tr("当前版本: ") + LOTOS_VERSION);
    ui->deleteAllShortCut->setDisabled(true);
    ui->openImageShortCut->setDisabled(true);
    ui->uploadShortCut->setDisabled(true);

    QDir stylesDir(":/res/styles/");
    stylesDir.setNameFilters(QStringList() << "*.css"
                                           << "*.qss");
    for (const QString &stylesheet : stylesDir.entryList()) {
        loadQStyleSheet(stylesDir.path() + "/" + stylesheet);
    }
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
    connect(ui->syncGalleryButton, &QPushButton::clicked, this, &MainWindow::onButtonSyncGalleryClicked);
    connect(ui->deleteSelectedButton, &QPushButton::clicked, ui->imagesList, &PictureTable::delSelectedItems);
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &MainWindow::onEditSearchTextChanged);
    connect(ui->imagesList, &PictureTable::downloadStarted, this, &MainWindow::downloadImage);
    connect(ui->imagesList, &PictureTable::copyLinkStarted, this, &MainWindow::copyLinks);

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
            QFont font("Roboto Mono", 36, 0);

            QString tipstr = tr("Paste & Drag & Select Files Here...");

            painter.setFont(font);
            painter.setPen(QColor("#C0C4CC"));
            QRectF rect = ui->uploadBoxContents->rect();
            rect.setRight(rect.width() / 8 * 7);
            rect.setLeft(rect.width() / 8);
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
        NetworkResponseBox *msgBox = new NetworkResponseBox(this);
        openMessagegBox(msgBox);
        msgBox->setIcontype(MessageBox::WAIT);
        msgBox->setTip(tr("正在登录中..."));

        HttpClient *client = smms->getAPIToken(ui->hostUsername->text(), ui->hostPassword->text());

        connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *res) {
            SMMS::Response data;
            SMMS::praseResponse(res->getJson(), data);

            if (res->statusCode.toUInt() != 200) {
                msgBox->setTip(tr("网络错误!"));
                msgBox->setIcontype(MessageBox::ERROR);
                ui->userInfoLabel->setText(tr("网络错误!"));
            } else if (data.success == false) {
                msgBox->setTip(tr("登录失败"));
                msgBox->setIcontype(MessageBox::ERROR);
                ui->userInfoLabel->setText(tr("<h4>登录失败 </h4>") + data.message);
            } else {
                msgBox->setTip(tr("登录成功"));
                msgBox->setIcontype(MessageBox::SUCCESS);
                QString token = data.data.at(0).toObject()["token"].toString();
                globalSettings.imghost.insert(KeyMap.imghost.token, token);
                globalSettings.imghost.insert(KeyMap.imghost.isAuthorized, true);
                globalSettings.save();
                onHostLoginClicked();
            }

            closeMessageBox(msgBox, 1000);
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

void MainWindow::onButtonSyncGalleryClicked() {
    NetworkResponseBox *msg = new NetworkResponseBox(this);
    openMessagegBox(msg);
    msg->setIcontype(MessageBox::WAIT);
    msg->setTip("正在同步中...");

    if (globalSettings.imghost[KeyMap.imghost.isAuthorized] == true) {
        ParallelCount *req = new ParallelCount(this);
        HttpClient *client = smms->getUploadHistory(1);
        req->add();
        connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *res) {
            SMMS::Response data;
            SMMS::praseResponse(res->getJson(), data);
            for (const auto &obj : qAsConst(data.data)) {
                SMMS::ImageInfomation info;
                SMMS::praseImageInfomation(obj.toObject(), info);
                info.token_with = true;

                req->store(info.toQVariantMap());
            }

            req->pop();
            delete res;
        });

        connect(req, &ParallelCount::clear, this, [=] {
            closeMessageBox(msg);
            QList<QVariantMap> r;
            for (const auto &i : req->get()) {
                r.append(i.toMap());
            }
            ui->imagesList->refresh(r);
        });
    } else {
        msg->setIcontype(MessageBox::WARN);
        msg->setTip(tr("当前版本暂时不支持管理本地上传历史列表"));
        closeMessageBox(msg, 1000);
    }
    //    HttpClient *client = smms->getTemporaryUploadHistory();
    //    req->add();
    //    connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *res) {
    //        SMMS::Response data;
    //        qDebug() << res->getText();
    //        SMMS::praseResponse(res->getJson(), data);
    //        for (const auto &obj : qAsConst(data.data)) {
    //            SMMS::ImageInfomation info;
    //            SMMS::praseImageInfomation(obj.toObject(), info);
    //            info.token_with = false;

    //            req->store(info.toQVariantMap());
    //        }
    //        req->pop();
    //        delete res;
    //    });
}

void MainWindow::onEditSearchTextChanged(const QString &text) {
    if (text == "") {
        ui->imagesList->filter(0);
    } else {
        ui->imagesList->filter(
            [=](const QVariantMap &data) -> bool { return data["filename"].toString().contains(text); });
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

void MainWindow::downloadImage(PictureTableLine *obj) {
    NetworkResponseBox *msg = new NetworkResponseBox(this);
    const QVariantMap &data = ui->imagesList->getLineData(obj);

    openMessagegBox(msg);
    msg->setTip(tr("准备下载: ") + data["filename"].toString());
    if (obj->hasCachedData()) {
        msg->setProgress(100);
        msg->setTip(tr("下载完成!"));
        msg->setIcontype(MessageBox::SUCCESS);
        closeMessageBox(msg, 1000);
    } else {
        HttpClient *client = new HttpClient(data["url"].toString(), this);
        msg->setIcontype(MessageBox::WAIT);
        client->downloadFile();
        connect(client, &HttpClient::downloadProgress, this, [=](qint64 a, qint64 b) {
            QStringList tips;
            tips << tr("正在下载: ") << data["filename"].toString() << "\n"
                 << formatFileSize(a) << "/" << formatFileSize(b);
            msg->setTip(tips.join(""));
            msg->setProgress(a / qreal(b) * 1000);
        });
        connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *r) {
            QString saveName = QFileDialog::getSaveFileName(this, "保存图片...", data["filename"].toString());
            QByteArray *d = new QByteArray(r->data);
            msg->setTip(tr("正在保存文件..."));

            QSaveFile *save = new QSaveFile(saveName);
            save->setDirectWriteFallback(true);
            if (save->open(QIODevice::WriteOnly)) {
                save->write(*d);
                if (save->commit()) {
                    msg->setIcontype(MessageBox::SUCCESS);
                    msg->setTip(tr("文件保存成功!"));
                } else {
                    msg->setIcontype(MessageBox::ERROR);
                    msg->setTip(tr("文件保存失败"));
                }
                save->deleteLater();
                delete d;

                closeMessageBox(msg, 1000);
            } else {
                msg->setIcontype(MessageBox::ERROR);
                msg->setTip(tr("文件保存失败"));
                closeMessageBox(msg, 1000);
            }
            delete r;
        });
    }
}

void MainWindow::copyLinks(PictureTableLine *obj) {
    LinkCopyBox *msg = new LinkCopyBox(this);
    const QVariantMap &data = ui->imagesList->getLineData(obj);
    const QString filename = data["filename"].toString();
    const QString url = data["url"].toString();

    openMessagegBox(msg);
    msg->setLink(0, formatExternalLink(filename, url, ExternalLinkType::BBCode));
    msg->setLink(1, formatExternalLink(filename, url, ExternalLinkType::Markdown));
    msg->setLink(2, formatExternalLink(filename, url, ExternalLinkType::HTML));
    msg->setLink(3, formatExternalLink(filename, url, ExternalLinkType::URL));
    msg->setLink(4, data["page"].toString());
    msg->setLink(5, data["delete"].toString());
    connect(msg->dialogButtonBox(), &QDialogButtonBox::accepted, this, [=] { closeMessageBox(msg); });
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

void MainWindow::openMessagegBox(MessageBox *msgBox) {
    maskFrame->layout()->addWidget(msgBox);
    maskFrame->stackUnder(msgBox);
    maskFrame->show();

    QGraphicsOpacityEffect *maskEffect = static_cast<QGraphicsOpacityEffect *>(maskFrame->graphicsEffect());
    maskEffect->setOpacity(0);
    fade(maskEffect, maskFrame)->start(QAbstractAnimation::DeleteWhenStopped);

    msgBox->show();
}

void MainWindow::closeMessageBox(MessageBox *msgBox, int pause) {
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
