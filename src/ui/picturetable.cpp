#include "picturetable.h"

#include <QCheckBox>
#include <QDebug>

#include "base.h"
#include "mainwindow.h"
#include "messagebox.h"
#include "notification.h"
#include "pictureviewwidget.h"
#include "utils/httpclient.h"
#include "utils/lotoshelper.h"
#include "utils/promise.h"

using namespace LotosHelper;

namespace PictureTableConstant {

struct {
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
    notify = &NotificationManager::Instance();

    header = new PictureTableHeader(this);
    header->setFixedHeight(lineHeight);

    setLayout(new QVBoxLayout(this));
    layout()->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->addWidget(header);

    connect(header->head_name, &QPushButton::clicked, this, [=]() {
        sortList(flag_fn, 1);
        flag_link = flag_size = 1;
        flag_fn = !flag_fn;
    });
    connect(header->head_size, &QPushButton::clicked, this, [=]() {
        sortList(flag_size, 2);
        flag_link = flag_fn = 1;
        flag_size = !flag_size;
    });
    connect(header->head_link, &QPushButton::clicked, this, [=]() {
        sortList(flag_link, 3);
        flag_fn = flag_size = 1;
        flag_link = !flag_link;
    });
    connect(header->all, &QCheckBox::clicked, this, [=](bool checked) {
        if (checked && lines.size() > 0) {
            for (int i = 0; i < lines.size(); i++) {
                lines.at(i)->cb->setChecked(true);
            }
        } else {
            header->all->setChecked(false);
            for (int i = 0; i < lines.size(); i++) {
                lines.at(i)->cb->setChecked(false);
            }
        }
    });
}

void PictureTable::filter(std::function<bool(const QVariantMap &)> f) {
    if (f == 0) {
        f = [=](const QVariantMap &) { return true; };
    }
    for (int i = 0; i < lines.length(); i++) {
        if (f(datas[lines.at((i))])) {
            lines.at(i)->show();
        } else {
            lines.at(i)->hide();
        }
    }
}

const QVariantMap PictureTable::getLineData(PictureTable::Item item) const {
    return datas[item];
}

void PictureTable::delSelectedItems() {
    QList<PictureTableLine *> items;
    for (int i = 0; i < lines.size(); i++) {
        if (lines.at(i)->cb->checkState() == Qt::Checked) {
            items.append(lines.at(i));
        }
    }
    for (PictureTableLine *i : qAsConst(items)) {
        onDeleteLine(i);
    }
}

void PictureTable::onDeleteLine(PictureTableLine *obj) {
    QUrl delete_link(obj->data.delete_link);

    datas.remove(obj);
    lines.removeOne(obj);
    layout()->removeWidget(obj);
    obj->deleteLater();
    if (lines.isEmpty()) {
        header->all->setChecked(false);
    }

    HttpClient *client = new HttpClient(this);
    client->get(delete_link);
    connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *r) {
        if (!r->isSucceeded) {
            notify->newNotify(NOTIFYS::ERROR, NOTIFYS::networkError(r->statusCode.toString(), tr("删除失败")));
        } else {
            qDebug() << r->getText();
        }
        delete r;
    });
}

void PictureTable::refresh(QList<QVariantMap> d) {
    flag_fn = flag_link = flag_size = 1;
    header->all->setChecked(false);
    for (int i = 0; i < d.size(); i++) {
        if (i >= lines.size()) {
            addData(d.at(i));
        } else {
            datas[lines.at(i)] = d.at(i);
            lines.at(i)->cb->setChecked(false);
            lines.at(i)->resetLine(d.at(i));
        }
    }

    if (d.size() < lines.size()) {
        while (datas.key(d.back()) != lines.back()) {
            onDeleteLine(lines.back());
        }
    }
}

