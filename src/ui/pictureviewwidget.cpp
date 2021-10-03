#include "pictureviewwidget.h"

#include <QScreen>

#include "base.h"
#include "utils/lotoshelper.h"
#include "utils/promise.h"

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

    move(0, 0);
    setFixedSize(qApp->primaryScreen()->availableSize());

    layout()->setContentsMargins(20, 20, 20, 10);
    layout()->setAlignment(imgBox, Qt::AlignHCenter | Qt::AlignVCenter);
    layout()->setAlignment(info, Qt::AlignHCenter | Qt::AlignBottom);

    info->setMargin(20);
    info->setTextInteractionFlags(Qt::TextSelectableByMouse);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    shadowGenerator(effect, 0.6, 0, 0, 14);
    info->setGraphicsEffect(effect);
}

PictureViewWidget &PictureViewWidget::Instance() {
    static PictureViewWidget instance;
    return instance;
}

void PictureViewWidget::showInfo(showManager display) {
    info->setText("");
    imgBox->setPixmap(QPixmap());

    index = display(this, imgBox, info);
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

void PictureViewWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        hide();
    }
}
bool PictureViewWidget::eventFilter(QObject *obj, QEvent *event) {
    if (obj == this && event->type() == QEvent::WindowDeactivate) {
        hide();
    }
    return QWidget::eventFilter(obj, event);
}
