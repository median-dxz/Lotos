#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QFrame>
#include <QObject>

class OpacityWithShadowEffectsGroup;
class NotificationManager;
class Notification;

class QLabel;

class Notification : public QFrame {
    Q_OBJECT
   public:
    Notification(QWidget *p, QString tl, QString mes, QString bc, QString fc, int pos);
    static const int GAP_SPACE = 16;
    static const int FIXED_WIDTH = 330;
    static const int MIN_HEIGHT = 56;
    static const int NOTIFY_DURATION = 3500;

   protected:
    void paintEvent(QPaintEvent *) override;

   private:
    QLabel *title;
    QLabel *message;

    OpacityWithShadowEffectsGroup *effect;
    int index;
    void animation();

   signals:
    void beforeClose(Notification *);

    friend class NotificationManager;
};

class NotificationManager : QObject {
    Q_OBJECT
   public:
    static NotificationManager &Instance();
   public slots:
    void newNotify(QWidget *parent, QString title, QString message, QString bgC = "#fff", QString fgC = "#303133");

   private:
    explicit NotificationManager(QObject *parent = nullptr);
    NotificationManager(const NotificationManager &) Q_DECL_EQ_DELETE;
    NotificationManager &operator=(NotificationManager rhs) Q_DECL_EQ_DELETE;

    QList<Notification *> notifications;
    QTimer *fixTop;
};

#endif  // NOTIFICATION_H
