#ifndef PICTURETABLE_H
#define PICTURETABLE_H

class MainWindow;
#include <mainwindow.h>
#include <QObject>
#include <QWidget>
#include <QList>

class PictureTableHeader;
class PictureTableLine;

#define DefaultHeaderHeight 24
#define DefaultLineHeight 42

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
    QString uploadtime = "uploadtime";
    QString thumb = "thumb";
} DataKey;

class PictureTable : public QFrame {
    Q_OBJECT
   signals:

   public:
    explicit PictureTable(QWidget *parent = nullptr);

    int getLineHeight() const { return lineHeight; };
    void setLineHeight(int h) { lineHeight = h; };




   public slots:
    void addData(QVariantMap d);


   private:
    PictureTableHeader *header;
    QList<PictureTableLine *> Lines;
    QList<QVariantMap> datas;
    QList <int> list;


    int lineHeight = DefaultLineHeight;
};

class PictureTableHeader : public QWidget {
    Q_OBJECT
   public:
    PictureTableHeader(QWidget *parent);
    int getHeaderHeight() const { return lineHeight; };
    void setHeaderHeight(int h) { lineHeight = h; };

   private:
    int lineHeight = DefaultHeaderHeight;
};

class PictureTableLine : public QWidget {
    Q_OBJECT
   public:
    PictureTableLine(QWidget *parent, QVariantMap &data);
    int getCheckStatus();

   private:
    QVariantMap &data;
    QCheckBox *cb;
    QLabel *lab_filename, *lab_link, *lab_width, *lab_size;
    QPushButton *opt_del;


signals: void onStateChanged(int );

   protected:
    void paintEvent(QPaintEvent *) override;


};

#endif  // PICTURETABLE_H
