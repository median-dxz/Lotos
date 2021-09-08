#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPushButton>
#include <QRect>
#include <QVariant>
#include <QWidget>
#include <cmath>

struct PicFileInfo {
    QString format;
    qint64 fileSize = 0;
    QSize size;
};

class IconWidget : public QWidget {
   public:
    explicit IconWidget(QWidget *parent = nullptr);
    bool setPic(QString fileName);
    const PicFileInfo &picInfo() const { return pic; }

    static QString sizeUnit(qint64 size);

   protected:
    void paintEvent(QPaintEvent *) override;

   private:
    void setShadow();
    void setDeleteBtn();
    PicFileInfo pic;
    QPixmap pix;
};

#endif  // ICONWIDGET_H
