#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include <QtCore>

namespace LotosSettings {

const struct {
    const QString imghost_token = "token";
    const QString imghost_isAuthorized = "isAuthorized";
} KeyMap;

}  // namespace LotosSettings

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
