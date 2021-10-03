#include "notification.h"

#include "base.h"
#include "utils/lotoshelper.h"

using namespace LotosHelper;
using namespace LotosAnimation;

Notification::Notification(QWidget *p, QString tl, QString mes, QString bc, QString fc, int pos) : QFrame(p) {
    setAttribute(Qt::WA_DeleteOnClose);

    move(p->width(), pos);
    setFixedWidth(FIXED_WIDTH);
    setMinimumHeight(MIN_HEIGHT);

    setLayout(new QVBoxLayout);

    title = new QLabel(this);
    message = new QLabel(this);

    QFont font;
    font.setStyleStrategy(QFont::PreferAntialias);

    font.setPixelSize(16);
    font.setWeight(QFont::Bold);
    title->setFont(font);
    title->setText(tl);

    font.setPixelSize(14);
    font.setWeight(QFont::Normal);
    message->setFont(font);
    message->setWordWrap(true);
    message->setText(mes);

    layout()->setSpacing(0);
    layout()->setContentsMargins(14, 14, 14, 14);

    layout()->addWidget(title);
    static_cast<QVBoxLayout *>(layout())->addSpacing(8);
    layout()->addWidget(message);

    effect = new OpacityWithShadowEffectsGroup(this);
    shadowGenerator(effect, 0.18, 0, 2, 20);
    effect->setOpacity(0);
    setGraphicsEffect(effect);

    setStyleSheet(QString(".Notification{ "
                          "border-radius: 8px;"
                          "background: %1;"
                          "}"
                          ".Notification QLabel{"
                          "color: %2;"
                          "}")
                      .arg(bc, fc));

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

    QPropertyAnimation *a_opa_in = fade(effect, this, 300);
    QPropertyAnimation *a_opa_out = fade(effect, this, 150, false);

    QTimeLine *a_geo_left_in = new QTimeLine(400, this);
    a_geo_left_in->setFrameRange(x(), x() - FIXED_WIDTH - GAP_SPACE);
    a_geo_left_in->setUpdateInterval(16);

    a_geo_up = new QTimeLine(150, this);
    a_geo_up->setUpdateInterval(16);

    connect(a_geo_left_in, &QTimeLine::frameChanged, this, [=](int frameValue) { move(frameValue, y()); });
    connect(a_geo_up, &QTimeLine::frameChanged, this, [=](int frameValue) { move(x(), frameValue); });

    a_geo_left_in->start();

    a_opa_in->setEasingCurve(QEasingCurve(QEasingCurve::OutInQuad));

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

NotificationManager::NotificationManager(QObject *parent) : QObject(parent) {}

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
        notifications.removeOne(orientedNotification);
        int top = Notification::GAP_SPACE;
        for (auto notification : qAsConst(notifications)) {
            if (notification->y() > top) {
                notification->a_geo_up->stop();
                notification->a_geo_up->setFrameRange(notification->y(), top);
                notification->a_geo_up->start();
            }
            top += Notification::GAP_SPACE + notification->height();
        }
    });
}

void NotificationManager::newNotify(NOTIFY_FACTORY::parac c, NOTIFY_FACTORY::parat t) {
    newNotify(p, t.title, t.message, c.bgC, c.fgC);
}

void NotificationManager::setNotifyParent(QWidget *value) {
    p = value;
}

const NOTIFYS::parac NOTIFYS::INFO{"#edf2fc", "#909399"};
const NOTIFYS::parac NOTIFYS::ERROR{"#fef0f0", "#f56c6c"};
const NOTIFYS::parac NOTIFYS::SUCCESS{"#f0f9eb", "#67c23a"};
const NOTIFYS::parac NOTIFYS::WARN{"#fdf6ec", "#e6a23c"};
