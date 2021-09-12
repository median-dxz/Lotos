#ifndef PICTUREVIEWWIDGET_H
#define PICTUREVIEWWIDGET_H

#include <QApplication>
#include <QObject>
#include <QScreen>
#include <QWidget>

class PictureViewWidget : public QWidget {
    Q_OBJECT
   public:
    static PictureViewWidget &Instance();

   protected:
    void showEvent(QShowEvent *);

   public slots:
    void setMainWidget(QWidget *mainWidget);
    void setBgColor(const QColor &bgColor);
    void setOpacity(double opacity);

   private:
    explicit PictureViewWidget(QWidget *parent = nullptr);
    PictureViewWidget(const PictureViewWidget &other);
    QWidget *mainWidget;  //需要遮罩的主窗体
};

#endif  // PICTUREVIEWWIDGET_H
