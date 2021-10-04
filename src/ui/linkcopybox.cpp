#include "linkcopybox.h"

#include <QDialogButtonBox>
#include <QLineEdit>

#include "base.h"
#include "picturetable.h"
#include "utils/lotoshelper.h"

using namespace LotosHelper;

LinkCopyBox::LinkCopyBox(QWidget *parent) : MessageBox(parent) {
    m_iconArea->hide();

    const QStringList link_types{"BBCode", "Markdown", "HTML", "Url", "Del", "Page"};
    int i = 0;
    QGridLayout *l = m_mainArea;

    for (const QString &type : link_types) {
        lab[i] = new QLabel(this);
        lab[i]->setText(type);
        lab[i]->setAlignment(Qt::AlignRight);

        edt[i] = new QLineEdit(this);
        edt[i]->setReadOnly(true);
        edt[i]->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
        edt[i]->setFont(QFont("Microsoft YaHei UI", 12));
        edt[i]->installEventFilter(this);

        l->addWidget(lab[i], i, 0);
        l->addWidget(edt[i], i, 1);
        i += 1;
    }
    QLabel *tip = new QLabel(this);
    l->addWidget(tip, 6, 0, 1, 2, Qt::AlignCenter);
    tip->setText(tr("提示: 双击复制链接"));

    QPushButton *ret = new QPushButton(this);
    ret->setProperty(StyleType.name, StyleType.button.primary);
    ret->setText(tr("返回"));

    m_dialogButtonBox->addButton(ret, QDialogButtonBox::AcceptRole);
}

bool LinkCopyBox::eventFilter(QObject *obj, QEvent *e) {
    if (e->type() == QEvent::MouseButtonDblClick) {
        if (static_cast<QMouseEvent *>(e)->button() == Qt::MouseButton::LeftButton) {
            static_cast<QLineEdit *>(obj)->selectAll();
            static_cast<QLineEdit *>(obj)->copy();
            return true;
        }
    }
    return MessageBox::eventFilter(obj, e);
}

void LinkCopyBox::setLink(int index, QString link) {
    edt[index]->setText(link);
}
