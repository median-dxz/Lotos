#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QWidget>
#include <QWidget>
#include <QPainter>
#include <QRect>
#include <QFont>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QFile>
#include <QFileDialog>

class iconwidget : public QWidget
{

    QByteArray pix;
public:
    explicit iconwidget(QWidget *parent = nullptr);
    static int dele;
    void setshadow();
    void setdelbtn();
    int panduan(qint64 size);

    QString pixpath,name;
    qint64 size;

protected:
    void paintEvent(QPaintEvent *)override;


};

#endif // ICONWIDGET_H
