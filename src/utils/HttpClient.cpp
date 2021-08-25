#include "HttpClient.h"

HttpClient::HttpClient(QObject *parent) : QObject(parent) {
    requestManager = new QNetworkAccessManager(this);
    agentIp = "";
    agentPort = 0;
    isUsingAgent = false;

    connect(requestManager, &QNetworkAccessManager::finished, this, &HttpClient::onResponseFiniehed);
}

void HttpClient::onResponseFiniehed(QNetworkReply *reply) {
    Response *response = new Response();
    response->data = reply->readAll();
    response->status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    emit responseFinished(response);
    reply->deleteLater();
}

void HttpClient::cancelAgent() {
    isUsingAgent = false;
}

void HttpClient::useAgent() {
    isUsingAgent = true;
}

void HttpClient::setAgent(QString ip, int port) {
    agentIp = ip;
    agentPort = port;
}

void HttpClient::getAgent(QString &ip, int &port) {
    ip = agentIp;
    port = agentPort;
}
