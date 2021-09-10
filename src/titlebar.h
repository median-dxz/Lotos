#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QObject>
#include <QPainter>
#include <QPushButton>
#include <QSpacerItem>
#include <QWidget>
#include <QWindow>

class TitleBar : public QWidget {
    Q_OBJECT
   public:
    TitleBar(QWidget *parent);

    void setIcon(const QPixmap &icon);
    void setTitle(const QString str);

   signals:
    void onMiniBtnClicked();
    void onCloseBtnClicked();

   protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

   private:
    QPushButton *miniBtn = new QPushButton(this);
    QPushButton *closeBtn = new QPushButton(this);
    QLabel *mainIcon = new QLabel(this);
    QLabel *title = new QLabel(this);

    bool mbtnHover = false;
    bool cbtnHover = false;
};

#endif  // TITLEBAR_H
