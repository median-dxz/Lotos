#include "pagebutton.h"

PageButton::PageButton(QWidget *parent) : QPushButton(parent) {
    connect(this, &PageButton::pressed, this, &PageButton::emitClicked);
    connect(this, &PageButton::clicked, this, &PageButton::emitClicked);
}

void PageButton::emitClicked() {
    if (!isChecked()) {
        setChecked(true);
        emit indexChanged(index);
    }
}

void PageButton::setIndex(int index) {
    PageButton::index = index;
}

void PageButton::setCurrentChosen(int index) {
    if (PageButton::index != index) {
        setChecked(false);
    } else {
        setChecked(true);
    }
}

void PageButton::setIconPath(QString path, QString pathIgnited) {
    normalIconPath = path;
    ignitedIconPath = pathIgnited;
}

void PageButton::drawIcon(bool ignited) {
    if (ignited) {
        setIcon(QIcon(ignitedIconPath));
    } else {
        setIcon(QIcon(normalIconPath));
    }
}

void PageButton::DrawPix(QString iconPath){


    label->setPixmap(iconPath);
    label->setScaledContents(true);
    label->resize(QSize(35, 35));
    label->move(5,27);



}

QString & PageButton::getnormalIconPath()
{
    return normalIconPath;
}
QString & PageButton::getignitedIconPath()
{
    return ignitedIconPath;
}

