#ifndef PICTUREVIEWWIDGET_H
#define PICTUREVIEWWIDGET_H

#include <QFileInfo>
#include <QWidget>

class QLabel;

class PictureViewWidget : public QWidget {
    Q_OBJECT
   public:
    static PictureViewWidget &Instance();
    void init();

   protected:
    void showEvent(QShowEvent *) override;
    void hideEvent(QHideEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;
   public slots:
    void setMainWidget(QWidget *mainWidget);
    void setBgColor(const QColor &bgColor);
    void setOpacity(double opacity);
    void showInfo(const QByteArray &ba, QFileInfo i);

   private:
    explicit PictureViewWidget(QWidget *parent = nullptr);
    PictureViewWidget(const PictureViewWidget &other);

    QLabel *imgBox;
    QLabel *info;

    QWidget *mainWidget;  //需要遮罩的主窗体
};

#endif  // PICTUREVIEWWIDGET_H
