#ifndef IMGEHOST_H
#define IMGEHOST_H

#include <QObject>

#include "utils/httpclient.h"

class SMMS : public QObject {
    Q_OBJECT
   public:
    static SMMS &Instance();

    struct Response {
        bool success;
        QString code;
        QString message;
        QJsonArray data;
        QString RequestId;
    };

    struct UserProfile {
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

    struct ImageInfomation {
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
        qint64 timestamp;
        bool token_with;

        QVariantMap toQVariantMap() const;
    };

    QString token();
    void setToken(QString token);

    HttpClient *getAPIToken(QString username, QString password);
    HttpClient *getUserProfile() const;

    HttpClient *clearTemporaryUploadHistory() const;
    HttpClient *getTemporaryUploadHistory() const;
    HttpClient *getUploadHistory(qint64 page = 1) const;

    HttpClient *upload(QByteArray &file, const QString filename, bool useToken = false) const;

    HttpClient *deleteImage(QString hash) const;

    static void praseResponse(const QJsonDocument &data, Response &res);
    static void praseImageInfomation(const QJsonObject &data, ImageInfomation &res);
    static void praseUserProfile(const QJsonObject &data, UserProfile &res);
    static QString supportFormat(const QByteArray &data);

   private:
    explicit SMMS(QObject *parent = nullptr, QString api = "https://sm.ms/api/v2");
    SMMS(const SMMS &other);

    const QString name = "sm.ms";
    const QString basic_api;
    QString authorization;
};

#endif  // IMGEHOST_H
