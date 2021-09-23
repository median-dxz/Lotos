#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPainter>
#include <QPushButton>


class PaintWindow: public QMainWindow
{

public:
    PaintWindow(QMainWindow *parent = 0);
    void paintArc(QPainter *painter);
    void paintErr(QPainter *painter);
    void paintSuc(QPainter * painter);
    QPushButton * Hide ;
    QString text;
    QString tit;
    int Animation=0;
    void reSetTime();
    QTimer *pTimer = new QTimer;



signals:

protected:
    void paintEvent(QPaintEvent *);

private:
    int second1 = 0;
    int second2= 0;
    int second3= 0;
    int traceSec=0;
    int d=160;//错误圆的直径
    void gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QColor color);
    void painting(int k,QPainter * painter);


private slots:
    void updatePaintArc();
    void updatePaintErr();
    void updatePaint();


};

#endif // PAINTWINDOW_H
