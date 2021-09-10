#include "settingshelper.h"

SettingsHelper::SettingsHelper() {
    setDefaultValue();
}

bool SettingsHelper::load(const QString fileName) {
    configFileName = fileName;
    QFile configFile(configFileName);
    if (configFile.exists()) {
        if (!configFile.open(QIODevice::ReadOnly)) {
            qDebug() << QStringLiteral("Couldn't open save file.");
            return false;
        }
        QJsonParseError *prase_err = new QJsonParseError();
        QJsonDocument settings = QJsonDocument::fromJson(configFile.readAll(), prase_err);
        if (settings.isNull()) {
            qDebug() << prase_err->errorString();
            return false;
        }
        imghost = settings["host"].toVariant().toMap();
        user = settings["user"].toVariant().toMap();
    } else {
        if (!configFile.open(QIODevice::WriteOnly)) {
            qDebug() << QStringLiteral("Couldn't open save file.");
            return false;
        }
        setDefaultValue();
        save();
    }
    return true;
}

void SettingsHelper::save() {
    QFile configFile(configFileName);
    QJsonDocument data;
    QVariantMap settings;
    settings.insert("host", imghost);
    settings.insert("user", user);

    data = QJsonDocument::fromVariant(settings);

    configFile.open(QIODevice::WriteOnly);
    configFile.write(data.toJson());
}

void SettingsHelper::setDefaultValue() {
    imghost.insert("token", "");
    imghost.insert("isAuthorized", false);
}
