#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QPushButton>


/*
 * 实现拖动窗口
 */
#include <QMoveEvent>

#include "pagebutton.h"
#include "utils\httpclient.h"

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
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void minwindow();
    void closewindow();
    void onmainwindowclosed();

   public slots:
   signals:
    void widgetPageChanged(int);

   private:
    Ui::MainWindow *ui;

    QPoint move_point;//移动的距离
    bool mouse_press;//按下鼠标左键
    bool loadQStyleSheet(const QString &fileName);
    void test();
    void HttpAccessTest(MainWindow *p);
};

#endif  // MAINWINDOW_H
