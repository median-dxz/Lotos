#include "picturetable.h"
#include <QDebug>

#define timestamp2str(time, format) (QDateTime::fromTime_t(time).toString(format))
QString formatFileSize(qint64 size) {
    int type = int(log2(size) / 10);
    QString sizeUnit;
    switch (type) {
        case 0:
            sizeUnit = "B";
            break;
        case 1:
            sizeUnit = "KB";
            break;
        case 2:
            sizeUnit = "MB";
            break;
        default:
            break;
    }

    return QString::number(size / pow(2, int(log2(size)) / 10 * 10), 'f', 2) + sizeUnit;
}

PictureTable::PictureTable(QWidget *parent) : QFrame(parent) {
    header = new PictureTableHeader(this);
    header->setFixedHeight(lineHeight);
    setLayout(new QVBoxLayout(this));
    layout()->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->addWidget(header);
    connect(header->all, &QCheckBox::stateChanged, this, [=]() {
        for (int i = 0; i < Lines.size(); i++)
            if (header->all->checkState() == Qt::Checked)
                Lines[i]->setCheckState(Qt::Checked);
            else
                Lines[i]->setCheckState(Qt::Unchecked);
    });

    connect(header->head_name, &QPushButton::clicked, this, [=]() {
        devListSort(flag_fn, 1, Lines);
        flag_link = flag_size = 1;
    });
    connect(header->head_size, &QPushButton::clicked, this, [=]() {
        devListSort(flag_size, 2, Lines);
        flag_link = flag_fn = 1;
    });
    connect(header->head_link, &QPushButton::clicked, this, [=]() {
        devListSort(flag_link, 3, Lines);
        flag_fn = flag_size = 1;
    });
}

void PictureTable::filter(std::function<bool(ImageInfo &)>) {
    for (int i = 0; i < Lines.length(); i++) {
        qDebug() << i;
    }
}

void PictureTable::onDeleteLine(PictureTableLine *obj) {
    QList<PictureTableLine *>::iterator del_iter;
    for (QList<PictureTableLine *>::iterator iter = Lines.begin(); iter != Lines.end(); iter++) {
        if (*iter == obj) {
            del_iter = iter;
            break;
        }
    }
    this->layout()->removeWidget(obj);
    Lines.erase(del_iter);
    obj->deleteLater();
}

void PictureTable::addData(QVariantMap d) {
    PictureTableLine *line = new PictureTableLine(this, d);
    line->setFixedHeight(lineHeight);
    layout()->addWidget(line);
    datas.append(d);
    Lines.append(line);
    connect(line, &PictureTableLine::onStateChanged, this, [=]() {
        list.clear();
        for (int i = 0; i < Lines.size(); i++) {
            list.append(Lines[i]->getCheckStatus());
            qDebug() << list[i];
        }
    });
    connect(line, &PictureTableLine::deleteLine, this, &PictureTable::onDeleteLine);
}

PictureTableHeader::PictureTableHeader(QWidget *parent) : QWidget(parent) {
    setLayout(new QHBoxLayout(this));
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    all = new QCheckBox(this);
    all->setFixedWidth(40);
    layout()->addWidget(all);
    head_name = new QPushButton(this);
    head_name->setFixedWidth(rowWidth[0]);
    head_name->setText("文件名");
    head_name->setProperty("class_type", "head");
    head_name->setCursor(QCursor(Qt::PointingHandCursor));
    fnSort = new QLabel(head_name);
    fnSort->setGeometry(60, 4, 20, 16);
    layout()->addWidget(head_name);

    QLabel *head_pix = new QLabel(this);
    head_pix->setFixedWidth(rowWidth[1]);
    head_pix->setText("预览");
    head_pix->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout()->addWidget(head_pix);

    head_link = new QPushButton(this);
    head_link->setFixedWidth(rowWidth[2]);
    head_link->setText("上传时间");
    head_link->setProperty("class_type", "head");
    head_link->setCursor(QCursor(Qt::PointingHandCursor));
    ((QHBoxLayout *)layout())->addSpacing(60);
    layout()->addWidget(head_link);

    linkSort = new QLabel(head_link);
    linkSort->setGeometry(80, 4, 20, 16);
    layout()->addWidget(head_link);

    head_size = new QPushButton(this);
    head_size->setFixedWidth(rowWidth[3]);
    head_size->setText("大小");
    head_size->setProperty("class_type", "head");
    head_size->setCursor(QCursor(Qt::PointingHandCursor));
    ((QHBoxLayout *)layout())->addSpacing(40);
    layout()->addWidget(head_size);

    sizeSort = new QLabel(head_size);
    sizeSort->setGeometry(45, 4, 20, 16);
    layout()->addWidget(head_size);

    QLabel *head_rec = new QLabel(this);

    head_rec->setFixedWidth(rowWidth[4]);
    head_rec->setText("上传来源");
    head_rec->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ((QHBoxLayout *)layout())->addSpacing(40);
    layout()->addWidget(head_rec);

    ((QHBoxLayout *)layout())->addSpacing(20);
    QLabel *head_op = new QLabel(this);
    head_op->setFixedWidth(rowWidth[5]);
    head_op->setText("操作");
    head_op->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout()->addWidget(head_op);
}

