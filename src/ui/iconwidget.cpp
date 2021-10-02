#include "iconwidget.h"

#include <QtConcurrent>
#include "base.h"
#include "utils/lotoshelper.h"

using namespace LotosHelper;
using namespace LotosAnimation;

IconWidget::IconWidget(QWidget *parent) : QWidget(parent) {
    infoBox = new QVBoxLayout;
    bottomLine = new QHBoxLayout;

    infoBox->setContentsMargins(8, 8, 8, 8);
    infoBox->setSpacing(8);

    bottomLine->setContentsMargins(0, 0, 0, 0);
    bottomLine->setSpacing(8);

    this->setLayout(infoBox);
}

void IconWidget::addImageFromFile(const QString &fileName, const QByteArray &fileData) {
    auto func = std::bind(&IconWidget::setImage, this, fileData);

    QFuture<void> future;
    future = (QtConcurrent::run(func));

    setImageInfo(QFileInfo(fileName));

    setShadow();

    infoBox->setAlignment(Qt::AlignBottom);
    infoBox->addSpacing(height() - INFO_SPACE);

    setInfo();
    setUploadBtn();
    setViewBtn();
    setDeleteBtn();

    connect(this, &IconWidget::statusChanged, this, &IconWidget::onStatusChanged);
    setStatus(IconWidget::PENDING);
}

void IconWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform |
                           QPainter::TextAntialiasing);
    painter.setPen(QColor("#fff"));
    painter.fillRect(0, 0, width() - painter.pen().width(), height() - painter.pen().width(), QBrush(QColor("#fff")));

    QRect rect;

    if (!thumb.isNull()) {
        rect.setRect((width() - thumb.width()) / 2, 0, thumb.width(), thumb.height());
        painter.drawImage(rect, thumb);
    }

    painter.setPen(QColor("#ebeef5"));
    painter.drawRect(0, 0, width() - painter.pen().width(), height() - painter.pen().width());
}

void IconWidget::enterEvent(QEvent *) {
    hover = true;
    QPropertyAnimation *enterAnimation = alphaGradient(shadow, "color", QColor(0, 0, 0), 0.08, 0.18, this);
    enterAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void IconWidget::leaveEvent(QEvent *) {
    hover = false;
    QPropertyAnimation *leaveAnimation = alphaGradient(shadow, "color", QColor(0, 0, 0), 0.08, 0.18, this, 200, false);
    leaveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void IconWidget::setShadow() {
    shadow = new QGraphicsDropShadowEffect(this);
    shadowGenerator(shadow, 0.08, 0, 2, 20);
    this->setGraphicsEffect(shadow);
}

void IconWidget::setInfo() {
    progress = new QProgressBar(this);
    QLabel *nameLine = new QLabel(this);
    QLabel *sizeLine = new QLabel(this);
    statusLine = new QLabel(this);

    infoBox->addWidget(progress);
    infoBox->addWidget(nameLine);
    infoBox->addWidget(sizeLine);
    infoBox->addLayout(bottomLine);
    bottomLine->addWidget(statusLine);
    bottomLine->addStretch();

    progress->setRange(0, 100);
    progress->setProperty("class_type", "iconwidget.sub.progress");
    progress->setProperty(StyleType.name, StyleType.progressbar.normal);
    progress->setFixedHeight(9);
    progress->setFormat("");

    nameLine->setAlignment(Qt::AlignHCenter);
    nameLine->setText(getElidedText(nameLine->font(), info.fileName(), nameLine->width()));
    nameLine->setToolTip(info.fileName());

    sizeLine->setAlignment(Qt::AlignHCenter);
    sizeLine->setText(formatFileSize(info.size()));
}

void IconWidget::setViewBtn() {
    QPushButton *btn = new QPushButton(this);
    bottomLine->addWidget(btn);
    btn->setFixedSize(QSize(32, 32));

    QIcon ico = QIcon();
    ico.addFile(":/res/icons/view.png", QSize(24, 24));

    btn->setIcon(ico);
    btn->setProperty("class_type", "iconwidget.sub.btn");
    btn->setProperty(StyleType.name, StyleType.button.normal);
    connect(btn, &QPushButton::clicked, this, [&] { emit onViewBtnClicked(this); });
}

void IconWidget::setDeleteBtn() {
    QPushButton *btn = new QPushButton(this);
    bottomLine->addWidget(btn);
    btn->setFixedSize(QSize(32, 32));

    QIcon ico = QIcon();
    ico.addFile(":/res/icons/del.png", QSize(24, 24), QIcon::Normal);

    btn->setIcon(ico);
    btn->setProperty("class_type", "iconwidget.sub.btn");
    btn->setProperty(StyleType.name, StyleType.button.danger);
    connect(btn, &QPushButton::clicked, this, [&] { emit onDeleteBtnClicked(this); });
}

void IconWidget::setUploadBtn() {
    QPushButton *btn = new QPushButton(this);
    bottomLine->addWidget(btn);
    btn->setFixedSize(QSize(32, 32));

    QIcon ico = QIcon();
    ico.addFile(":/res/icons/upload.png", QSize(24, 24), QIcon::Normal);

    btn->setIcon(ico);
    btn->setProperty("class_type", "iconwidget.sub.btn");
    btn->setProperty(StyleType.name, StyleType.button.normal);
    connect(btn, &QPushButton::clicked, this, [&] { emit onUploadBtnClicked(this); });
    uploadBtn = btn;
}

void IconWidget::onStatusChanged(IconWidget::UPLOAD_STATUS newStatus) {
    QString statusStr = QString("<h4>%1:</h4> %2");
    switch (newStatus) {
        case FAILED:
            statusStr = statusStr.arg(tr("状态"), tr("上传失败"));
            uploadBtn->setDisabled(false);
            break;
        case PENDING:
            statusStr = statusStr.arg(tr("状态"), tr("等待上传"));
            uploadBtn->setDisabled(false);
            break;
        case UPLOADED:
            statusStr = statusStr.arg(tr("状态"), tr("已上传"));
            uploadBtn->setDisabled(true);
            break;
        case UPLOADING:
            statusStr = statusStr.arg(tr("状态"), tr("上传中"));
            uploadBtn->setDisabled(true);
            break;
    }
    statusLine->setText(statusStr);
    m_status = newStatus;
}

void IconWidget::updateUploadProgress(qint64 bytesSent, qint64 bytesTotal) {
    if (bytesSent != 0 || bytesTotal != 0) {
        progress->setValue((double(bytesSent) / bytesTotal) * 100);
    }
}
