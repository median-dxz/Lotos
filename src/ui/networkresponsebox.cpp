#include "networkresponsebox.h"

#include "base.h"

NetworkResponseBox::NetworkResponseBox(QWidget *parent) : MessageBox(parent) {
    lab_tip = new QLabel(this);
    lab_tip->setContentsMargins(10, 10, 10, 10);
    lab_tip->setTextInteractionFlags(Qt::TextSelectableByMouse);
    lab_tip->setAlignment(Qt::AlignHCenter | Qt::AlignBaseline);
    QFont font;
    font.setPointSize(18);
    lab_tip->setFont(font);
    lab_tip->setWordWrap(true);
    lab_tip->setProperty("class_type", "messagebox.label");

    mainArea()->addWidget(lab_tip, 0, 0);
}

void NetworkResponseBox::setTip(const QString &text) {
    lab_tip->setText(text);
}
