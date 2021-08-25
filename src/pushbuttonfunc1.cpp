#include "pushbuttonfunc1.h"
#include <QMouseEvent>
#include <QDebug>
//Pushbuttonfunc1::Pushbuttonfunc1(QWidget *parent) : QPushButton(parent)
//{


//}


Pushbuttonfunc1:: Pushbuttonfunc1(QString normal , QString floating , QString press  )
{

    setMouseTracking(true);
    this->normalpath = normal;
    this->presspath = press;
    this->floatingpath = floating;


    QPixmap pix;
    pix.load(normalpath);

    this->setFixedSize(*beginsize);
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);

    this->setIconSize(*beginsize);


}



void Pushbuttonfunc1::mouseReleaseEvent(QMouseEvent *ev)
{
    QPixmap pix;
    pix.load(this->floatingpath);
    this->setFixedSize(*beginsize);
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);

    this->setIconSize(*beginsize);
    return QPushButton::mouseReleaseEvent(ev);


}



void Pushbuttonfunc1:: mousePressEvent(QMouseEvent *ev)
{

    QPixmap pix;
    pix.load(this->presspath);
    this->setFixedSize(*beginsize);
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);

    this->setIconSize(*beginsize);




     return QPushButton::mousePressEvent(ev);




}

void Pushbuttonfunc1:: enterEvent(QEvent *)
{
        QPixmap pix;
        pix.load(this->floatingpath);
        this->setFixedSize(*beginsize);
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);

        this->setIconSize(*beginsize);




}
void Pushbuttonfunc1::leaveEvent(QEvent *)
{

    QPixmap pix;
    pix.load(normalpath);

    this->setFixedSize(*beginsize);
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);

    this->setIconSize(*beginsize);



}

