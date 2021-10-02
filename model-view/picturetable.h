#ifndef PICTURETABLE_H
#define PICTURETABLE_H

class MainWindow;
#include <mainwindow.h>
#include <QList>
#include <QObject>
#include <QWidget>

class PictureTableHeader;
class PictureTableLine;

struct ImageInfo {
    int width;
    int height;
    bool uploadWithToken;
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
};

const struct {
    QString width = "width";
    QString height = "height";
    QString filename = "filename";
    QString storename = "storename";
    QString size = "size";
    QString path = "path";
    QString hash = "hash";
    QString url = "url";
    QString delete_link = "delete_link";
    QString page_link = "page_link";
    QString timestamp = "timestamp";
    QString uploadWithToken = "uploadWithToken";
    QString thumb = "thumb";
} DataKey;

static const QStringList iconsPath = {":/checkbox.png", ":/checkbox_o.png", ":/checkbox_o_hover.png", ":/downarrow.png",
                                      ":/uparrow.png"};
static constexpr int DefaultHeaderHeight = 24;
static constexpr int DefaultLineHeight = 42;
static constexpr int rowWidth[6] = {250, 75, 125, 100, 100, 200};

class PictureTable : public QFrame {
    Q_OBJECT
   signals:

   public:
    explicit PictureTable(QWidget *parent = nullptr);

    int getLineHeight() const { return lineHeight; }
    void setLineHeight(int h) { lineHeight = h; };
    void devListSort(bool &cmp, int sel, QList<PictureTableLine *> list);

   public slots:
    void addData(QVariantMap d);
    void onDeleteLine(PictureTableLine *);

   private:
    PictureTableHeader *header;
    QList<PictureTableLine *> Lines;
    QList<QVariantMap> datas;
    QList<int> list;
    bool flag_fn = 1, flag_link = 1, flag_size = 1;
    void filter(std::function<bool(ImageInfo &)>);

    int lineHeight = DefaultLineHeight;
};

class PictureTableHeader : public QWidget {
    Q_OBJECT
   public:
    PictureTableHeader(QWidget *parent);
    int getHeaderHeight() const { return lineHeight; }
    void setHeaderHeight(int h) { lineHeight = h; }
    QCheckBox *all;
    QPushButton *head_name, *head_link, *head_size;
    QLabel *fnSort, *sizeSort, *linkSort;

   protected:
    void paintEvent(QPaintEvent *) override;

   private:
    int lineHeight = DefaultHeaderHeight;

   public slots:
};

class PictureTableLine : public QWidget {
    Q_OBJECT
   public:
    PictureTableLine(QWidget *parent, QVariantMap &data);
    Qt::CheckState getCheckStatus();
    void setCheckState(Qt::CheckState);
    void resetLine(ImageInfo data);
    ImageInfo d;

   private:
    QVariantMap &data;
    QCheckBox *cb;
    QLabel *lab_filename, *lab_link, *lab_thumb, *lab_size, *lab_rec;
    QPushButton *op_link, *op_view, *op_del, *op_load;
    QColor lineBackgroundColor = QColor("#fff");
    QPixmap p;

   signals:
    void onStateChanged();
    void deleteLine(PictureTableLine *);
    void preview(PictureTableLine *);
    void download(PictureTableLine *);
    void copyLink(PictureTableLine *);

   protected:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
};

#endif  // PICTURETABLE_H
