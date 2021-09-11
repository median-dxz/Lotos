#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QPainter>
#include <QPushButton>
#include <QVector>

#include "iconwidget.h"
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

   public slots:
   signals:
    void widgetPageChanged(int);

   protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

   private:
    Ui::MainWindow *ui;
    bool loadQStyleSheet(const QString &fileName);
    void componentsLayoutManager();

    QList<IconWidget *> iconWidgets;
    void test();

    const QList<QString> iconPaths = QList<QString>() << (":/res/icons/page_1.png") << (":/res/icons/page_1_ig.png")
                                                      << (":/res/icons/page_2.png") << (":/res/icons/page_2_ig.png")
                                                      << (":/res/icons/page_3.png") << (":/res/icons/page_3_ig.png")
                                                      << (":/res/icons/page_4.png") << (":/res/icons/page_4_ig.png");
    const QSize iconWidgetSize = QSize(213, 247);

   private:
    void onUploadButtonClicked();
};

#endif  // MAINWINDOW_H
