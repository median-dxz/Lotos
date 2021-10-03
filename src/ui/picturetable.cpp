#include "picturetable.h"

#include <QCheckBox>
#include <QDebug>

#include "base.h"
#include "utils/lotoshelper.h"

using namespace LotosHelper;

namespace PictureTableConstant {

const struct {
    QString width = "width";
    QString height = "height";
    QString filename = "filename";
    QString storename = "storename";
    QString size = "size";
    QString path = "path";
    QString hash = "hash";
    QString url = "url";
    QString delete_link = "delete";
    QString page_link = "page";
    QString time_stamp = "created_at";
    QString with_token = "token_with";
} sk;

QStringList iconsPath = {":/res/icons/triangle_down.png", ":/res/icons/triangle_up.png"};
constexpr int DefaultLineHeight = 42;
constexpr int rowWidth[6] = {240, 70, 170, 75, 110, 200};

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
        sortList(flag_fn, 1);
        flag_link = flag_size = 1;
        flag_fn = 0;
    });
    connect(header->head_size, &QPushButton::clicked, this, [=]() {
        sortList(flag_size, 2);
        flag_link = flag_fn = 1;
        flag_size = 0;
    });
    connect(header->head_link, &QPushButton::clicked, this, [=]() {
        sortList(flag_link, 3);
        flag_fn = flag_size = 1;
        flag_link = 0;
    });
}

void PictureTable::filter(std::function<bool(const SMMS::ImageInfomation &)>) {
    for (int i = 0; i < lines.length(); i++) {
        qDebug() << i;
    }
}

QList<int> PictureTable::getCheckedItems() const {
    QList<int> items;
    for (int i = 0; i < lines.size(); i++) {
        if (lines.at(i)->getCheckStatus() == Qt::CheckState::Checked) {
            items << i;
        }
    }
    return items;
}

void PictureTable::onDeleteLine(PictureTableLine *obj) {
    QUrl delete_link(obj->d["delete"].toString());

    datas.remove(obj);
    lines.removeOne(obj);
    layout()->removeWidget(obj);
    obj->deleteLater();

    qDebug() << delete_link;
    HttpClient *client = new HttpClient(this);
    client->get(delete_link);
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
    datas.insert(line, d);
    lines.append(line);

    // connect(line, &PictureTableLine::checkedStateChanged, this, [=]() {});
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

PictureTableLine::PictureTableLine(QWidget *parent, QVariantMap &data) : QWidget(parent), d(data) {
    setLayout(new QHBoxLayout(this));
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    layout()->setMargin(0);
    layout()->setSpacing(10);
    layout()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    cb = new QCheckBox(this);
    cb->setFixedWidth(40);
    connect(cb, &QCheckBox::stateChanged, this, &PictureTableLine::checkedStateChanged);
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
    painter.setRenderHints(QPainter::HighQualityAntialiasing);
    painter.fillRect(0, 0, width(), height(), QColor("#fff"));
    painter.setPen(QColor("#ebeef5"));
    painter.drawLine(0, 0, width(), 0);
}

void PictureTableLine::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing);
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

void PictureTable::sortList(bool cmp, int col) {
    QList<Qt::CheckState> check;
    header->fnSort->setPixmap(QPixmap());
    header->sizeSort->setPixmap(QPixmap());
    header->linkSort->setPixmap(QPixmap());

    for (int i = 0; i < lines.length(); i++) {
        check.append(lines[i]->getCheckStatus());
    }

    if (cmp) {
        switch (col) {
            case 1:
                qSort(lines.begin(), lines.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d[sk.filename] < info2->d[sk.filename];
                });
                header->fnSort->setPixmap(QPixmap(iconsPath[0]));
                break;
            case 2:
                qSort(lines.begin(), lines.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d[sk.size] < info2->d[sk.size];
                });
                header->sizeSort->setPixmap(QPixmap(iconsPath[0]));
                break;
            case 3:
                qSort(lines.begin(), lines.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d[sk.time_stamp] < info2->d[sk.time_stamp];
                });
                header->linkSort->setPixmap(QPixmap(iconsPath[0]));
                break;
        }
    } else {
        switch (col) {
            case 1:
                qSort(lines.begin(), lines.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d[sk.filename] < info2->d[sk.filename];
                });
                header->fnSort->setPixmap(QPixmap(iconsPath[1]));
                break;
            case 2:
                qSort(lines.begin(), lines.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d[sk.size] > info2->d[sk.size];
                });
                header->sizeSort->setPixmap(QPixmap(iconsPath[1]));
                break;
            case 3:
                qSort(lines.begin(), lines.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d[sk.time_stamp] > info2->d[sk.time_stamp];
                });
                header->linkSort->setPixmap(QPixmap(iconsPath[1]));
                break;
        }
    }

    for (int i = 0; i < lines.length(); i++) {
        lines[i]->resetLine(datas[lines[i]]);
        lines[i]->d = datas[lines[i]];
        lines[i]->setCheckState(check[i]);
        // qDebug()<< i << Lines.at(i)->in;//换过去
    }
}

void PictureTableLine::resetLine(const QVariantMap &data) {
    d = data;
    //    if(p.isNull()){
    //        p = QPixmap();
    //        p = p.scaledToHeight(height() - 8, Qt::SmoothTransformation);
    //        lab_thumb->setPixmap(p);
    //    }
    lab_filename->setText(data[sk.filename].toString());
    lab_time->setText(timestamp2str(data[sk.time_stamp].toUInt(), "yyyy/MM/dd hh:mm:ss"));
    lab_size->setText(formatFileSize(data[sk.size].toUInt()));
    lab_base->setText(data[sk.with_token].toBool() ? tr("基于Token") : tr("基于IP"));
}

int PictureTableLine::getIndex() const {
    return index;
}

void PictureTableLine::setIndex(int value) {
    index = value;
}
