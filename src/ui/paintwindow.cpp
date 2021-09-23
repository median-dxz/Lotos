#include "paintwindow.h"

PaintWindow::PaintWindow(QMainWindow * )
{

    this->setFixedSize(QSize(400,300));
    connect(pTimer, &QTimer::timeout, this, &PaintWindow::updatePaintArc);
    connect(pTimer, &QTimer::timeout, this, &PaintWindow::updatePaintErr);
    connect(pTimer, &QTimer::timeout, this, &PaintWindow::updatePaint);

    Hide = new QPushButton(this);
    Hide->move(width()-Hide->width(),height()-Hide->height());
    Hide->setText("back");

}

void PaintWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.translate(width() >> 1, (height() >> 1));
    painter.setFont(QFont("Microsoft YaHei", 12, 400));
    painter.setPen(QColor("#CCC"));
    painter.drawText(QRect(-60, 80, width(), 40), text);
    painter.drawText(QRect(-60, 110, width(), 40), tit);
    //painter.setRenderHint(QPainter::Antialiasing, true);
    //paintArc(&painter);
    //paintTuo(&painter);
    //paintSuc(&painter);
    painting(Animation , &painter);


    update();





}


void PaintWindow::paintArc(QPainter *painter)
{

    painter->rotate(second3);
    gradientArc(painter, 40, 0,  90, 8, QColor(39,120,196));
    gradientArc(painter, 40, 180,  90, 8, QColor(39,120,196));


}

void PaintWindow:: paintErr(QPainter *painter)
{
    QPen pen(QColor(242, 116, 116),8);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);
    if(second1<d)
    painter->drawEllipse(QRect(-d/2,-(second1/2.0),d, second1));
    else {
        painter->drawEllipse(QRect(-d/2,-d/2,d, d));
    }

    if(second1>d&&second1<d+30)
    {

        painter ->drawLine(0,d-second1,d/4,d/4+d-second1);
        painter ->drawLine(0,d-second1,-d/4,-d/4+d-second1);
        painter ->drawLine(0,d-second1,-d/4,d/4+d-second1);
        painter ->drawLine(0,d-second1,d/4,-d/4+d-second1);


    }
    if(second1>d+30)
    {

        painter ->drawLine(0,-d-60+second1,d/4,d/4-d-60+second1);
        painter ->drawLine(0,-d-60+second1,-d/4,-d/4-d-60+second1);
        painter ->drawLine(0,-d-60+second1,-d/4,d/4-d-60+second1);
        painter ->drawLine(0,-d-60+second1,d/4,-d/4-d-60+second1);

    }



}

void PaintWindow::paintSuc(QPainter * painter)
{

     QPen pen(QColor(165, 220, 134),8);
     pen.setCapStyle(Qt::RoundCap);
      painter->setPen(pen);

      if(second2>220)
      {
          if(traceSec<55)
            painter->drawLine(-70,-25,-70+traceSec,traceSec-25);
          if(traceSec>=55)
          {
              if(traceSec>=95)
              {
                  int j =95;
                  painter->drawLine(-45-25+traceSec-j,0-25+traceSec-j,-45,0);
              }
              else
                  painter->drawLine(-70,-25,-45,0);

              painter->drawLine(-45,0,-15,30);
              painter->drawLine(-15,30,-15+traceSec-55,30-traceSec+55);
          }

      }


      gradientArc(painter, 80, 0,  360, 5, QColor(228,244,218));//这个是画圆环的
      gradientArc(painter, 80, 270,  second2, 5, QColor(165, 220, 134));

      if(second2>=220)
      {
          gradientArc(painter, 80, 270,  traceSec*220.0/95, 5, QColor(228,244,218));
          gradientArc(painter, 80, 160, 110, 5, QColor(228,244,218));
      }

}

void PaintWindow::gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QColor color)
{

    painter->setBrush(QColor(color) );
    QRectF rect(-radius, -radius, radius << 1, radius << 1);
    QPainterPath path;
    path.arcTo(rect, startAngle, angleLength);
    QPainterPath subPath;
    subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));

    // path为扇形 subPath为椭圆
    path -= subPath;

    painter->setPen(Qt::NoPen);
    painter->drawPath(path);
}


void PaintWindow::updatePaintArc()
{
    second2++;
    if(second2>220)
    {
        traceSec++;
        if (traceSec > 120)
            traceSec = 120;
    }

    if (second2 > 360)
        second2 = 360;
    update();
}



void PaintWindow::updatePaintErr()
{
    second1++;
    if (second1 > d+60)
        second1 = d+60;
    update();
}


void PaintWindow::updatePaint()
{
    second3++;
    if (second3 > 360)
        second3 = 0;
    update();

}

void PaintWindow::painting(int Animation ,QPainter * painter)
{

    switch (Animation) {
    case 0:paintSuc(painter);
           break;
    case 1:paintErr(painter);
           break;
    case 2:paintArc(painter);
           break;
    }

}

void PaintWindow::reSetTime()
{
    second1 = 0;
    second2= 0;
    second3= 0;
    traceSec=0;
}
