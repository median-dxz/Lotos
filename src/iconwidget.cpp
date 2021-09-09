#include "iconwidget.h"

iconwidget::iconwidget(QWidget *parent) : QWidget(parent)
{

}


void iconwidget::paintEvent(QPaintEvent *)
{

        QPainter painter(this);
        QRect rect ;
        rect.setRect(5,5,200,200);
        QPixmap pix;
        pix.load(pixpath);
        painter.drawPixmap(rect,pix);

        if(pixpath!=nullptr)
        {
            painter.setFont(QFont("宋体",5,1));
            painter.drawText(QRect(30,210,60,60),name);
            painter.drawText(QRect(100,210,60,60),QString::number(float(size)/1024)+"KB");
            painter.setPen(QColor(139, 139, 139));
            painter.drawLine(0, 0, this->width() - 1, 0);
            painter.drawLine(0, 0, 0, this->height() - 1);
            painter.drawLine(this->width() - 1, 0, this->width() - 1, this->height() - 1);
            painter.drawLine(0, this->height() - 1, this->width() - 1, this->height() - 1   );
        }

}


void iconwidget::setshadow()
{
    QGraphicsDropShadowEffect * shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(5,5);
    //shadow->setColor(QColor(43,43,43));
    shadow->setBlurRadius(6);
    this->setGraphicsEffect(shadow);




}

