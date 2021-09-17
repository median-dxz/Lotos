#include "notification.h"

Notification::Notification(QWidget *)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(330,75.2);


    QPushButton *button = new QPushButton(this);
    button->resize(16,16);
    button->move(this->width()-16-8,5);



     QPropertyAnimation *animation = new QPropertyAnimation(this,"windowOpacity");
     animation->setDuration(4000);
     animation->setStartValue(1);
     animation->setEndValue(0);
     animation->start();
     connect(animation,SIGNAL(finished()),this,SLOT(close()));
     connect(button,&QPushButton::clicked,this,this->hide);
}
void Notification::set(QString t, QString c ,QString color){

    QFrame *frame = new QFrame(this);
    frame->setStyleSheet("QFrame{background-color:"+QString(color)+";border-radius:10px}");//设置圆角与背景透明
    frame->setGeometry(5,5,this->width()-10,this->height()-10); //设置有效范围框
    title = new QLabel(this);
    title->resize(87,22.6);
    title->move(10,10.2);
    core = new QLabel(this);
    core->resize(114,20.8);
    core->move(10,34);

    QGraphicsDropShadowEffect *shadow_effect = new  QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(0,0);
    shadow_effect->setColor(Qt::black);
    shadow_effect->setBlurRadius(10);
    frame->setGraphicsEffect(shadow_effect);
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    QFont font;
    font.setFamily("微软雅黑");
    font.setPixelSize(14);
    font.setBold(true);//字体加粗
    core->setFont(font);
    //core->setPalette(); //可设置字体颜色
    core->setText(QString(c));
    font.setPixelSize(16);
    title->setFont(font);
    title->setText(QString(t));
}

void Notification::fadeindown(){
    QRect r = QRect();
    r.setX(this->x());
    r.setY(this->y() - this->height());
    r.setHeight(this->height());
    r.setWidth(this->width());
    QRect r2 = r;
    r2.setX(this->x());
    r2.setY(this->y());
    r2.setHeight(this->height());
    r2.setWidth(this->width());
    QPropertyAnimation *a = new QPropertyAnimation(this,"geometry");
    a->setDuration( 150 );
    a->setStartValue( r );
    a->setEndValue( r2 );
    a->start();
    this->show();
}

void Notification::fadeoutdown(){
    QRect r = QRect();
    r.setX(this->x());
    r.setY(this->y());
    r.setHeight(this->height());
    r.setWidth(this->width());
    QRect r2 = r;
    r2.setX(this->x());
    r2.setY(this->y()+ this->height());
    r2.setHeight(this->height());
    r2.setWidth(this->width());
    QPropertyAnimation *a = new QPropertyAnimation(this,"geometry");
    a->setDuration( 500 );
    a->setStartValue( r );
    a->setEndValue( r2 );
    a->start();
    this->show();
}

