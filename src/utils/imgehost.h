#ifndef IMGEHOST_H
#define IMGEHOST_H

#include <QJsonObject>
#include <QObject>
#include <QWidget>

#include "httpclient.h"

struct smmsResponse {
    bool success;
    QString code;
    QString message;
    QJsonArray data;
    QString RequestId;
};

struct smmsProfile {
    QString username;
    QString email;
    QString role;
    QString group_expire;
    int email_verified;
    QString disk_usage;
    QString disk_limit;
    qint64 disk_usage_raw;
    qint64 disk_limit_raw;
};

struct smmsImageInfomation {
    int width;
    int height;
    QString filename;
    QString storename;
    qint64 size;
    QString path;
    QString hash;
    QString url;
    QString delete_link;
    QString page_link;
};

class smms : public QObject {
    Q_OBJECT
   public:
    explicit smms(QObject *parent = nullptr);
    static smms &getInstance() {
        static smms instance;
        return instance;
    }

    smmsResponse getAPIToken(QString username, QString password);
    smmsResponse getUserProfile(smmsProfile &ret);

    smmsResponse clearTemporaryUploadHistory();
    smmsResponse getTemporaryUploadHistory(QList<smmsImageInfomation> &ret);
    smmsResponse getUploadHistory(QList<smmsImageInfomation> &ret);

    smmsResponse upload(const QList<QByteArray> &files, QList<smmsImageInfomation> &ret);

    smmsResponse deleteImage(QString hash);

   private:
    const QString name = "sm.ms";
    const QString basic_api = "https://sm.ms/api/v2";
    QString authorization;
};

#endif  // IMGEHOST_H
