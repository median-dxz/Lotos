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
public:
    explicit iconwidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *);
    void setshadow();
    QString pixpath,name;
    qint64 size;
};

#endif // ICONWIDGET_H
