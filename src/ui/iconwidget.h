#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QBoxLayout>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QPainter>
#include <QProgressBar>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QRect>
#include <QVariant>
#include <QWidget>

#include <cmath>

class IconWidget : public QWidget {
    Q_OBJECT
   public:
    explicit IconWidget(QWidget *parent = nullptr);

    void addImageFromFile(const QString &fileName);
    //    void addImageFromUrl();
    //    void setImageFromData();

    void setImage(QImage i);
    QImage image() const;
    void setImageData(QByteArray ba);
    QByteArray &imageData();
    void setImageInfo(QFileInfo fi);
    QFileInfo imageInfo() const;

    static QString sizeUnit(qint64 size);

   signals:
    void onDeleteBtnClicked(IconWidget *);
    void onViewBtnClicked(IconWidget *);

   protected:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;

   private:
    void setShadow();
    void setDeleteBtn();
    void setViewBtn();
    void setInfo();

    QImage pix;
    QFileInfo info;
    QByteArray data;
    QImage thumb;

    bool hover = false;
    QGraphicsDropShadowEffect *shadow;
    QVBoxLayout *infoBox;
    QHBoxLayout *bottomLine;

    const int INFO_SPACE = 164;
};

#endif  // ICONWIDGET_H
