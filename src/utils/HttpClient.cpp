#include "HttpClient.h"

HttpClient::HttpClient(QObject *parent) : QObject(parent) {
    requestManager = new QNetworkAccessManager(this);
    codec = QTextCodec::codecForName("utf-8");
    connect(requestManager, &QNetworkAccessManager::finished, this, &HttpClient::onResponseFiniehed);
}

void HttpClient::get(QUrl url) {
    requestManager->get(QNetworkRequest(url));
}

void HttpClient::post(QUrl url) {
    requestManager->get(QNetworkRequest(url));
}

void HttpClient::setEncode(const char *code) {
    codec->codecForName(code);
}

void HttpClient::onResponseFiniehed(QNetworkReply *reply) {
    Response *response = new Response();
    response->data = reply->readAll();
    response->text = codec->toUnicode(response->data);
    response->status = 200;
    emit responseFinished(response);
    reply->deleteLater();
}
