#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include <QFile>
#include <QMap>
#include <QObject>
#include <QVariant>

class SettingsHelper {
   public:
    SettingsHelper();
    void load(const QString fileName);
    void save(const QString fileName);
    void setDefaultValue();

   private:
    QMap<QString, QVariant> imghost;
    QMap<QString, QVariant> user;

    friend class MainWindow;
};

#endif  // SETTINGSHELPER_H
