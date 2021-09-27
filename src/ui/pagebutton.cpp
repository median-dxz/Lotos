#include "pagebutton.h"

#include <QStyle>
#include "base.h"

PageButton::PageButton(QWidget *parent) : QPushButton(parent) {
    iconLabel = new QLabel(this);
    connect(this, &PageButton::pressed, this, &PageButton::emitClicked);
    installEventFilter(this);
}

void PageButton::emitClicked() {
    if (!PageButton::isChecked()) {
        PageButton::setChecked(true);
        emit indexChanged(index);
    }
}

void PageButton::setCurrentChosen(int index) {
    if (this->index != index) {
        PageButton::setChecked(false);
        drawPix(normalIconPath);
    } else {
        PageButton::setChecked(true);
        drawPix(ignitedIconPath);
    }
}

void PageButton::enterEvent(QEvent *) {
    drawPix(ignitedIconPath);
    QPropertyAnimation *animation = new QPropertyAnimation(this, "bgC", this);
    animation->setStartValue(0);
    animation->setEndValue(255 * 0.1);
    animation->setDuration(150);
    animation->start();
}

void PageButton::leaveEvent(QEvent *) {
    if (!isChecked()) {
        drawPix(normalIconPath);
    }
    QPropertyAnimation *animation = new QPropertyAnimation(this, "bgC", this);
    animation->setStartValue(255 * 0.1);
    animation->setEndValue(0);
    animation->setDuration(150);
    animation->start();
}

bool PageButton::eventFilter(QObject *watched, QEvent *event) {
    if (watched == this && event->type() == QEvent::Paint) {
        QPainter p(this);
        p.fillRect(rect(), QBrush(QColor(0, 0, 0, bgC)));
        p.end();
        return false;
    }
    return QPushButton::eventFilter(watched, event);
}

int PageButton::getBgC() const {
    return bgC;
}

void PageButton::setBgC(const int &value) {
    bgC = value;
    update();
}

void PageButton::setIconPath(QString pathNormal, QString pathIgnited) {
    normalIconPath = pathNormal;
    ignitedIconPath = pathIgnited;
}

void PageButton::drawPix(const QString iconPath) {
    iconLabel->setPixmap(iconPath);
    iconLabel->setScaledContents(true);
    iconLabel->resize(iconSize());
    iconLabel->move(iconOffset);
}

QString PageButton::getNormalIconPath() const {
    return normalIconPath;
}

QString PageButton::getIgnitedIconPath() const {
    return ignitedIconPath;
}

void PageButton::setIndex(int index) {
    this->index = index;
}

void PageButton::setIconOffset(int x, int y) {
    iconOffset = QPoint(x, y);
}

int PageButton::Index() const {
    return index;
}

bool PageButton::isChecked() const {
    return chosen;
}

void PageButton::setChecked(bool c) {
    chosen = c;
    if (chosen) {
        setProperty("class_type", "pagebutton.checked");
    } else {
        setProperty("class_type", "pagebutton.unchecked");
    }
    style()->unpolish(this);
    style()->polish(this);
}
