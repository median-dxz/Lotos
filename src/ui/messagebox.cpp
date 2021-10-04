#include "messagebox.h"

#include <QBoxLayout>
#include <QDebug>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPainter>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QTimeLine>
#include <QTimer>

#include "utils/lotoshelper.h"
using namespace LotosAnimation;

MessageBox::MessageBox(QWidget *parent) : QFrame(parent) {
    setAttribute(Qt::WA_DeleteOnClose);
    setProperty("class_type", "messagebox");

    OpacityWithShadowEffectsGroup *effect = new OpacityWithShadowEffectsGroup(this);
    shadowGenerator(effect, 0.28, 0, 1, 40);
    effect->setOpacity(1);
    setGraphicsEffect(effect);

    QSizePolicy policy = QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    setSizePolicy(policy);
    setMinimumWidth(400);
    setMaximumWidth(parent->width() / 2);

    fade(effect, this);

    boxLayout = new QVBoxLayout;
    setLayout(boxLayout);

    boxLayout->setAlignment(Qt::AlignHCenter);
    boxLayout->setContentsMargins(24, 30, 24, 30);
    boxLayout->setSpacing(20);

    m_iconArea = new QLabel(this);
    m_iconArea->setFixedHeight(d / 3 * 2);
    boxLayout->addWidget(m_iconArea);

    animation = new QTimeLine(600, this);
    animation->setUpdateInterval(10);
    animation->setFrameRange(0, 360);

    connect(animation, &QTimeLine::frameChanged, this, &MessageBox::paintIcon);

    m_mainArea = new QGridLayout;
    boxLayout->addLayout(m_mainArea);

    m_dialogButtonBox = new QDialogButtonBox(this);
    m_dialogButtonBox->setCenterButtons(true);
    boxLayout->addWidget(m_dialogButtonBox, Qt::AlignHCenter);
}

void MessageBox::paintWar(QPainter *painter, int frame) {
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

    QColor fgC = QColor(250, 206, 168);

    QPen pen;
    pen.setCapStyle(Qt::RoundCap);

    fgC.setAlpha(255 * frame / 360.0);
    pen.setColor(fgC);
    pen.setWidthF(8);
    painter->setPen(pen);

    painter->save();
    painter->translate(0, d / 4);
    QPainterPath path;
    path.addEllipse(-d / 32, -d / 32, d / 16, d / 16);
    painter->fillPath(path, QBrush(fgC));
    painter->restore();

    pen.setWidthF(d / 16);
    painter->setPen(pen);
    painter->drawLine(0, -d / 4, 0, d / 4 - d / 8);
    gradientArc(painter, d / 2, 0, 360, 6, fgC);
}

void MessageBox::paintInf(QPainter *painter, int frame) {
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

    QColor fgC = QColor(157, 224, 246);

    QPen pen;
    pen.setCapStyle(Qt::RoundCap);

    fgC.setAlpha(255 * frame / 360.0);
    pen.setColor(fgC);
    pen.setWidthF(8);
    painter->setPen(pen);

    painter->save();
    painter->translate(0, -d / 4);
    QPainterPath path;
    path.addEllipse(-d / 32, -d / 32, d / 16, d / 16);
    painter->fillPath(path, QBrush(fgC));
    painter->restore();

    pen.setWidthF(d / 16);
    painter->setPen(pen);
    painter->drawLine(0, -d / 4 + d / 8, 0, d / 4);
    gradientArc(painter, d / 2, 0, 360, 6, fgC);
}

void MessageBox::paintWai(QPainter *painter, int frame) {
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter->rotate(-frame);

    QColor fgC = QColor(0x40, 0x9e, 0xff);
    QColor bgC = QColor(0xec, 0xf5, 0xff);

    int t = animation->currentTime();
    qreal v = animation->valueForTime(t);
    int prev = t + 360 * 7 * 4 * (-(v - 0.5) * (v - 0.5) + 0.25);
    int len = animation->frameForTime(prev) - frame;
    if (prev >= 360 * 7) {
        len = (270 - (frame - 90));
    }

    gradientArc(painter, d / 2, 0, 360, 6, bgC);

    gradientArc(painter, d / 2, -90 + frame, len, 6, fgC);
}

