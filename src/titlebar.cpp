#include "titlebar.h"

#include "base.h"

TitleBar::TitleBar(QWidget *parent) : QWidget(parent) {
    miniBtn = new QPushButton(this);
    closeBtn = new QPushButton(this);
    mainIcon = new QLabel(this);
    title = new QLabel(this);

    QHBoxLayout *titleBarLayout = new QHBoxLayout;

    titleBarLayout->addWidget(mainIcon);
    titleBarLayout->addSpacing(10);
    titleBarLayout->addWidget(title);
    titleBarLayout->addStretch();
    titleBarLayout->addWidget(miniBtn);
    titleBarLayout->addWidget(closeBtn);

    titleBarLayout->setContentsMargins(0, 0, 0, 0);
    titleBarLayout->setSpacing(0);
    setLayout(titleBarLayout);

    miniBtn->setFixedSize(QSize(32, 32));
    closeBtn->setFixedSize(QSize(32, 32));
    mainIcon->setFixedSize(QSize(32, 32));

    miniBtn->installEventFilter(this);
    closeBtn->installEventFilter(this);

    //    title->setFont(QFont("Microsoft YaHei UI", 12));
    title->setFont(QFont("Agency FB", 16));

    connect(miniBtn, &QPushButton::clicked, this, &TitleBar::onMiniBtnClicked);
    connect(closeBtn, &QPushButton::clicked, this, &TitleBar::onCloseBtnClicked);
}

void TitleBar::setIcon(const QPixmap &icon) {
    mainIcon->setScaledContents(true);
    mainIcon->setPixmap(icon);
}

void TitleBar::setTitle(const QString str) {
    title->setText(str);
}

bool TitleBar::eventFilter(QObject *obj, QEvent *event) {
    if (obj == miniBtn) {
        if (event->type() == QEvent::Enter) {
            mbtnHover = true;
        }
        if (event->type() == QEvent::Leave) {
            mbtnHover = false;
        }
        if (event->type() == QEvent::Paint) {
            QPainter painter(miniBtn);
            QPen pen;
            pen.setWidth(1);
            pen.setColor("#606266");

            if (mbtnHover) {
                painter.fillRect(QRect(0, 0, 32, 32), QColor(0xdc, 0xdf, 0xe6, 255 * 0.4));
                painter.setPen(pen);
                painter.drawLine(8, 16, 24, 16);
            } else {
                painter.setPen(pen);
                painter.drawLine(8, 16, 24, 16);
            }
            return true;
        }
    }
    if (obj == closeBtn) {
        if (event->type() == QEvent::Enter) {
            cbtnHover = true;
        }
        if (event->type() == QEvent::Leave) {
            cbtnHover = false;
        }
        if (event->type() == QEvent::Paint) {
            QPainter painter(closeBtn);
            QPen pen;
            pen.setWidth(1);
            if (cbtnHover) {
                pen.setColor("#ffffff");
                painter.fillRect(QRect(0, 0, 32, 32), QColor("#f56c6c"));
                painter.setPen(pen);
                painter.drawLine(8, 8, 24, 24);
                painter.drawLine(24, 8, 8, 24);
            } else {
                pen.setColor("#606266");
                painter.setPen(pen);
                painter.drawLine(8, 8, 24, 24);
                painter.drawLine(24, 8, 8, 24);
            }
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}
