#include "pagebutton.h"

PageButton::PageButton(QWidget *parent) : QPushButton(parent) {
    connect(this, &PageButton::pressed, this, &PageButton::emitClicked);
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
    } else {
        PageButton::setChecked(true);
    }
}

void PageButton::enterEvent(QEvent *) {
    drawPix(ignitedIconPath);
}

void PageButton::leaveEvent(QEvent *) {
    drawPix(normalIconPath);
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
