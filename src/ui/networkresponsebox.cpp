#include "networkresponsebox.h"

#include "base.h"
#include "utils/lotoshelper.h"

using namespace LotosHelper;

NetworkResponseBox::NetworkResponseBox(QWidget *parent) : MessageBox(parent) {
    lab_tip = new QLabel(this);
    lab_tip->setContentsMargins(2, 8, 2, 8);
    lab_tip->setTextInteractionFlags(Qt::TextSelectableByMouse);
    lab_tip->setAlignment(Qt::AlignHCenter | Qt::AlignBaseline);
    QFont font;
    font.setPointSize(14);
    lab_tip->setFont(font);
    lab_tip->setWordWrap(true);

    progress = new QProgressBar(this);
    progress->setGeometry(0, height() - 6, width(), 6);
    progress->setRange(0, 1000);
    progress->setProperty(StyleType.name, StyleType.progressbar.normal);
    progress->setTextVisible(false);

    mainArea()->addWidget(lab_tip, 0, 0);
}

void NetworkResponseBox::setTip(const QString &text) {
    lab_tip->setText(text);
}

void NetworkResponseBox::setProgress(int v) {
    QPropertyAnimation *animation = new QPropertyAnimation(progress, "value", this);
    animation->setStartValue(progress->value());
    animation->setEndValue(v);
    animation->setDuration(150);
    animation->setEasingCurve(QEasingCurve(QEasingCurve::InOutQuad));
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NetworkResponseBox::resizeEvent(QResizeEvent *e) {
    progress->setGeometry(0, height() - 6, width(), 6);
    QFrame::resizeEvent(e);
}
