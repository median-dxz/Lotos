#include "iconwidget.h"

#include "base.h"
#include "utils/lotoshelper.h"

IconWidget::IconWidget(QWidget *parent) : QWidget(parent) {
    infoBox = new QVBoxLayout;
    bottomLine = new QHBoxLayout;

    infoBox->setContentsMargins(8, 8, 8, 8);
    infoBox->setSpacing(8);
    bottomLine->setContentsMargins(0, 0, 0, 0);
    bottomLine->setSpacing(8);

    this->setLayout(infoBox);
}

void IconWidget::addImageFromFile(const QString &fileName) {
    setImage(QImage(fileName));
    setImageInfo(QFileInfo(fileName));
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    setImageData(QByteArray(file.readAll()));

    setShadow();

    infoBox->setAlignment(Qt::AlignBottom);
    infoBox->addSpacing(height() - INFO_SPACE);
    connect(this, &IconWidget::statusChanged, this, &IconWidget::onStatusChanged);
    setInfo();

    setUploadBtn();
    setViewBtn();
    setDeleteBtn();
    update();
}

void IconWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform |
                           QPainter::TextAntialiasing);
    painter.setPen(QColor("#fff"));
    painter.fillRect(0, 0, width() - painter.pen().width(), height() - painter.pen().width(), QBrush(QColor("#fff")));

    QRect rect;

    rect.setRect((width() - thumb.width()) / 2, 0, thumb.width(), thumb.height());
    painter.drawImage(rect, thumb);

    painter.setPen(QColor("#ebeef5"));
    painter.drawRect(0, 0, width() - painter.pen().width(), height() - painter.pen().width());
}

void IconWidget::enterEvent(QEvent *) {
    hover = true;
    QPropertyAnimation *EnterAnimation = new QPropertyAnimation(shadow, "color", this);

    EnterAnimation->setDuration(200);
    EnterAnimation->setStartValue(QColor(0, 0, 0, 255 * 0.08));
    EnterAnimation->setEndValue(QColor(0, 0, 0, 255 * 0.18));
    EnterAnimation->start();
}

void IconWidget::leaveEvent(QEvent *) {
    hover = false;
    QPropertyAnimation *EnterAnimation = new QPropertyAnimation(shadow, "color", this);

    EnterAnimation->setDuration(200);
    EnterAnimation->setStartValue(QColor(0, 0, 0, 255 * 0.18));
    EnterAnimation->setEndValue(QColor(0, 0, 0, 255 * 0.08));
    EnterAnimation->start();
}

void IconWidget::setShadow() {
    shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 2);
    shadow->setColor(QColor(0, 0, 0, 255 * 0.08));
    shadow->setBlurRadius(20);
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
    progress->setFixedHeight(9);

    nameLine->setAlignment(Qt::AlignHCenter);
    nameLine->setText(info.completeBaseName() + "." + info.completeSuffix());

    sizeLine->setAlignment(Qt::AlignHCenter);
    sizeLine->setText(formatFileSize(info.size()));
    setStatus(IconWidget::PENDING);
}

void IconWidget::setViewBtn() {
    QPushButton *btn = new QPushButton(this);
    bottomLine->addWidget(btn);
    btn->setFixedSize(QSize(32, 32));

    QIcon ico = QIcon();
    ico.addFile(":/res/icons/view.png", QSize(24, 24));

    btn->setIcon(ico);
    btn->setProperty("class_type", "iconwidget.sub.view");
    connect(btn, &QPushButton::clicked, this, [&] { emit onViewBtnClicked(this); });
}

void IconWidget::setDeleteBtn() {
    QPushButton *btn = new QPushButton(this);
    bottomLine->addWidget(btn);
    btn->setFixedSize(QSize(32, 32));

    QIcon ico = QIcon();
    ico.addFile(":/res/icons/del.png", QSize(24, 24), QIcon::Normal);

    btn->setIcon(ico);
    btn->setProperty("class_type", "iconwidget.sub.del");
    connect(btn, &QPushButton::clicked, this, [&] { emit onDeleteBtnClicked(this); });
}

void IconWidget::setUploadBtn() {
    QPushButton *btn = new QPushButton(this);
    bottomLine->addWidget(btn);
    btn->setFixedSize(QSize(32, 32));

    QIcon ico = QIcon();
    ico.addFile(":/res/icons/upload.png", QSize(24, 24), QIcon::Normal);

    btn->setIcon(ico);
    btn->setProperty("class_type", "iconwidget.sub.upload");
    connect(btn, &QPushButton::clicked, this, [&] { emit onUploadBtnClicked(this); });
}

void IconWidget::onStatusChanged(IconWidget::UPLOAD_STATUS newStatus) {
    QString statusStr = QString("<h4>%1:</h4> %2");
    switch (newStatus) {
        case FAILED:
            statusStr = statusStr.arg(tr("状态"), tr("上传失败"));
            break;
        case PENDING:
            statusStr = statusStr.arg(tr("状态"), tr("等待上传"));
            break;
        case UPLOADED:
            statusStr = statusStr.arg(tr("状态"), tr("已上传"));
            break;
        case UPLOADING:
            statusStr = statusStr.arg(tr("状态"), tr("上传中"));
            break;
    }
    statusLine->setText(statusStr);
    m_status = newStatus;
}

void IconWidget::updateUploadProgress(qint64 bytesSent, qint64 bytesTotal) {
    if (bytesSent != 0 || bytesTotal != 0) {
        progress->setFormat(formatFileSize(bytesSent) + "/" + formatFileSize(bytesTotal));
        progress->setValue((double(bytesSent) / bytesTotal) * 100);
    }
}
