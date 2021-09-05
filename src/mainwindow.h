#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QPushButton>

#include "imagehost.h"
#include "pagebutton.h"
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
   signals:
    void widgetPageChanged(int);

   protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

   private:
    Ui::MainWindow *ui;
    QNetworkProxy proxy;
    SMMS *smms;
    void init();

    bool loadQStyleSheet(const QString &fileName);
    void componentsLayoutManager();

    void test();
};

#endif  // MAINWINDOW_H
