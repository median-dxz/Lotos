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

static const int rowWidth[6]={300,100,250,250,250,50};

class PictureTable : public QFrame {
    Q_OBJECT
   signals:

   public:
    explicit PictureTable(QWidget *parent = nullptr);

    int getLineHeight() const { return lineHeight; };
    void setLineHeight(int h) { lineHeight = h; };
    static bool subDevListSort(const PictureTableLine* info1  ,const  PictureTableLine* info2 );
    void devListSort(int cmp , int sel , QList <PictureTableLine *> list);




   public slots:
    void addData(QVariantMap d);
    void deleteLine1(PictureTableLine *);


   private:
    PictureTableHeader *header;
    QList<PictureTableLine *> Lines;
    QList<QVariantMap > datas;
    QList <int> list;
    bool flag = 1;


    int lineHeight = DefaultLineHeight;
};

class PictureTableHeader : public QWidget {
    Q_OBJECT
   public:
    PictureTableHeader(QWidget *parent);
    int getHeaderHeight() const { return lineHeight; }
    void setHeaderHeight(int h) { lineHeight = h; }
    QCheckBox *all;
    QPushButton *fnSort, *sizeSort, *linkSort;

protected:
   void paintEvent(QPaintEvent *) override;

   private:
    int lineHeight = DefaultHeaderHeight;

public slots:
};

class PictureTableLine : public QWidget {
    Q_OBJECT
   public:
    static int s;
    int in;
    PictureTableLine(QWidget *parent, QVariantMap &data);
    Qt::CheckState getCheckStatus();
    void setCheckState(Qt::CheckState);
    void resetLine(int  , QString , QString  , QString  ,int );
    QLabel *lab_filename;
    QList <QString> content;

   private:
    QVariantMap &data;
    QCheckBox *cb;
    QLabel *lab_link, *lab_width, *lab_size , *lab_rec;
//    QComboBox *bx;
    QPushButton /**opt_del*/ *op_view , *op_del ,*op_load;
    QColor lineBackgroundColor = QColor("#fff");
    QPixmap p;
//    QLabel *arrow;


signals: void onStateChanged();
         void deleteLine(PictureTableLine *);

   protected:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEvent *)override;
    void leaveEvent(QEvent *)override;

public slots:
//    void x(int );

};

#endif  // PICTURETABLE_H
