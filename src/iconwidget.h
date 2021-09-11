#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QDebug>
#include <QFile>
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QRect>
#include <QVariant>
#include <QWidget>
#include <cmath>

class IconWidget : public QWidget {
    Q_OBJECT
   public:
    explicit IconWidget(QWidget *parent = nullptr);

    enum HOST_SUPPORT_FORMAT { JPG, PNG, BMP, WEBP, GIF };

    void setImage(QImage img);
    QImage &Image() { return pix; }

    static QString sizeUnit(qint64 size);

   signals:
    void onDeleteBtnClicked(IconWidget *);

   protected:
    void paintEvent(QPaintEvent *) override;

   private:
    void setShadow();
    void setDeleteBtn();
    QImage pix;
    QImage thrumb;
};

#endif  // ICONWIDGET_H
