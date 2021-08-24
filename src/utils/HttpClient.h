#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QWidget>
#include <QtNetwork>

namespace Lotos {

struct Response {
    QByteArray data;
    int status;
    bool isSucceeded;
    QString ERROR_TYPE;
};

struct Resquest {
    QUrl url;
    QString params;
    QList<QString> headers;
};

class HttpClient : public QObject {
    Q_OBJECT
   public:
    explicit HttpClient(QObject *parent = nullptr);

    void setEncode(const char *code);
    void setHeaders(QMap<QString, QString> headers);
    void setAgent(QString ip, int port);
    void appendHeaders(QMap<QString, QString> headers);
    void get();
    void get(QUrl url, QString params = "");
    void get(QUrl url, QMap<QString, QString> params);

    void post();
    void post(QUrl url, QString params);
    void post(QUrl url, QMap<QString, QString> params);
    QString parseURLSearchParams();
    QString parseFormData();
    QString stringifyURLSearchParams();
    QString stringifyFormData();
    void downloadFile();
    void uploadFile();

   signals:
    void responseFinished(Response *);

   private:
    QNetworkAccessManager *requestManager;
    QTextCodec *codec;

   private slots:
    void onResponseFiniehed(QNetworkReply *);
};

}  // namespace Lotos

using namespace Lotos;

#endif  // HTTPCLIENT_H
