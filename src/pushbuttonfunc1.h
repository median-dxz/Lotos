#ifndef PUSHBUTTONFUNC1_H
#define PUSHBUTTONFUNC1_H

#include <QPushButton>

class Pushbuttonfunc1 : public QPushButton
{
    Q_OBJECT
public:
    //explicit Pushbuttonfunc1(QWidget *parent = nullptr);
    Pushbuttonfunc1(QString normal , QString floating ="", QString press = "" );


    QString normalpath;
    QString presspath;
    QString floatingpath;

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
//    void mouseMoveEvent(QMouseEvent *);
    void enterEvent(QEvent *ev);
    void leaveEvent(QEvent *ev);

    QSize * beginsize =new QSize(100,100);









signals:

public slots:
};

#endif // PUSHBUTTONFUNC1_H
