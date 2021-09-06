#include "iconwidget.h"
#include "pagebutton.h"
#include <QDebug>
#include <QBuffer>

iconwidget::iconwidget(QWidget *parent) : QWidget(parent)
{
    if(pixpath!=NULL)
    {
        QBuffer buffer;
        buffer.open(QIODevice::ReadWrite);
        QPixmap p(pixpath);
        p.save(&buffer,"jpg");
        pix.append(buffer.data());

    }


}

//int iconwidget::index= 0;
int iconwidget::dele = 0;

void iconwidget::paintEvent(QPaintEvent *)
{


        QPainter painter(this);
        QPixmap pix;
        pix.load(pixpath);

        QRect rect ;
        rect.setRect(5*pix.width()/pix.height(),5,190*pix.width()/pix.height(),190);
        painter.drawPixmap(rect,pix);


//        if(pixpath!=nullptr)
//        {
            painter.setFont(QFont("雅黑",6,25));
            if(panduan(size)==1)
            {
             QString text = QString("%1  (%2%3) ").arg(name).arg(QString::number(float(size)/1024,'f',2)).arg("KB");
             painter.drawText(QRect(52*pix.width()/pix.height(),205,200,60),text);}
            if(panduan(size)==0)
            {
            QString text = QString("%1  (%2%3) ").arg(name).arg(QString::number(size)).arg("B");
            painter.drawText(QRect(52*pix.width()/pix.height(),205,200,60),text);}

            if(panduan(size)==2)
            {
            QString text = QString("%1  (%2%3) ").arg(name).arg(QString::number(float(size)/1024/1024,'f',2)).arg("MB");
            painter.drawText(QRect(52*pix.width()/pix.height(),205,200,60),text);}

            painter.setPen(QColor(139, 139, 139));
            painter.drawLine(0, 0, this->width() - 1, 0);
            painter.drawLine(0, 0, 0, this->height() - 1);
            painter.drawLine(this->width() - 1, 0, this->width() - 1, this->height() - 1);
            painter.drawLine(0, this->height() - 1, this->width() - 1, this->height() - 1   );


//        }

}


void iconwidget::setshadow()
{
    QGraphicsDropShadowEffect * shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(5,5);
    //shadow->setColor(QColor(43,43,43));
    shadow->setBlurRadius(6);
    this->setGraphicsEffect(shadow);




}

void iconwidget::setdelbtn()
{
     PageButton * btn = new PageButton(this);
     btn->setFixedSize(QSize(25,25));
     btn->setIcon(QIcon(":/res/icons/del.png"));
     btn->setGeometry(this->width()-btn->width() , this->height()-btn->height(),25,25);
     //btn->setText("删除");
     btn->show();
     connect(btn, &QPushButton::clicked, this, [=]() {
             delete this;
             dele = 1;
             //qDebug()<<dele;
         });


}

int iconwidget::panduan(qint64 size)
{
    if(size<1024)
        return  0;

    else if(size>=1024*1024)
        return  2;

    else return 1;


}