void PictureTable::addData(QVariantMap d) {
    PictureTableLine *line = new PictureTableLine(this, d);
    line->setFixedHeight(lineHeight);
    layout()->addWidget(line);
    datas.insert(line, d);

    lines.append(line);
    header->all->setChecked(false);
    connect(line, &PictureTableLine::checkedStateChanged, this, [=](int s) {
        if (Qt::CheckState(s) == Qt::Unchecked) {
            header->all->setChecked(false);
        }
    });
    connect(line, &PictureTableLine::preview, this, &PictureTable::previewImage);
    connect(line, &PictureTableLine::deleteLine, this, &PictureTable::onDeleteLine);
    connect(line, &PictureTableLine::download, this, &PictureTable::downloadStarted);
    connect(line, &PictureTableLine::copyLink, this, &PictureTable::copyLinkStarted);
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
    //    head_pix->setText("预览");
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

PictureTableLine::PictureTableLine(QWidget *parent, QVariantMap &data) : QWidget(parent) {
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
    resetLine(data);
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
    header->fnSort->setPixmap(QPixmap());
    header->sizeSort->setPixmap(QPixmap());
    header->linkSort->setPixmap(QPixmap());

    for (int i = 1; i <= lines.length(); i++) {
        layout()->takeAt(i);
    }

    if (cmp) {
        switch (col) {
            case 1:
                std::sort(lines.begin(), lines.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->data.filename < info2->data.filename;
                });
                header->fnSort->setPixmap(QPixmap(iconsPath[0]));
                break;
            case 2:
                std::sort(lines.begin(), lines.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->data.size < info2->data.size;
                });
                header->sizeSort->setPixmap(QPixmap(iconsPath[0]));
                break;
            case 3:
                std::sort(lines.begin(), lines.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->data.timestamp < info2->data.timestamp;
                });
                header->linkSort->setPixmap(QPixmap(iconsPath[0]));
                break;
        }
    } else {
        switch (col) {
            case 1:
                std::sort(lines.begin(), lines.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->data.filename > info2->data.filename;
                });
                header->fnSort->setPixmap(QPixmap(iconsPath[1]));
                break;
            case 2:
                std::sort(lines.begin(), lines.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->data.size > info2->data.size;
                });
                header->sizeSort->setPixmap(QPixmap(iconsPath[1]));
                break;
            case 3:
                std::sort(lines.begin(), lines.end(), [](const PictureTableLine *info1, const PictureTableLine *info2) {
                    return info1->data.timestamp > info2->data.timestamp;
                });
                header->linkSort->setPixmap(QPixmap(iconsPath[1]));
                break;
        }
    }

    for (int i = 0; i < lines.length(); i++) {
        layout()->addWidget(lines[i]);
    }
}
void PictureTableLine::resetLine(const QVariantMap &d) {
    SMMS::praseImageInfomation(QJsonObject::fromVariantMap(d), data);
    //    if(p.isNull()){
    //        p = QPixmap();
    //        p = p.scaledToHeight(height() - 8, Qt::SmoothTransformation);
    //        lab_thumb->setPixmap(p);
    //    }
    lab_filename->setText(data.filename);
    lab_time->setText((data.timestamp) ? timestamp2str(data.timestamp, "yyyy/MM/dd hh:mm:ss") : tr("无上传时间信息"));
    lab_size->setText(formatFileSize(data.size));
    lab_base->setText(data.token_with ? tr("基于Token") : tr("基于IP"));
}

int PictureTableLine::getIndex() const {
    return index;
}

void PictureTableLine::setIndex(int value) {
    index = value;
}

void PictureTable::previewImage(PictureTableLine *obj) {
    using namespace std::placeholders;

    PictureViewWidget::showManager f = std::bind(&PictureTable::setPreviewImage, this, obj, _1, _2, _3);
    PictureViewWidget::Instance().showInfo(f);
}

int PictureTable::setPreviewImage(PictureTableLine *obj, PictureViewWidget *self, QLabel *imgBox, QLabel *info) {
    QString infoText =
        tr("<h3>文件名</h3>\n%1\n"
           "<h3>文件路径</h3>\n%2\n"
           "<h3>文件大小</h3>\n%3\n"
           "<h3>图片尺寸</h3>\n%4 × %5");

    Promise<QPixmap> *scalePix = new Promise<QPixmap>(this);
    scalePix->onFinished([=](Promise<QPixmap>::result p) {
        info->setText(infoText.arg(obj->data.filename)
                          .arg(obj->data.url)
                          .arg(formatFileSize(obj->pixmapData.size()))
                          .arg(obj->data.width)
                          .arg(obj->data.height));

        info->adjustSize();
        imgBox->setPixmap(p);
        self->show();
    });

    Promise<QPixmap>::async scaleProgress = [=](Promise<QPixmap>::Resolver resolve, Promise<QPixmap>::Rejector) {
        QPixmap p;
        QFontMetrics metrics = QFontMetrics(info->font());
        QRect rect = metrics.boundingRect(infoText);
        QMetaObject::invokeMethod(info, "setText", Q_ARG(QString, tr("加载图片中...")));
        p.loadFromData(obj->pixmapData);

        if (p.height() > self->height() - 20 - rect.height() - 40)
            p = p.scaledToHeight(self->height() - 20 - rect.height() - 40, Qt::SmoothTransformation);
        if (p.width() > self->width())
            p = p.scaledToWidth(self->width(), Qt::SmoothTransformation);
        resolve(p);
    };

    if (obj->pixmapData.isNull()) {
        self->show();
        HttpClient *client = new HttpClient(this);
        client->setUrl(obj->data.url);
        client->downloadFile();
        connect(client, &HttpClient::downloadProgress, this, [=](long long a, long long b) {
            QStringList loadText = QStringList() << tr("正在下载 ") << obj->data.filename << ": "
                                                 << formatFileSize(a) + "/" + formatFileSize(b);

            info->setText(loadText.join(""));
        });
        connect(client, &HttpClient::responseFinished, this, [=](HttpClient::Response *r) {
            obj->pixmapData = r->data;
            scalePix->setPromise(scaleProgress);
            delete r;
        });
    } else {
        scalePix->setPromise(scaleProgress);
    }

    return lines.indexOf(obj);
}
