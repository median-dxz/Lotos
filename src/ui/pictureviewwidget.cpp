#include "pictureviewwidget.h"

PictureViewWidget::PictureViewWidget(QWidget *parent) : QWidget(parent) {
    mainWidget = 0;
    setOpacity(0.7);
    setBgColor(QColor(0, 0, 0));

    //不设置主窗体则遮罩层大小为默认桌面大小
    this->setGeometry(qApp->primaryScreen()->availableGeometry());
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
}

PictureViewWidget &PictureViewWidget::Instance() {
    static PictureViewWidget instance;
    return instance;
}

void PictureViewWidget::setMainWidget(QWidget *mainWidget) {
    this->mainWidget = mainWidget;
}

void PictureViewWidget::setOpacity(double opacity) {
    this->setWindowOpacity(opacity);
}

void PictureViewWidget::setBgColor(const QColor &bgColor) {
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, bgColor);
    this->setPalette(palette);
}

void PictureViewWidget::showEvent(QShowEvent *) {
    if (mainWidget != nullptr) {
        this->setGeometry(mainWidget->geometry());
    }
}
