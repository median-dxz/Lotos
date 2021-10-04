#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include <QtCore>

namespace LotosSettings {

const static struct {
    struct {
        const QString token = "token";
        const QString isAuthorized = "isAuthorized";
    } imghost;
    struct {
        const QString uploadWithToken = "uploadWithToken";
        const QString clipSaveFileName = "clipSaveFileName";
        const QString clipSaveImageType = "clipSaveImageType";
    } user;
} KeyMap;

}  // namespace LotosSettings

inline const char *LotosVersion() {
    return QLatin1String(LOTOS_VERSION).data();
}

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
