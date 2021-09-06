#include "settingshelper.h"

SettingsHelper::SettingsHelper() {
    setDefaultValue();
}

void SettingsHelper::load(const QString fileName) {
    QFile *f = new QFile(fileName);
    if (f->exists()) {
    } else {
        setDefaultValue();
    }
}

void SettingsHelper::setDefaultValue() {
    imghost.insert("token", "");
    imghost.insert("isAuthorized", false);
}