PictureTableLine::PictureTableLine(QWidget *parent, QVariantMap &data) : QWidget(parent), data(data) {
    d.filename = data[DataKey.filename].toString();
    d.size = data[DataKey.size].toUInt();
    d.timestamp = data[DataKey.timestamp].toUInt();
    d.uploadWithToken = data[DataKey.uploadWithToken].toBool();  //到时候初始化列表就行，这里这些代码只是测试
    d.thumb = data[DataKey.thumb].toByteArray();

    setLayout(new QHBoxLayout(this));
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    cb = new QCheckBox(this);
    cb->setFixedWidth(40);
    connect(cb, &QCheckBox::stateChanged, this, &PictureTableLine::onStateChanged);
    lab_filename = new QLabel(this);
    lab_filename->setFixedWidth(rowWidth[0]);
    lab_filename->setText(data[DataKey.filename].toString());

    lab_thumb = new QLabel(this);
    lab_thumb->setFixedWidth(rowWidth[1]);

    lab_link = new QLabel(this);
    lab_link->setFixedWidth(rowWidth[2]);
    lab_link->setText(data[DataKey.width].toString());

    lab_size = new QLabel(this);
    lab_size->setFixedWidth(rowWidth[3]);
    lab_size->setText(data[DataKey.size].toString());

    lab_rec = new QLabel(this);
    lab_rec->setFixedWidth(rowWidth[4]);
    lab_rec->setText(data[DataKey.uploadWithToken].toString());

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

    p.loadFromData(d.thumb);
    p = p.scaledToHeight(height() - 8, Qt::SmoothTransformation);
    lab_thumb->setPixmap(p);

    layout()->addWidget(cb);
    layout()->addWidget(lab_filename);
    layout()->addWidget(lab_thumb);
    ((QHBoxLayout *)layout())->addSpacing(40 + 20);
    layout()->addWidget(lab_link);
    ((QHBoxLayout *)layout())->addSpacing(40);
    layout()->addWidget(lab_size);
    ((QHBoxLayout *)layout())->addSpacing(40);
    layout()->addWidget(lab_rec);
    ((QHBoxLayout *)layout())->addSpacing(20);

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
}

void PictureTableHeader::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    painter.fillRect(0, 0, width(), height(), QColor("#fff"));
    painter.setPen(QColor("#ebeef5"));
    painter.drawLine(0, 0, width(), 0);
    update();
}

void PictureTableLine::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    painter.fillRect(0, 0, width(), height(), lineBackgroundColor);
    painter.setPen(QColor("#ebeef5"));
    painter.drawLine(0, 0, width(), 0);
    update();
}

void PictureTableLine::enterEvent(QEvent *) {
    lineBackgroundColor = QColor("#f7f7f7");
}

void PictureTableLine::leaveEvent(QEvent *) {
    lineBackgroundColor = QColor("#fff");
}

Qt::CheckState PictureTableLine::getCheckStatus() {
    return cb->checkState();
}
void PictureTableLine::setCheckState(Qt::CheckState state) {
    cb->setCheckState(state);
}

void PictureTable::devListSort(bool &cmp, int sel, QList<PictureTableLine *> list) {
    QList<Qt::CheckState> check;
    QList<ImageInfo> infomation;

    header->fnSort->setPixmap(QPixmap(""));
    header->sizeSort->setPixmap(QPixmap(""));
    header->linkSort->setPixmap(QPixmap(""));
    if (cmp) {
        switch (sel) {
            case 1:
                qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d.filename < info2->d.filename;
                });
                header->fnSort->setPixmap(QPixmap(iconsPath[3]));
                break;
            case 2:
                qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d.size < info2->d.size;
                });
                header->sizeSort->setPixmap(QPixmap(iconsPath[3]));
                break;
            case 3:
                qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d.width < info2->d.width;
                });
                header->linkSort->setPixmap(QPixmap(iconsPath[3]));
                break;
        }
        cmp = 0;
    } else {
        switch (sel) {
            case 1:
                qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d.filename > info2->d.filename;
                });
                header->fnSort->setPixmap(QPixmap(iconsPath[4]));
                break;
            case 2:
                qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d.size > info2->d.size;
                });
                header->sizeSort->setPixmap(QPixmap(iconsPath[4]));
                break;
            case 3:
                qSort(list.begin(), list.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->d.width > info2->d.width;
                });
                header->linkSort->setPixmap(QPixmap(iconsPath[4]));
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
        Lines[i]->resetLine(infomation[i]);
        Lines[i]->d = infomation[i];
        Lines[i]->setCheckState(check[i]);
        // qDebug()<< i << Lines.at(i)->in;//换过去
    }
}

void PictureTableLine::resetLine(ImageInfo data) /*QString pix, QString fn , int link , qint64 size ,QString rec)*/
{
    p = QPixmap(data.thumb);
    p = p.scaledToHeight(height() - 8, Qt::SmoothTransformation);
    lab_thumb->setPixmap(p);
    lab_filename->setText(data.filename);
    lab_link->setText(QString::number(data.width));
    lab_size->setText(QString::number(data.size));
    lab_rec->setText(QString::number(data.uploadWithToken));
}
