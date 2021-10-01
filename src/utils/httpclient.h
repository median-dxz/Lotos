#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QtNetwork>

//维护单一网络请求
class HttpClient : public QObject {
    Q_OBJECT
   public:
    explicit HttpClient(QObject *parent = nullptr);
    explicit HttpClient(QString url, QObject *parent = nullptr);

    bool setUrl(QString url);
    void setHeaders(QMap<QString, QVariant> headers);

    //全局单一实例
    static QNetworkAccessManager &getNetworkAccessManagerInstanse();
    static QNetworkProxy &getNetworkProxyInstanse();

    void setProxy();
    void setNoProxy();
    void setLocalProxy();

    enum TYPE_REQUEST_FINISHED { DOWNLOAD, UPLOAD, FETCH };

    struct Response {
        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        QByteArray data;
        QVariant statusCode;
        bool isSucceeded;
        QVariant ERROR_INFO;
        void setEncode(const char *code) { codec->codecForName(code); }
        QString getText() { return codec->toUnicode(data); }
        QJsonDocument getJson() { return QJsonDocument::fromJson(data); }
    };

   public slots:
    void cancel();
    void get();
    void get(QUrl url);  //简单访问重载

    void post(QByteArray *data);

    void downloadFile();
    void uploadFile(QByteArray *data, QString name, QString fileName);

   signals:
    void responseFinished(Response *);
    void downloadProgress(qint64, qint64);
    void uplpodProgress(qint64, qint64);

   private:
    QUrl Url;
    QMap<QString, QVariant> Headers;
    QNetworkReply *reply = nullptr;

    TYPE_REQUEST_FINISHED REQUEST_TYPE;

   private slots:
    void onResponseFiniehed();
};

#endif  // HTTPCLIENT_H
