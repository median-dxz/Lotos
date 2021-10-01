#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QFrame>
#include <QObject>

class OpacityWithShadowEffectsGroup;
class NotificationManager;
class Notification;

class QLabel;
class QTimeLine;

class Notification : public QFrame {
    Q_OBJECT
   public:
    Notification(QWidget *p, QString tl, QString mes, QString bc, QString fc, int pos);
    static const int GAP_SPACE = 12;
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
    QTimeLine *a_geo_up;
    void animation();

   signals:
    void beforeClose(Notification *);

    friend class NotificationManager;
};

class NotificationManager : QObject {
    Q_OBJECT
   public:
    static NotificationManager &Instance();

    void setNotifyParent(QWidget *value);
   public slots:
    void newNotify(QWidget *parent, QString title, QString message, QString bgC = "#ffffff", QString fgC = "#303133");

   private:
    explicit NotificationManager(QObject *parent = nullptr);
    NotificationManager(const NotificationManager &) Q_DECL_EQ_DELETE;
    NotificationManager &operator=(NotificationManager rhs) Q_DECL_EQ_DELETE;

    QList<Notification *> notifications;
    QWidget *p;

   public:
    struct NOTIFY_FACTORY {
        struct parac {
            QString bgC;
            QString fgC;
        };
        struct parat {
            QString title;
            QString message;
            int id;
        };

        const static parac INFO;
        const static parac ERROR;
        const static parac SUCCESS;
        const static parac WARN;

        static parat networkError(QString _1, QString _2) {
            return {tr("网络错误"), QString(tr("status code: %1\nmessage: %2")).arg(_1, _2), 0};
        }

        static parat uploadSucceed(QString _1) { return {tr("上传成功"), QString(tr("%1")).arg(_1), 1}; }

        static parat uploadFailed(QString _1, QString _2) {
            return {tr("上传失败"), QString(tr("%1\nmessage: %2")).arg(_1, _2), 2};
        }

        static parat uploadCanceled() {
            return {tr("上传取消"), tr("注意:如果上传进度达到100%,则已经上传到服务器,只是未响应回复请求"), 3};
        }

        static parat imageFileError(QString _1) {
            return {tr("错误"), QString(tr("打开图片文件失败，文件无效或不存在\n文件名: %1")).arg(_1), 4};
        }

        static parat imageRepeated(QString _1) { return {tr("错误"), QString(tr("重复的文件\n%1")).arg(_1), 5}; }

        static parat imageNotSupported() {
            return {tr("错误"), tr("不是受支持的格式\nSupport Formats: (*.png *.jpg *webp *.gif *.bmp)"), 6};
        }

        static parat imageWidgetLimit(int _1) {
            return {tr("错误"), QString(tr("待传区图片数量已达上限(limit: %1)")).arg(_1), 7};
        }

        static parat imageWidgetNoPending() { return {tr("信息"), tr("没有可以上传的文件"), 8}; }

        static parat imageClipBoardNotSave(QString _1) {
            return {tr("错误"), QString(("未能成功保存剪辑板图片,请检查保存文件名是否合法:\n%1")).arg(_1), 9};
        }
    };
   public slots:
    void newNotify(NOTIFY_FACTORY::parac c, NOTIFY_FACTORY::parat t);
};

typedef NotificationManager::NOTIFY_FACTORY NOTIFYS;

#endif  // NOTIFICATION_H
