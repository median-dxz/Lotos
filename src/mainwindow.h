#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "settingshelper.h"

#include "utils/httpclient.h"
#include "utils/imagehost.h"
#include "utils/lotoshelper.h"

#include "iconwidget.h"
#include "linkcopybox.h"
#include "loginbox.h"
#include "messagebox.h"
#include "notification.h"
#include "pictureviewwidget.h"
#include "titlebar.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    enum PAGE { UploadPage, GalleryPage, HostDashBoardPage, SettingsPage };

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private slots:
    void onHostLoginClicked();
    void onButtonHostResetClicked();
    void onMainProcessClosed();
    void onButtonSelectFilesClicked();
    void onButtonUploadClicked();

    void uploadImage(IconWidget *obj);

    void loadPage(int index);
    void onUploadStatusChanged();

    MessageBox *openMessagegBox();
    void closeMessageBox(int msecs = 0);

   signals:
    void widgetPageChanged(int);
    void uploadStatusChanged();

   protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

   private:
    Ui::MainWindow *ui;
    QNetworkProxy proxy;
    SMMS *smms;
    SettingsHelper globalSettings;
    NotificationManager *notify;

    int currentPage = 0;

    void init();

    void componentsManager();
    void appearanceManager();

    void test();

    bool mousePressed = false;
    QPoint movingPoint;

    QFrame *maskFrame;
    MessageBox *msgBox;

    const QString PATH_CONFIG = "config.json";
    const QList<QString> iconPaths = QList<QString>() << (":/res/icons/page_1.png") << (":/res/icons/page_1_ig.png")
                                                      << (":/res/icons/page_2.png") << (":/res/icons/page_2_ig.png")
                                                      << (":/res/icons/page_3.png") << (":/res/icons/page_3_ig.png")
                                                      << (":/res/icons/page_4.png") << (":/res/icons/page_4_ig.png");
};

#endif  // MAINWINDOW_H
