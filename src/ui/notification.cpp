#include "notification.h"

#include "base.h"
#include "utils/lotoshelper.h"

using namespace LotosHelper;

Notification::Notification(QWidget *p, QString tl, QString mes, QString bc, QString fc, int pos) : QFrame(p) {
    setAttribute(Qt::WA_DeleteOnClose);

    move(p->width(), pos);
    setFixedWidth(FIXED_WIDTH);
    setMinimumHeight(MIN_HEIGHT);

    setLayout(new QVBoxLayout);

    title = new QLabel(this);
    message = new QLabel(this);

    QFont font("Microsoft YaHei");
    font.setStyleStrategy(QFont::PreferAntialias);

    font.setPixelSize(16);
    font.setWeight(QFont::Bold);
    title->setFont(font);
    title->setText(tl);

    font.setPixelSize(14);
    font.setWeight(QFont::Normal);
    message->setFont(font);
    message->setText(mes);

    layout()->setSpacing(0);
    layout()->setContentsMargins(14, 14, 14, 14);

    layout()->addWidget(title);
    static_cast<QVBoxLayout *>(layout())->addSpacing(8);
    layout()->addWidget(message);

    effect = new OpacityWithShadowEffectsGroup(this);
    effect->setOffset(QPoint(0, 2));
    effect->setBlurRadius(20);
    effect->setColor(QColor(0, 0, 0, 255 * 0.18));
    effect->setOpacity(0);
    setGraphicsEffect(effect);

    setStyleSheet(QString(".Notification{ "
                          "border-radius: 8px;"
                          "background: %1;"
                          "}"
                          ".Notification QLabel{"
                          "color: %2;"
                          "}")
                      .arg(bc)
                      .arg(fc));

    adjustSize();
    show();
    animation();
}

void Notification::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    QFrame::paintEvent(e);
}

void Notification::animation() {
    QSequentialAnimationGroup *sequence = new QSequentialAnimationGroup(this);

    QPropertyAnimation *a_opa_in = new QPropertyAnimation(effect, "opacity");
    QPropertyAnimation *a_opa_out = new QPropertyAnimation(effect, "opacity");

    QTimeLine *a_geo_left_in = new QTimeLine(400, this);
    a_geo_left_in->setFrameRange(x(), x() - FIXED_WIDTH - GAP_SPACE);
    a_geo_left_in->setUpdateInterval(10);

    connect(a_geo_left_in, &QTimeLine::frameChanged, this, [=](int frameValue) { move(frameValue, y()); });

    a_geo_left_in->start();

    a_opa_in->setStartValue(0);
    a_opa_in->setEndValue(1);
    a_opa_in->setDuration(300);
    a_opa_in->setEasingCurve(QEasingCurve(QEasingCurve::InOutQuad));

    a_opa_out->setStartValue(1);
    a_opa_out->setEndValue(0);
    a_opa_out->setDuration(150);
    a_opa_out->setEasingCurve(QEasingCurve(QEasingCurve::InOutQuad));

    sequence->addAnimation(a_opa_in);
    sequence->addPause(NOTIFY_DURATION);
    sequence->addAnimation(a_opa_out);

    sequence->start(QAbstractAnimation::DeleteWhenStopped);

    connect(sequence, &QAbstractAnimation::finished, this, &Notification::close);
    //    connect(sequence, &QAbstractAnimation::finished, this, [=] { emit beforeClose(this); });
    connect(sequence, &QSequentialAnimationGroup::currentAnimationChanged, this, [=](QAbstractAnimation *current) {
        if (static_cast<QPropertyAnimation *>(current) == a_opa_out) {
            emit beforeClose(this);
        }
    });
}

NotificationManager::NotificationManager(QObject *parent) : QObject(parent) {
    fixTop = new QTimer(this);
    fixTop->setInterval(10);
    fixTop->start();
    connect(fixTop, &QTimer::timeout, this, [=] {
        int top = Notification::GAP_SPACE;
        for (auto notification : qAsConst(notifications)) {
            if (notification->y() > top) {
                notification->move(notification->x(), notification->y() - 8);
            } else {
                notification->move(notification->x(), top);
            }
            top += Notification::GAP_SPACE + notification->height();
        }
    });
}

NotificationManager &NotificationManager::Instance() {
    static NotificationManager instance;
    return instance;
}

void NotificationManager::newNotify(QWidget *parent, QString title, QString message, QString bgC, QString fgC) {
    int newTop = Notification::GAP_SPACE;
    for (auto notification : qAsConst(notifications)) {
        newTop += Notification::GAP_SPACE + notification->height();
    }
    Notification *notification = new Notification(parent, title, message, bgC, fgC, newTop);
    notifications.append(notification);
    connect(notification, &Notification::beforeClose, this, [=](Notification *orientedNotification) {
        QList<Notification *>::iterator del_iter;
        for (QList<Notification *>::iterator iter = notifications.begin(); iter != notifications.end(); iter++) {
            if (*iter == orientedNotification) {
                del_iter = iter;
            }
        }
        notifications.erase(del_iter);
    });
}
