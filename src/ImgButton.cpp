#include "ImgButton.h"

ImgButton::ImgButton(QWidget *parent) : QPushButton(parent) {}

ImgButton::ImgButton(QString normal, QString hover, QString press) {
    setMouseTracking(true);
    normalIconPath = normal;
    pressIconPath = press;
    hoverIconPath = hover;

    QPixmap pix;
    pix.load(normalIconPath);

    QSize iconSize = QSize(32, 32);
    setFixedSize(iconSize);
    setStyleSheet("QPushButton{border:none;}");
    setIcon(pix);

    setIconSize(iconSize);
}

void ImgButton::mouseReleaseEvent(QMouseEvent *e) {
    QPixmap pix;
    pix.load(hoverIconPath);
    setIcon(pix);

    QPushButton::mouseReleaseEvent(e);
}

void ImgButton::mousePressEvent(QMouseEvent *e) {
    QPixmap pix;
    pix.load(pressIconPath);
    setIcon(pix);

    QPushButton::mousePressEvent(e);
}

void ImgButton::enterEvent(QEvent *) {
    QPixmap pix;
    pix.load(hoverIconPath);
    setIcon(pix);
}

void ImgButton::leaveEvent(QEvent *) {
    QPixmap pix;
    pix.load(normalIconPath);
    setIcon(pix);
}

void ImgButton::SetIconPath(QString normal, QString hover, QString press) {
    normalIconPath = normal;
    pressIconPath = press;
    hoverIconPath = hover;
    QPixmap pix;
    pix.load(normalIconPath);
    setIcon(pix);
}
