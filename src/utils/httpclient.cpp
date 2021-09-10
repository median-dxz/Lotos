#include "httpclient.h"

HttpClient::HttpClient(QObject *parent) : QObject(parent) {}

HttpClient::HttpClient(QString url) : Url(url) {}

void HttpClient::get() {
    QNetworkRequest request = QNetworkRequest(Url);
    for (auto i = Headers.begin(); i != Headers.end(); i++) {
        request.setRawHeader(i.key().toLatin1(), i.value().toByteArray());
    }
    connect(getNetworkAccessManagerInstanse().get(request), &QNetworkReply::finished, this,
            &HttpClient::onResponseFiniehed);
}

void HttpClient::get(QUrl url) {
    connect(getNetworkAccessManagerInstanse().get(QNetworkRequest(url)), &QNetworkReply::finished, this,
            &HttpClient::onResponseFiniehed);
}

void HttpClient::post(QByteArray *data) {
    QNetworkRequest request = QNetworkRequest(Url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");  // default type
    for (auto i = Headers.begin(); i != Headers.end(); i++) {
        request.setRawHeader(i.key().toLatin1(), i.value().toByteArray());
    }

    connect(getNetworkAccessManagerInstanse().post(request, *data), &QNetworkReply::finished, this,
            &HttpClient::onResponseFiniehed);
}

void HttpClient::downloadFile() {
    QNetworkRequest request = QNetworkRequest(Url);

    for (auto i = Headers.begin(); i != Headers.end(); i++) {
        request.setRawHeader(i.key().toLatin1(), i.value().toByteArray());
    }

    QNetworkReply *reply = getNetworkAccessManagerInstanse().get(request);
    connect(reply, &QNetworkReply::downloadProgress, this,
            [=](qint64 bytesReceived, qint64 bytesTotal) { emit downloadProgress(bytesReceived, bytesTotal); });
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onResponseFiniehed);
}

void HttpClient::uploadFile(QByteArray *data, QString name, QString fileName) {
    QNetworkRequest request = QNetworkRequest(Url);

    // set file body
    QHttpMultiPart *multi_part = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart image_part;
    image_part.setHeader(QNetworkRequest::ContentDispositionHeader,
                         QVariant(QString("form-data; name=\"%1\";filename=\"%2\";").arg(name, fileName)));
    image_part.setBody(*data);
    multi_part->append(image_part);

    for (auto i = Headers.begin(); i != Headers.end(); i++) {
        request.setRawHeader(i.key().toLatin1(), i.value().toByteArray());
    }

    QNetworkReply *reply = getNetworkAccessManagerInstanse().post(request, multi_part);
    connect(reply, &QNetworkReply::uploadProgress, this,
            [=](qint64 bytesSent, qint64 bytesTotal) { emit uplpodProgress(bytesSent, bytesTotal); });
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onResponseFiniehed);
}

void HttpClient::onResponseFiniehed() {
    QNetworkReply *reply = (QNetworkReply *)sender();  // get reply

    Response *response = new Response();
    response->data = reply->readAll();
    response->statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    emit responseFinished(response);

    reply->deleteLater();  // destory clinet obj later
    deleteLater();
}

bool HttpClient::setUrl(QString url) {
    Url = QUrl(url);
    return Url.isValid();
}

void HttpClient::setHeaders(QMap<QString, QVariant> headers) {
    Headers = headers;
}

void HttpClient::setProxy() {
    getNetworkAccessManagerInstanse().setProxy(getNetworkProxyInstanse());
}

void HttpClient::setLocalProxy() {
    getNetworkAccessManagerInstanse().setProxy(QNetworkProxy::DefaultProxy);
}

void HttpClient::setNoProxy() {
    getNetworkAccessManagerInstanse().setProxy(QNetworkProxy::NoProxy);
}

QNetworkAccessManager &HttpClient::getNetworkAccessManagerInstanse() {
    static QNetworkAccessManager globalRequestManager;
    return globalRequestManager;
}

QNetworkProxy &HttpClient::getNetworkProxyInstanse() {
    static QNetworkProxy globalProxyManager;
    return globalProxyManager;
}
