#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

<<<<<<< HEAD
<<<<<<< HEAD
#include "ImgButton.h"
#include "utils\HttpClient.h"
=======
#include "imgbutton.h"
=======
>>>>>>> 2920bae (refactor: 删除ImgaeButton实现)
#include "pagebutton.h"
#include "utils\httpclient.h"
>>>>>>> e2619e0 (sync: 修复与整理阶段性工作，修复之前阶段性工作提交的问题)

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

    bool LoadQStyleSheet(const QString &fileName);

   protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

   private:
    Ui::MainWindow *ui;
<<<<<<< HEAD
=======

    bool loadQStyleSheet(const QString &fileName);
    void componentsLayoutManager();
>>>>>>> 614284c (pref: 优化图标绘制，整理主窗口代码)
    void test();
    void httpAccessTest(MainWindow *p);
};

#endif  // MAINWINDOW_H
