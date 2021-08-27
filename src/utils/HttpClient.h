#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QtNetwork>

enum TYPE_REQUEST_FINISHED { DOWNLOAD, UPLOAD, FETCH };

class Response {
   public:
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QByteArray data;
    QVariant statusCode;
    bool isSucceeded;
    QVariant ERROR_INFO;
    void setEncode(const char *code) { codec->codecForName(code); }
    QString getText() { return codec->toUnicode(data); }
    QJsonDocument getJson() { return QJsonDocument::fromBinaryData(data); }
};

class HttpClient : public QObject {
    Q_OBJECT
   public:
    explicit HttpClient(QObject *parent = nullptr);
    explicit HttpClient(QString url);

    bool setUrl(QString url);
    void setHeaders(QMap<QString, QVariant> headers);

    void get();
    void get(QUrl url);  //对简单请求的重载

    void post(QByteArray *data);

    void downloadFile();
    void uploadFile(QByteArray *data, QString name, QString fileName);

    //获取全局实例
    QNetworkAccessManager &getNetworkAccessManagerInstanse();
    QNetworkProxy &getNetworkProxyInstanse();

    void setProxy();
    void setNoProxy();
    void setLocalProxy();
   signals:
    void responseFinished(Response *);
    void downloadProgress(qint64, qint64);
    void uplpodProgress(qint64, qint64);

   private:
    QUrl Url;
    QMap<QString, QVariant> Headers;

    TYPE_REQUEST_FINISHED REQUEST_TYPE;

   private slots:
    void onResponseFiniehed();
};

#endif  // HTTPCLIENT_H
