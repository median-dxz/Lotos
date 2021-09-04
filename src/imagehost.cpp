#include "imagehost.h"

SMMS::SMMS(QObject *parent, QString api) : QObject(parent), basic_api(api) {}

SMMS &SMMS::getInstance() {
    static SMMS instance;
    return instance;
}

QString SMMS::token() {
    return authorization;
}

void SMMS::setToken(QString token) {
    authorization = token;
}

void SMMS::praseResponse(const QJsonDocument &data, Response &res) {
    res.RequestId = data["RequestId"].toString();
    res.code = data["code"].toString();
    res.message = data["message"].toString();
    res.success = data["success"].toBool();
    if (data["data"].isArray()) {
        res.data = data["data"].toArray();
    } else if (data["data"].isObject()) {
        res.data.append(data["data"].toObject());
    }
}

void SMMS::praseImageInfomation(const QJsonObject &data, SMMS::ImageInfomation &res) {
    res.delete_link = data["delete"].toString();
    res.page_link = data["page"].toString();
    res.filename = data["filename"].toString();
    res.storename = data["storename"].toString();
    res.hash = data["hash"].toString();
    res.height = data["height"].toInt();
    res.width = data["width"].toInt();
    res.path = data["path"].toString();
    res.size = data["size"].toDouble();
    res.url = data["url"].toString();
}

void SMMS::praseUserProfile(const QJsonObject &data, UserProfile &res) {
    res.disk_limit = data["disk_limit"].toString();
    res.disk_limit_raw = data["disk_limit_raw"].toDouble();
    res.disk_usage = data["disk_usage"].toString();
    res.disk_usage_raw = data["disk_usage_raw"].toDouble();
    res.email = data["email"].toString();
    res.email_verified = data["email_verified"].toInt();
    res.group_expire = data["group_expire"].toString();
    res.role = data["role"].toString();
    res.username = data["username"].toString();
}

HttpClient *SMMS::getAPIToken(QString username, QString password) {
    HttpClient *client = new HttpClient();
    QByteArray *data = new QByteArray();
    QUrlQuery query;

    query.addQueryItem("username", username);
    query.addQueryItem("password", password);
    data->append(query.query());

    client->setUrl(basic_api + "/token");
    client->post(data);

    return client;
}

HttpClient *SMMS::getUserProfile() const {
    HttpClient *client = new HttpClient();
    QMap<QString, QVariant> headers;
    headers["Authorization"] = authorization;

    client->setUrl(basic_api + "/profile");
    client->setHeaders(headers);
    client->post(new QByteArray());

    return client;
}

HttpClient *SMMS::clearTemporaryUploadHistory() const {
    HttpClient *client = new HttpClient(basic_api + "/clear");

    client->get();

    return client;
}

HttpClient *SMMS::getTemporaryUploadHistory() const {
    HttpClient *client = new HttpClient(basic_api + "/history");

    client->get();

    return client;
}

HttpClient *SMMS::getUploadHistory(qint64 page) const {
    HttpClient *client = new HttpClient();

    QMap<QString, QVariant> headers;
    QUrlQuery query;
    QUrl req = QUrl(basic_api + "/upload_history");

    headers["Authorization"] = authorization;
    query.addQueryItem("page", QString::number(page));
    req.setQuery(query);

    client->setUrl(req.toString());
    client->setHeaders(headers);
    client->get();

    return client;
}

HttpClient *SMMS::upload(QByteArray &file, const QString filename, bool useToken) const {
    HttpClient *client = new HttpClient();
    QMap<QString, QVariant> headers;
    headers["Authorization"] = authorization;

    if (useToken)
        client->setHeaders(headers);
    client->setUrl(basic_api + "/upload");

    client->uploadFile(&file, "smfile", filename);

    return client;
}

HttpClient *SMMS::deleteImage(QString hash) const {
    HttpClient *client = new HttpClient();

    client->setUrl(basic_api + QString("/delete/%1").arg(hash));
    client->get();

    return client;
}
