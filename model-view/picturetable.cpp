#include "picturetable.h"

PictureTable::PictureTable(QWidget *parent) : QFrame(parent) {
    header = new PictureTableHeader(this);
    setLayout(new QVBoxLayout(this));
    layout()->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    layout()->setMargin(0);
    layout()->setSpacing(0);
}

void PictureTable::addData(QVariantMap d) {
    PictureTableLine *line = new PictureTableLine(this, d);

    line->setFixedHeight(lineHeight);
    layout()->addWidget(line);

    datas.append(d);
    Lines.append(line);
}

PictureTableHeader::PictureTableHeader(QWidget *parent) : QWidget(parent) {}

PictureTableLine::PictureTableLine(QWidget *parent, QVariantMap &data) : QWidget(parent), data(data) {
    setLayout(new QHBoxLayout(this));
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    cb = new QCheckBox(this);
    cb->setFixedWidth(40);

    lab_filename = new QLabel(this);
    lab_filename->setText(data[DataKey.filename].toString());
    lab_link = new QLabel(this);
    lab_link->setText(data[DataKey.width].toString());
    lab_size = new QLabel(this);
    lab_size->setText(data[DataKey.size].toString());
    lab_width = new QLabel(this);
    opt_del = new QPushButton(this);

    QPixmap p;
    p.loadFromData(data[DataKey.thumb].toByteArray());
    p = p.scaledToHeight(height() - 8, Qt::SmoothTransformation);
    lab_width->setPixmap(p);

    layout()->addWidget(cb);
    layout()->addWidget(lab_filename);
    ((QHBoxLayout *)layout())->addSpacing(10);
    layout()->addWidget(lab_link);
    ((QHBoxLayout *)layout())->addSpacing(10);
    layout()->addWidget(lab_size);
    ((QHBoxLayout *)layout())->addSpacing(10);
    layout()->addWidget(lab_width);
    ((QHBoxLayout *)layout())->addSpacing(10);
    layout()->addWidget(opt_del);
    ((QHBoxLayout *)layout())->addSpacing(10);

    opt_del->setProperty("class_type", "link");
    opt_del->setText(" dsad");

    QComboBox *bx = new QComboBox(this);
    bx->addItem("asdasd");
    bx->addItem("Sd");
    layout()->addWidget(bx);
    ((QHBoxLayout *)layout())->addSpacing(10);
    connect(opt_del, &QPushButton::clicked, bx, &QComboBox::showPopup);
}

void PictureTableLine::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    painter.setPen(QPen(QColor("#fff"), 1));

    painter.fillRect(0, 0, width(), height(), QColor("#fff"));

    painter.setPen(QColor("#ebeef5"));
    painter.drawLine(0, 0, width(), 0);
}
