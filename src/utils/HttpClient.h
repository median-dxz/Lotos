#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QWidget>
#include <QtNetwork>

struct Response {
    QTextCodec *codec;
    QByteArray data;
    QVariant status;
    bool isSucceeded;
    QString ERROR_TYPE;
    // setEncode(const char *code) codec->codecForName(code); codec = QTextCodec::codecForName("utf-8"); QString getText() {
    // codec->toUnicode() void setEncode(const char *code);return QString().data }
};

struct Resquest {
    QUrl url;
    QString params;
    QList<QString> headers;
    QString parseURLSearchParams();
    QString parseFormData();
    QString stringifyURLSearchParams();
    QString stringifyFormData();
};

class HttpClient : public QObject {
    Q_OBJECT
   public:
    explicit HttpClient(QObject *parent = nullptr);

    void setHeaders(QMap<QString, QVariant> headers);
    void appendHeaders(QMap<QString, QVariant> headers);

    void get();
    void get(QUrl url, QString params = "");
    void get(QUrl url, QMap<QString, QVariant> params);

    void post();
    void post(QUrl url, QString params);
    void post(QUrl url, QMap<QString, QVariant> params);

    void downloadFile();
    void uploadFile();

    void setAgent(QString ip, int port);
    void getAgent(QString &ip, int &port);

    void useAgent();
    void cancelAgent();

   signals:
    void responseFinished(Response *);

   private:
    QNetworkAccessManager *requestManager;
    bool isUsingAgent;
    QString agentIp;
    int agentPort;

   private slots:
    void onResponseFiniehed(QNetworkReply *);
};

#endif  // HTTPCLIENT_H
