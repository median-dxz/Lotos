#include "pictureviewwidget.h"

#include <QDebug>

PictureViewWidget::PictureViewWidget(QWidget *parent) : QWidget(parent) {
    mainWidget = 0;
    //    setOpacity(0.05);
    //    setBgColor(QColor("#fff"));

    setAttribute(Qt::WA_TranslucentBackground);
    QVBoxLayout *l = new QVBoxLayout(this);
    setLayout(l);
    imgBox = new QLabel(this);
    info = new QLabel(this);
    l->addStretch();
    l->addWidget(imgBox);
    l->addSpacing(40);
    l->addWidget(info);
    l->addStretch();

    this->installEventFilter(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
}

PictureViewWidget &PictureViewWidget::Instance() {
    static PictureViewWidget instance;
    return instance;
}

void PictureViewWidget::setMainWidget(QWidget *mainWidget) {
    this->mainWidget = mainWidget;
    move(0, 0);
}

void PictureViewWidget::setOpacity(double opacity) {
    this->setWindowOpacity(opacity);
}

void PictureViewWidget::showInfo(QByteArray &ba, QFileInfo i) {
    layout()->setContentsMargins(20, 20, 20, 20);
    layout()->setAlignment(imgBox, Qt::AlignHCenter | Qt::AlignVCenter);
    layout()->setAlignment(info, Qt::AlignHCenter | Qt::AlignBottom);

    QPixmap p = QPixmap();
    p.loadFromData(ba);
    info->setText(QString(tr("<h3>文件名</h3>\n%1\n"
                             "<h3>文件路径</h3>\n%2\n"
                             "<h3>文件大小</h3>\n%3\n"
                             "<h3>图片尺寸</h3>\n%4 × %5"))
                      .arg(i.fileName())
                      .arg(i.filePath())
                      .arg(QString::number(i.size() / pow(2, int(log2(i.size())) / 10 * 10), 'f', 2) +
                           IconWidget::sizeUnit(i.size()))
                      .arg(p.width())
                      .arg(p.height()));

    if (p.width() > width() * 0.8 - 6)
        p = p.scaledToWidth(width() * 0.8 - 6, Qt::SmoothTransformation);
    if (p.height() > height())
        p = p.scaledToWidth(height(), Qt::SmoothTransformation);
    imgBox->setPixmap(p);

    info->setMinimumWidth(0.2 * width());
}

void PictureViewWidget::setBgColor(const QColor &bgColor) {
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, bgColor);
    this->setPalette(palette);
}

void PictureViewWidget::showEvent(QShowEvent *) {
    if (mainWidget != nullptr) {
        setFixedSize(qApp->primaryScreen()->availableSize());
    }
}

void PictureViewWidget::hideEvent(QHideEvent *) {
    if (mainWidget != nullptr) {
    }
}

void PictureViewWidget::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.fillRect(rect(), QColor(0, 0, 0, 255 * 0.4));
}

void PictureViewWidget::mousePressEvent(QMouseEvent *event) {
    if (imgBox->geometry().contains(event->pos())) {
        qDebug() << "imgbox";
    } else if (info->geometry().contains(event->pos())) {
        qDebug() << "info";
    } else {
        hide();
    }
}
bool PictureViewWidget::eventFilter(QObject *obj, QEvent *event) {
    if (obj == this && event->type() == QEvent::WindowDeactivate) {
        hide();
    }
    return QWidget::eventFilter(obj, event);
}
