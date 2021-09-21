#include "iconwidget.h"

#include <QDebug>

IconWidget::IconWidget(QWidget *parent) : QWidget(parent) {
    infoBox = new QVBoxLayout(this);
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
    setInfo();

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
    shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(0, 2);
    shadow->setColor(QColor(0, 0, 0, 255 * 0.08));
    shadow->setBlurRadius(20);
    this->setGraphicsEffect(shadow);
}

void IconWidget::setInfo() {
    QProgressBar *progress = new QProgressBar(this);
    QLabel *nameLine = new QLabel(this);
    QLabel *sizeLine = new QLabel(this);
    QLabel *statusLine = new QLabel(this);

    infoBox->addWidget(progress);
    infoBox->addWidget(nameLine);
    infoBox->addWidget(sizeLine);
    infoBox->addLayout(bottomLine);
    bottomLine->addWidget(statusLine);
    bottomLine->addStretch();
    //    painter.setFont(QFont("Microsoft YaHei", 12));
    //    painter.setPen(QColor("#606266"));

    //    QString text;
    //    text = (info.baseName().size() > 16 ? info.baseName().left(16) + "..." : info.baseName());

    //    painter.drawText(QRect((width() - painter.fontMetrics().width(text)) / 2, height() - INFO_SPACE, width(), 40),
    //                     text);

    //    text =
    //        "\n" + QString::number(info.size() / pow(2, int(log2(info.size())) / 10 * 10), 'f', 2) +
    //        sizeUnit(info.size());
    //    painter.drawText(QRect((width() - painter.fontMetrics().width(text)) / 2, height() - INFO_SPACE, width(), 40),
    //                     text);
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

QString IconWidget::sizeUnit(qint64 size) {
    enum SIZE_TYPE { B, KB, MB } type;
    type = SIZE_TYPE(int(log2(size) / 10));
    switch (type) {
        case B:
            return "B";
        case KB:
            return "KB";
        case MB:
            return "MB";
        default:
            break;
    }
    return "";
}

void IconWidget::setImage(QImage img) {
    pix = img;
    thumb = pix.scaled(width(), height() - INFO_SPACE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

QImage IconWidget::image() const {
    return pix;
}

void IconWidget::setImageData(QByteArray ba) {
    data = ba;
}

QByteArray &IconWidget::imageData() {
    return data;
}

void IconWidget::setImageInfo(QFileInfo fi) {
    info = fi;
}

QFileInfo IconWidget::imageInfo() const {
    return info;
}
