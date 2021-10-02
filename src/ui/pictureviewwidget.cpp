#include "pictureviewwidget.h"

#include <QScreen>

#include "base.h"
#include "utils/lotoshelper.h"

using namespace LotosHelper;
using namespace LotosAnimation;

PictureViewWidget::PictureViewWidget(QWidget *parent) : QWidget(parent) {
    //    setOpacity(0.05);
    //    setBgColor(QColor("#fff"));

    setAttribute(Qt::WA_TranslucentBackground);
    QVBoxLayout *l = new QVBoxLayout;
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

void PictureViewWidget::showInfo(const QByteArray &ba, QFileInfo i) {
    move(0, 0);
    setFixedSize(qApp->primaryScreen()->availableSize());

    layout()->setContentsMargins(20, 20, 20, 10);
    layout()->setAlignment(imgBox, Qt::AlignHCenter | Qt::AlignVCenter);
    layout()->setAlignment(info, Qt::AlignHCenter | Qt::AlignBottom);

    QPixmap p = QPixmap();
    p.loadFromData(ba);

    info->setMargin(20);
    info->setText(QString(tr("<h3>文件名</h3>\n%1\n"
                             "<h3>文件路径</h3>\n%2\n"
                             "<h3>文件大小</h3>\n%3\n"
                             "<h3>图片尺寸</h3>\n%4 × %5"))
                      .arg(i.fileName())
                      .arg(i.filePath())
                      .arg(formatFileSize(i.size()))
                      .arg(p.width())
                      .arg(p.height()));
    info->setTextInteractionFlags(Qt::TextSelectableByMouse);
    info->adjustSize();
    if (p.height() > height() - 20 - info->height())
        p = p.scaledToHeight(height() - 20 - info->height(), Qt::SmoothTransformation);
    if (p.width() > width())
        p = p.scaledToWidth(width(), Qt::SmoothTransformation);
    imgBox->setPixmap(p);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    shadowGenerator(effect, 0.6, 0, 0, 14);
    info->setGraphicsEffect(effect);
}

void PictureViewWidget::showEvent(QShowEvent *) {}

void PictureViewWidget::hideEvent(QHideEvent *) {}

void PictureViewWidget::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.fillRect(rect(), QColor(0, 0, 0, 255 * 0.4));
    QWidget::paintEvent(event);
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
