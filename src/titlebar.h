#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

class QPushButton;
class QLabel;

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
    QPushButton *miniBtn;
    QPushButton *closeBtn;
    QLabel *mainIcon;
    QLabel *title;

    bool mbtnHover = false;
    bool cbtnHover = false;
};

#endif  // TITLEBAR_H
