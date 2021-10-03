#ifndef PICTURETABLE_H
#define PICTURETABLE_H

#include <QFrame>
#include <QObject>

#include "utils/imagehost.h"

class QCheckBox;
class QPushButton;
class QLabel;
class PictureViewWidget;

class NotificationManager;

class PictureTableHeader;
class PictureTableLine;

class PictureTable : public QFrame {
    Q_OBJECT

   public:
    explicit PictureTable(QWidget *parent = nullptr);

    int getLineHeight() const { return lineHeight; }
    void setLineHeight(int h) { lineHeight = h; };
    void sortList(bool cmp, int col);
    void filter(std::function<bool(const QVariantMap &)>);

   public slots:
    void addData(QVariantMap d);
    void onDeleteLine(PictureTableLine *);
    void refresh(QList<QVariantMap> d);
    int setPreviewImage(PictureTableLine *obj, PictureViewWidget *self, QLabel *imgBox, QLabel *info);

    void delSelectedItems();

   private:
    void previewImage(PictureTableLine *obj);

    PictureTableHeader *header;
    QVector<PictureTableLine *> lines;
    QMap<PictureTableLine *, QVariantMap> datas;
    NotificationManager *notify;

    bool flag_fn = 1, flag_link = 1, flag_size = 1;
    int lineHeight;
};

class PictureTableHeader : public QWidget {
    Q_OBJECT
   public:
    PictureTableHeader(QWidget *parent);

   public slots:

   protected:
    void paintEvent(QPaintEvent *) override;

   private:
    QCheckBox *all;
    QPushButton *head_name, *head_link, *head_size;
    QLabel *fnSort, *sizeSort, *linkSort;
    friend class PictureTable;
};

class PictureTableLine : public QWidget {
    Q_OBJECT
   public:
    PictureTableLine(QWidget *parent, QVariantMap &data);
    Qt::CheckState getCheckStatus();
    void setCheckState(Qt::CheckState);
    void resetLine(const QVariantMap &data);

    int getIndex() const;
    void setIndex(int value);

   private:
    SMMS::ImageInfomation data;
    int index;
    QCheckBox *cb;
    QLabel *lab_filename, *lab_time, *lab_thumb, *lab_size, *lab_base;
    QPushButton *op_link, *op_view, *op_del, *op_load;
    QColor lineBackgroundColor = QColor("#fff");
    QByteArray pixmapData = 0;

   signals:
    void checkedStateChanged(int);
    void deleteLine(PictureTableLine *);
    void preview(PictureTableLine *);
    void download(PictureTableLine *);
    void copyLink(PictureTableLine *);

   protected:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    friend class PictureTable;
};

#endif  // PICTURETABLE_H
