#include "HttpClient.h"

HttpClient::HttpClient(QObject *parent) : QObject(parent) {
    requestManager = new QNetworkAccessManager(this);
    codec = QTextCodec::codecForName("utf-8");
    connect(requestManager, &QNetworkAccessManager::finished, this, &HttpClient::onResponseFiniehed);
}

void HttpClient::setEncode(const char *code) {
    codec->codecForName(code);
}

void HttpClient::onResponseFiniehed(QNetworkReply *reply) {
    Response *response = new Response();
    response->data = reply->readAll();
    response->status = 200;
    emit responseFinished(response);
    reply->deleteLater();
}
