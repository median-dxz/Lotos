#include "mainwindow.h"

struct ImageInfomation {
    int width;
    int height;
    QString filename;
    QString storename;
    qint64 size;
    QString path;
    QString hash;
    QString url;
    QString delete_link;
    QString page_link;
    qint64 timestamp;
    QByteArray thumb;

    QVariantMap toQVariantMap() const;
};

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QFile f("a.png");
    f.open(QFile::ReadOnly);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFile qssFile(":/style.css");
    qssFile.open(QFile::ReadOnly);
    qApp->setStyleSheet(qssFile.readAll());

    resize(500, 500);

    PictureTable *nt = new PictureTable(this);
    nt->setFixedHeight(400);

    mainLayout->addWidget(nt);

    ImageInfomation d;
    d.width = 200;
    d.height = 300;
    d.delete_link = "del";
    d.filename = "fn";
    d.hash = "hash";
    d.page_link = "page";
    d.path = "path";
    d.size = 555;
    d.storename = "storen";
    d.url = "url//";
    d.timestamp = 100;
    d.thumb = f.readAll();

    qApp->setFont(QFont("Microsoft YaHei", 12));
    nt->addData(d.toQVariantMap());
    nt->addData(d.toQVariantMap());
    nt->addData(d.toQVariantMap());

    setLayout(mainLayout);
}

MainWindow::~MainWindow() {}

void MainWindow::insertData() {}

void MainWindow::deleteData() {}

void MainWindow::showData() {}

QVariantMap ImageInfomation::toQVariantMap() const {
    QVariantMap data;
    data["delete"] = delete_link;
    data["page"] = page_link;
    data["filename"] = filename;
    data["storename"] = storename;
    data["hash"] = hash;
    data["height"] = height;
    data["width"] = width;
    data["path"] = path;
    data["size"] = size;
    data["url"] = url;
    data["uploadtime"] = timestamp;
    data["thumb"] = thumb;
    return data;
}

QString MainWindow::getElidedText(QFont font, QString str, int MaxWidth)
{
    QStringList list = str.split('\n', QString::SkipEmptyParts);
    QFontMetrics fontWidth(font);
    for(int i= 0 ;i<list.size();i++)
    list[i] = fontWidth.elidedText(list[i],Qt::ElideRight,MaxWidth);
    str = list.join('\n');
    return str;
}

QString MainWindow::Time_t(qint64 time){
    QDateTime time1 = QDateTime::fromTime_t(time);
    return time1.toString("yyyy-MM-ddThh:mm:ss.000Z");
}
