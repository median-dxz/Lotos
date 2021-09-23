#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "settingshelper.h"

#include "utils/httpclient.h"
#include "utils/imagehost.h"
#include "utils/lotoshelper.h"

#include "iconwidget.h"
#include "notification.h"
#include "pagebutton.h"
#include "pictureviewwidget.h"
#include "titlebar.h"

using namespace LotosSettings;

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

   private slots:
    void onHostLoginClicked();
    void onHostResetClicked();
    void onMainProcessClosed();
    void onSelectFilesButtonClicked();
    void onUploadButtonClicked();

    void addIconWidget(QString filename);
    void delIconWidget(IconWidget *obj);
    void uploadFromIconWidget(IconWidget *iconwidget);

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
    enum PAGE { UploadPage, GalleryPage, HostDashBoardPage, SettingsPage };

    Ui::MainWindow *ui;
    QNetworkProxy proxy;
    SMMS *smms;
    SettingsHelper globalSettings;
    NotificationManager *notify;

    int currentPage = 0;
    int uploadBoxCols = 0;

    void init();

    bool loadQStyleSheet(const QString &fileName);
    void componentsLayoutManager();
    void appearanceManager();
    void loadPage(PAGE index);

    void test();

    bool mousePressed = false;
    QPoint movingPoint;

    QList<IconWidget *> iconWidgets;

    const int UPLOAD_FILE_LIMIT = 20;
    const QString PATH_CONFIG = "config.json";
    const QList<QString> iconPaths = QList<QString>() << (":/res/icons/page_1.png") << (":/res/icons/page_1_ig.png")
                                                      << (":/res/icons/page_2.png") << (":/res/icons/page_2_ig.png")
                                                      << (":/res/icons/page_3.png") << (":/res/icons/page_3_ig.png")
                                                      << (":/res/icons/page_4.png") << (":/res/icons/page_4_ig.png");
    const QSize iconWidgetSize = QSize(270, 313);
};

#endif  // MAINWINDOW_H
