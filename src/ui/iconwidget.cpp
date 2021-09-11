#include "iconwidget.h"

#include <QDebug>

IconWidget::IconWidget(QWidget *parent) : QWidget(parent) {
    QHBoxLayout *box = new QHBoxLayout(this);
    box->setMargin(0);
    box->setSpacing(0);
    box->setAlignment(Qt::AlignBottom);
    this->setLayout(box);
}

void IconWidget::setImage(QImage img) {
    QHBoxLayout *box = ((QHBoxLayout *)layout());
    pix = img;

    thrumb = pix.scaled(width(), height() - 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    setShadow();
    box->addStretch();
    setDeleteBtn();
    update();
}

void IconWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform |
                           QPainter::TextAntialiasing);
    painter.setPen(QColor("#fff"));
    painter.fillRect(0, 0, width() - painter.pen().width(), height() - painter.pen().width(), QBrush(QColor("#fff")));

    QRect rect;

    rect.setRect(0, 0, thrumb.width(), thrumb.height());
    painter.drawImage(rect, thrumb);

    painter.setFont(QFont("Microsoft YaHei", 12, 400));

    QString text = QString("%1  (%2%3) ").arg("name").arg(1000).arg(sizeUnit(1000));
    painter.setPen(QColor("#CCC"));
    painter.drawText(QRect(0, height() - 80, width(), 40), text);
}

void IconWidget::setShadow() {
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(0, 2);
    shadow->setColor(QColor(0, 0, 0, 255 * 0.14));
    shadow->setBlurRadius(20);
    this->setGraphicsEffect(shadow);
}

void IconWidget::setDeleteBtn() {
    QPushButton *btn = new QPushButton(this);
    layout()->addWidget(btn);
    btn->setFixedSize(QSize(40, 40));

    QIcon ico = QIcon();
    ico.addFile(":/res/icons/del.png", QSize(24, 24), QIcon::Normal);

    btn->setIcon(ico);
    btn->setProperty("class_type", "iconwidget.sub.del");
    connect(btn, &QPushButton::clicked, this, [&] { emit onDeleteBtnClicked(this); });
}

QString IconWidget::sizeUnit(qint64 size) {
    enum SIZE_TYPE { B, KB, MB } type;
    type = SIZE_TYPE(int(log2(size) / 10));
    switch (type) {
        case B:
            return "B";
        case KB:
            return "KB";
        case MB:
            return "MB";
        default:
            break;
    }
    return "";
}
