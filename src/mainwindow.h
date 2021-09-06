#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "imagehost.h"
#include "pagebutton.h"
#include "settingshelper.h"
#include "utils/httpclient.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   public slots:
    void onHostLoginClicked();
    void onHostResetClicked();

   signals:
    void widgetPageChanged(int);

   protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

   private:
    Ui::MainWindow *ui;
    QNetworkProxy proxy;

    SMMS *smms;
    SettingsHelper globalSettings;

    void init();

    bool loadQStyleSheet(const QString &fileName);
    void componentsLayoutManager();

    void test();
};

#endif  // MAINWINDOW_H
