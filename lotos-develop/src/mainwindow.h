#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    bool LoadQStyleSheet(const QString &fileName);
    //void ChangeChosen(QPushButton *Pagebutton,int index);
    //Pagebutton  *PageB;

public slots:
    void  dealSlot(int,bool);

private:
    //bool chosen[4];
    Pagebutton PageB[4];
    Ui::MainWindow *ui;
};


#endif  // MAINWINDOW_H