void MessageBox::paintErr(QPainter *painter, int frame) {
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

    QColor fgC = QColor(242, 116, 116);
    QPen pen;
    pen.setCapStyle(Qt::RoundCap);

    fgC.setAlpha(255 * frame / 360.0);
    pen.setColor(fgC);
    pen.setWidthF(6);
    painter->setPen(pen);

    if (frame < d)
        painter->drawEllipse(QRect(-d / 2, -(frame / 2.0), d, frame));
    else {
        painter->drawEllipse(QRect(-d / 2, -d / 2, d, d));
    }

    pen.setWidthF(8);
    if (frame >= 60 && frame <= 270) {
        qreal progress = frame / 270.0;
        int marginTop = d / 4 * (1 - progress);
        qreal scaleHint = 0.4 + 0.6 * (frame / 270.0);

        fgC.setAlpha(255 * (frame - 60.0) / (270 - 60));
        pen.setColor(fgC);

        painter->save();
        painter->setPen(pen);
        painter->translate(QPointF(0, marginTop));
        painter->scale(scaleHint, scaleHint);

        painter->rotate(45 * progress);
        painter->drawLine(-d / 4.0, 0, d / 4.0, 0);
        painter->rotate(-90 * progress);
        painter->drawLine(-d / 4.0, 0, d / 4.0, 0);

        painter->restore();
    } else if (frame > 270) {
        painter->setPen(pen);
        painter->save();
        painter->rotate(45);
        painter->drawLine(-d / 4.0, 0, d / 4.0, 0);

        painter->rotate(-90);
        painter->drawLine(-d / 4.0, 0, d / 4.0, 0);
        painter->restore();
    }
}

void MessageBox::paintSuc(QPainter *painter, int frame) {
    QColor fgC = QColor(165, 220, 134);
    QColor bgC = QColor(230, 245, 221);
    QPen pen(fgC, 8);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

    int traceSec = frame - 250;
    if (frame > 250) {
        if (traceSec < 50) {
            painter->drawLine(-70, -25, -70 + traceSec, traceSec - 25);
        } else if (traceSec >= 50) {
            if (traceSec < 95) {
                painter->drawLine(-70, -25, -45, 0);
            } else {
                int j = traceSec - 95;
                if (frame <= 360)
                    painter->drawLine(-45 + j, 0 + j, -45, 0);
            }
            painter->drawLine(-45, 0, -15, 30);
            if (frame <= 375) {
                painter->drawLine(-15, 30, -15 + traceSec - 50, 30 - traceSec + 50);
            } else {
                painter->drawLine(-15, 30, -15 + 125 - 50, 30 - 125 + 50);
            }
        }
    }

    gradientArc(painter, d / 2, 0, 360, 6, bgC);  //这个是画圆环的
    gradientArc(painter, d / 2, 270, frame, 6, fgC);

    if (frame >= 250) {
        gradientArc(painter, d / 2, 270, traceSec * 220.0 / 95, 6, bgC);
        gradientArc(painter, d / 2, 162, 110, 6, bgC);
    }
}

void MessageBox::paintIcon(int frame) {
    QPixmap pix(d + 8, d + 8);
    QPainter *painter = new QPainter(&pix);
    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);

    painter->fillRect(pix.rect(), QColor(255, 255, 255));
    painter->translate(pix.width() >> 1, pix.height() >> 1);

    switch (m_icontype) {
        case SUCCESS:
            paintSuc(painter, frame);
            break;
        case ERROR:
            paintErr(painter, frame);
            break;
        case WAIT:
            paintWai(painter, frame);
            break;
        case INFO:
            paintInf(painter, frame);
            break;
        case WARN:
            paintWar(painter, frame);
            break;
        default:
            break;
    }

    QPixmap scaled = pix.scaledToHeight(m_iconArea->height(), Qt::SmoothTransformation);
    painter->end();

    QPixmap labelResized(m_iconArea->size());
    painter->begin(&labelResized);

    painter->fillRect(labelResized.rect(), QColor(255, 255, 255));
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter->drawPixmap(QRect(QPoint((labelResized.width() >> 1) - (scaled.width() >> 1), 0), scaled.size()), scaled);

    painter->end();

    m_iconArea->setPixmap(labelResized);
}

MessageBox::IconType MessageBox::icontype() const {
    return m_icontype;
}

void MessageBox::setIcontype(const IconType &icontype) {
    m_icontype = icontype;
    if (icontype == WAIT) {
        animation->setLoopCount(0);
        animation->setDuration(360 * 7);
        animation->setEasingCurve(QEasingCurve(QEasingCurve::Linear));
    } else {
        animation->setLoopCount(1);
        animation->setDuration(600);
        animation->setEasingCurve(QEasingCurve(QEasingCurve::OutQuad));
    }
    animation->setCurrentTime(0);
    animation->stop();
    animation->start();
}

QDialogButtonBox *MessageBox::dialogButtonBox() const {
    return m_dialogButtonBox;
}

void MessageBox::setDialogButtonBox(QDialogButtonBox *dialogButtonBox) {
    m_dialogButtonBox = dialogButtonBox;
}

QGridLayout *MessageBox::mainArea() const {
    return m_mainArea;
}

void MessageBox::setMainArea(QGridLayout *mainArea) {
    m_mainArea = mainArea;
}

void MessageBox::gradientArc(QPainter *painter,
                             int radius,
                             int startAngle,
                             int angleLength,
                             int arcHeight,
                             QColor color) {
    painter->setBrush(QColor(color));
    QRectF rect(-radius, -radius, radius << 1, radius << 1);
    QPainterPath path;
    path.arcTo(rect, startAngle, angleLength);
    QPainterPath subPath;
    subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));

    // path为扇形 subPath为椭圆
    path -= subPath;

    painter->setPen(Qt::NoPen);
    painter->drawPath(path);
}
