#include "picturetable.h"

#include <QCheckBox>
#include <QDebug>

#include "base.h"
#include "utils/lotoshelper.h"

using namespace LotosHelper;

namespace PictureTableConstant {

QStringList iconsPath = {":/res/icons/triangle_down.png", ":/res/icons/triangle_up.png"};
constexpr int DefaultLineHeight = 42;
constexpr int rowWidth[6] = {250, 80, 150, 75, 110, 200};

};  // namespace PictureTableConstant

using namespace PictureTableConstant;

PictureTable::PictureTable(QWidget *parent) : QFrame(parent) {
    lineHeight = PictureTableConstant::DefaultLineHeight;

    header = new PictureTableHeader(this);
    header->setFixedHeight(lineHeight);
    setLayout(new QVBoxLayout(this));
    layout()->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->addWidget(header);
    connect(header->all, &QCheckBox::stateChanged, this, [=]() {
        for (int i = 0; i < lines.size(); i++)
            if (header->all->checkState() == Qt::Checked)
                lines[i]->setCheckState(Qt::Checked);
            else
                lines[i]->setCheckState(Qt::Unchecked);
    });

    connect(header->head_name, &QPushButton::clicked, this, [=]() {
        sortList(flag_fn, 1, lines);
        flag_link = flag_size = 1;
    });
    connect(header->head_size, &QPushButton::clicked, this, [=]() {
        sortList(flag_size, 2, lines);
        flag_link = flag_fn = 1;
    });
    connect(header->head_link, &QPushButton::clicked, this, [=]() {
        sortList(flag_link, 3, lines);
        flag_fn = flag_size = 1;
    });
}

void PictureTable::filter(std::function<bool(const SMMS::ImageInfomation &)>) {
    for (int i = 0; i < lines.length(); i++) {
        qDebug() << i;
    }
}

void PictureTable::onDeleteLine(PictureTableLine *obj) {
    QUrl link_del(obj->d.delete_link);

    lines.removeOne(obj);
    layout()->removeWidget(obj);
    obj->deleteLater();

    qDebug() << link_del;
    HttpClient *client = new HttpClient(this);
    client->get(link_del);
    connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *r) {
        if (r->isSucceeded) {
        }
        delete r;
    });
}

void PictureTable::addData(QVariantMap d) {
    PictureTableLine *line = new PictureTableLine(this, d);
    line->setFixedHeight(lineHeight);
    layout()->addWidget(line);
    datas.append(d);
    lines.append(line);

    connect(line, &PictureTableLine::onStateChanged, this, [=]() {
        list.clear();
        for (int i = 0; i < lines.size(); i++) {
            list.append(lines[i]->getCheckStatus());
            qDebug() << list[i];
        }
    });
    connect(line, &PictureTableLine::deleteLine, this, &PictureTable::onDeleteLine);
}

PictureTableHeader::PictureTableHeader(QWidget *parent) : QWidget(parent) {
    setLayout(new QHBoxLayout(this));
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    layout()->setMargin(0);
    layout()->setSpacing(10);
    layout()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    all = new QCheckBox(this);
    all->setFixedWidth(40);

    head_name = new QPushButton(this);
    head_name->setFixedWidth(rowWidth[0]);
    head_name->setText("文件名");
    head_name->setProperty("class_type", "head");
    head_name->setCursor(QCursor(Qt::PointingHandCursor));
    fnSort = new QLabel(head_name);
    fnSort->setGeometry(60, 4, 20, 16);

    QLabel *head_pix = new QLabel(this);
    head_pix->setFixedWidth(rowWidth[1]);
    head_pix->setText("预览");
    head_pix->setTextInteractionFlags(Qt::TextSelectableByMouse);

    head_link = new QPushButton(this);
    head_link->setFixedWidth(rowWidth[2]);
    head_link->setText("上传时间");
    head_link->setProperty("class_type", "head");
    head_link->setCursor(QCursor(Qt::PointingHandCursor));

    linkSort = new QLabel(head_link);
    linkSort->setGeometry(80, 4, 20, 16);

    head_size = new QPushButton(this);
    head_size->setFixedWidth(rowWidth[3]);
    head_size->setText("大小");
    head_size->setProperty("class_type", "head");
    head_size->setCursor(QCursor(Qt::PointingHandCursor));

    sizeSort = new QLabel(head_size);
    sizeSort->setGeometry(40, 4, 20, 16);

    QLabel *head_rec = new QLabel(this);
    head_rec->setFixedWidth(rowWidth[4]);
    head_rec->setText("上传来源");
    head_rec->setTextInteractionFlags(Qt::TextSelectableByMouse);

    QLabel *head_op = new QLabel(this);
    head_op->setFixedWidth(rowWidth[5]);
    head_op->setText("操作");
    head_op->setTextInteractionFlags(Qt::TextSelectableByMouse);

    layout()->addWidget(all);
    layout()->addWidget(head_name);
    layout()->addWidget(head_pix);
    layout()->addWidget(head_link);
    layout()->addWidget(head_size);
    layout()->addWidget(head_rec);
    layout()->addWidget(head_op);
}

