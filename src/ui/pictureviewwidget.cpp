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
    imgBox->setStyleSheet("border:10px solid black");

    info->setText(
        "sdaff5sgf4165df \n \
        sadfasdfasdfas \n \
        sadfasdfas \n \
        sadfsadfasdfasdfsadf");
    info->setMaximumWidth(0.2 * width());

    QPixmap p = QPixmap();
    p.loadFromData(ba);
    if (p.width() > width() * 0.8 - 6)
        p = p.scaledToWidth(width() * 0.8 - 6, Qt::SmoothTransformation);
    if (p.height() > height())
        p = p.scaledToWidth(height(), Qt::SmoothTransformation);
    imgBox->setPixmap(p);
}

void PictureViewWidget::setBgColor(const QColor &bgColor) {
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, bgColor);
    this->setPalette(palette);
}

void PictureViewWidget::showEvent(QShowEvent *) {
    if (mainWidget != nullptr) {
        QGraphicsBlurEffect *e = new QGraphicsBlurEffect;
        e->setBlurHints(QGraphicsBlurEffect::QualityHint);
        e->setBlurRadius(10);
        mainWidget->setGraphicsEffect(e);
        setGraphicsEffect(nullptr);
        setFixedSize(qApp->primaryScreen()->availableSize());
    }
}

void PictureViewWidget::hideEvent(QHideEvent *) {
    if (mainWidget != nullptr) {
        mainWidget->setGraphicsEffect(nullptr);
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
