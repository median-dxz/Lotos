#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QPushButton>
#include <QPainter>
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

    const QList<QString> iconPaths = QList<QString>()
    <<(":/res/icons/page_1.png")
    <<(":/res/icons/page_1_ig.png")
    <<(":/res/icons/page_2.png")
    <<(":/res/icons/page_2_ig.png")
    <<(":/res/icons/page_3.png")
    <<(":/res/icons/page_3_ig.png")
    <<(":/res/icons/page_4.png")
    <<(":/res/icons/page_4_ig.png");



   public slots:
   signals:
    void widgetPageChanged(int);




   protected:
    bool eventFilter(QObject *obj, QEvent *event) override;


   private:
    Ui::MainWindow *ui;
    bool loadQStyleSheet(const QString &fileName);
    void componentsLayoutManager();
    void test();
    void httpAccessTest(MainWindow *p);
};

#endif  // MAINWINDOW_H
