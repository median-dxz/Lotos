#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include <QDebug>
#include <QFile>
#include <QMap>
#include <QObject>
#include <QVariant>
#include <QtNetwork>

const struct {
    const QString imghost_token = "token";
    const QString imghost_isAuthorized = "isAuthorized";
} KeyMap;

class SettingsHelper {
   public:
    SettingsHelper();
    bool load(const QString fileName);
    void save();
    void setDefaultValue();

   private:
    QVariantMap imghost;
    QVariantMap user;
    QString configFileName;

    friend class MainWindow;
};

#endif  // SETTINGSHELPER_H
