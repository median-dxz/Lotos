#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QFile>
#include <QMainWindow>
#include <QPushButton>

#include "pagebutton.h"

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
    bool loadQStyleSheet(const QString &fileName);

   public slots:
   signals:
    void widgetPageChanged(int);

   private:
    Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_H
