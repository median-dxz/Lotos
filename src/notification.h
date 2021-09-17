#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QDialog>
#include <QLabel>
#include <QTimer>
#include <QString>
#include <QPropertyAnimation>
#include <QColor>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QFont>
#include <QPushButton>
#include <QPalette>



class Notification: public QDialog
{
public:
    Notification(QWidget *parent = 0);

    void set(QString t, QString c, QString color);
    void fadeindown();
    void fadeoutdown();
private:
    QLabel *title;
    QLabel *core;
    QLabel *margin;

};

#endif // NOTIFICATION_H
