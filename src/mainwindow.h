#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QPushButton>
#include <QPainter>
#include <QPen>
#include <QBrush>


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

    void closewindow();
    void onmainwindowclosed();

   public slots:
   signals:
    void widgetPageChanged(int);

   private:
    Ui::MainWindow *ui;

    QPoint move_point;//移动的距离
    bool mouse_press = false;//按下鼠标左键
    QPushButton *minibutton;
    QPushButton *closebutton;
    int h_min=0;
    int h_cls=0;
    bool loadQStyleSheet(const QString &fileName);
    void test();
    void HttpAccessTest(MainWindow *p);
protected:
    void mousePressEvent(QMouseEvent * event)  override;
    void mouseReleaseEvent(QMouseEvent * event)override;
    void mouseMoveEvent(QMouseEvent * event)   override;
    bool eventFilter(QObject *watched, QEvent *event) override;


};

#endif  // MAINWINDOW_H