PictureTableLine::PictureTableLine(QWidget *parent, QVariantMap &data) : QWidget(parent), data(data) {
    SMMS::praseImageInfomation(QJsonObject::fromVariantMap(data), d);
    setLayout(new QHBoxLayout(this));
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    layout()->setMargin(0);
    layout()->setSpacing(10);
    layout()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    cb = new QCheckBox(this);
    cb->setFixedWidth(40);
    connect(cb, &QCheckBox::stateChanged, this, &PictureTableLine::onStateChanged);
    lab_filename = new QLabel(this);
    lab_filename->setFixedWidth(rowWidth[0]);

    lab_thumb = new QLabel(this);
    lab_thumb->setFixedWidth(rowWidth[1]);

    lab_time = new QLabel(this);
    lab_time->setFixedWidth(rowWidth[2]);

    lab_size = new QLabel(this);
    lab_size->setFixedWidth(rowWidth[3]);

    lab_base = new QLabel(this);
    lab_base->setFixedWidth(rowWidth[4]);

    op_view = new QPushButton(this);
    op_view->setFixedWidth(rowWidth[5] / 5 * 1);
    op_view->setText("预览");

    op_del = new QPushButton(this);
    op_del->setFixedWidth(rowWidth[5] / 5 * 1);
    op_del->setText("删除");

    op_load = new QPushButton(this);
    op_load->setFixedWidth(rowWidth[5] / 5 * 1);
    op_load->setText("下载");

    op_link = new QPushButton(this);
    op_link->setFixedWidth(rowWidth[5] / 5 * 2);
    op_link->setText("复制链接");

    layout()->addWidget(cb);
    layout()->addWidget(lab_filename);
    layout()->addWidget(lab_thumb);
    layout()->addWidget(lab_time);
    layout()->addWidget(lab_size);
    layout()->addWidget(lab_base);

    layout()->addWidget(op_view);
    layout()->addWidget(op_del);
    layout()->addWidget(op_load);
    layout()->addWidget(op_link);

    op_view->setProperty("class_type", "link");
    op_load->setProperty("class_type", "link");
    op_del->setProperty("class_type", "link");
    op_link->setProperty("class_type", "link");
    connect(op_del, &QPushButton::clicked, this, [=]() { emit deleteLine(this); });
    connect(op_view, &QPushButton::clicked, this, [=]() { emit preview(this); });
    connect(op_load, &QPushButton::clicked, this, [=]() { emit download(this); });
    connect(op_link, &QPushButton::clicked, this, [=]() { emit copyLink(this); });
    resetLine(d);
}

void PictureTableHeader::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    painter.fillRect(0, 0, width(), height(), QColor("#fff"));
    painter.setPen(QColor("#ebeef5"));
    painter.drawLine(0, 0, width(), 0);
}

void PictureTableLine::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    painter.fillRect(0, 0, width(), height(), lineBackgroundColor);
    painter.setPen(QColor("#ebeef5"));
    painter.drawLine(0, 0, width(), 0);
}

void PictureTableLine::enterEvent(QEvent *) {
    lineBackgroundColor = QColor("#f7f7f7");
    update();
}

void PictureTableLine::leaveEvent(QEvent *) {
    lineBackgroundColor = QColor("#fff");
    update();
}

Qt::CheckState PictureTableLine::getCheckStatus() {
    return cb->checkState();
}
void PictureTableLine::setCheckState(Qt::CheckState state) {
    cb->setCheckState(state);
}

void PictureTable::sortList(bool &cmp, int col, QList<PictureTableLine *> list) {
    QList<Qt::CheckState> check;
    QList<SMMS::ImageInfomation> infomation;
    header->fnSort->setPixmap(QPixmap());
    header->sizeSort->setPixmap(QPixmap());
    header->linkSort->setPixmap(QPixmap());
    if (cmp) {
        switch (col) {
            case 1:
                qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d.filename < info2->d.filename;
                });
                header->fnSort->setPixmap(QPixmap(iconsPath[0]));
                break;
            case 2:
                qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d.size < info2->d.size;
                });
                header->sizeSort->setPixmap(QPixmap(iconsPath[0]));
                break;
            case 3:
                qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d.timestamp < info2->d.timestamp;
                });
                header->linkSort->setPixmap(QPixmap(iconsPath[0]));
                break;
        }
        cmp = 0;
    } else {
        switch (col) {
            case 1:
                qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d.filename > info2->d.filename;
                });
                header->fnSort->setPixmap(QPixmap(iconsPath[1]));
                break;
            case 2:
                qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d.size > info2->d.size;
                });
                header->sizeSort->setPixmap(QPixmap(iconsPath[1]));
                break;
            case 3:
                qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d.timestamp > info2->d.timestamp;
                });
                header->linkSort->setPixmap(QPixmap(iconsPath[1]));
                break;
        }
        cmp = 1;
    }

    for (int i = 0; i < list.length(); i++) {
        infomation.append(list[i]->d);
        check.append(list[i]->getCheckStatus());
        // qDebug()<< i << list.at(i)->content[0];
        // Lines[i] =list[i];//这个不行是因为换了整体
        // qDebug()<< i << Lines.at(i)->content[0];//还没换过去
    }

    for (int i = 0; i < list.length(); i++) {
        lines[i]->resetLine(infomation[i]);
        lines[i]->d = infomation[i];
        lines[i]->setCheckState(check[i]);
        // qDebug()<< i << Lines.at(i)->in;//换过去
    }
}

void PictureTableLine::resetLine(const SMMS::ImageInfomation &data) {
    d = data;
    //    if(p.isNull()){
    //        p = QPixmap();
    //        p = p.scaledToHeight(height() - 8, Qt::SmoothTransformation);
    //        lab_thumb->setPixmap(p);
    //    }

    lab_filename->setText(data.filename);
    lab_time->setText(timestamp2str(data.timestamp, "yyyy/MM/dd hh:mm:ss"));
    lab_size->setText(formatFileSize(data.size));
    lab_base->setText(data.token_with ? tr("基于Token") : tr("基于IP"));
}
