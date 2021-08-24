#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "utils\HttpClient.h"

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

   private:
    Ui::MainWindow *ui;
};
using namespace Lotos;
#endif  // MAINWINDOW_H
