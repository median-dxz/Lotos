#include "ImgButton.h"

ImgButton::ImgButton(QWidget *parent) : QPushButton(parent) {}

ImgButton::ImgButton(QString normal, QString hover, QString press) {
    setMouseTracking(true);
    normalIconPath = normal;
    pressIconPath = press;
    hoverIconPath = hover;

    QSize iconSize = QSize(32, 32);
    setFixedSize(iconSize);
    setStyleSheet("QPushButton{border:none;}");
    DrawIcon(normalIconPath);

    setIconSize(iconSize);
}

void ImgButton::mouseReleaseEvent(QMouseEvent *e) {
    DrawIcon(hoverIconPath);

    QPushButton::mouseReleaseEvent(e);
}

void ImgButton::mousePressEvent(QMouseEvent *e) {
    DrawIcon(pressIconPath);

    QPushButton::mousePressEvent(e);
}

void ImgButton::enterEvent(QEvent *) {
    DrawIcon(hoverIconPath);
}

void ImgButton::leaveEvent(QEvent *) {
    DrawIcon(normalIconPath);
}

void ImgButton::SetIconPath(QString normal, QString hover, QString press) {
    normalIconPath = normal;
    pressIconPath = press;
    hoverIconPath = hover;
    DrawIcon(normalIconPath);
}

void ImgButton::DrawIcon(QString iconPath) {
    QPixmap pix;
    pix.load(iconPath);
    setIcon(pix);
}
