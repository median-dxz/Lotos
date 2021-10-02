#ifndef PICTURETABLE_H
#define PICTURETABLE_H

#include <QFrame>
#include <QObject>

#include "utils/imagehost.h"

class QCheckBox;
class QPushButton;
class QLabel;

class PictureTableHeader;
class PictureTableLine;

class PictureTable : public QFrame {
    Q_OBJECT

   public:
    explicit PictureTable(QWidget *parent = nullptr);

    int getLineHeight() const { return lineHeight; }
    void setLineHeight(int h) { lineHeight = h; };
    void sortList(bool &cmp, int key, QList<PictureTableLine *> list);
    void filter(std::function<bool(const SMMS::ImageInfomation &)>);

   public slots:
    void addData(QVariantMap d);
    void onDeleteLine(PictureTableLine *);

   private:
    PictureTableHeader *header;
    QList<PictureTableLine *> lines;
    QList<QVariantMap> datas;
    QList<int> list;
    bool flag_fn = 1, flag_link = 1, flag_size = 1;
    int lineHeight;
};

class PictureTableHeader : public QWidget {
    Q_OBJECT
   public:
    PictureTableHeader(QWidget *parent);
    QCheckBox *all;
    QPushButton *head_name, *head_link, *head_size;
    QLabel *fnSort, *sizeSort, *linkSort;
   public slots:

   protected:
    void paintEvent(QPaintEvent *) override;

   private:
};

class PictureTableLine : public QWidget {
    Q_OBJECT
   public:
    PictureTableLine(QWidget *parent, QVariantMap &data);
    Qt::CheckState getCheckStatus();
    void setCheckState(Qt::CheckState);
    void resetLine(const SMMS::ImageInfomation &data);
    SMMS::ImageInfomation d;

   private:
    QVariantMap &data;
    QCheckBox *cb;
    QLabel *lab_filename, *lab_time, *lab_thumb, *lab_size, *lab_base;
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
