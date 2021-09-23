#ifndef THREAD_H
#define THREAD_H


#include <QThread>
#include "paintwindow.h"
class MainWindow;
#include "mainwindow.h"

class Thread : public QThread{
    Q_OBJECT

public:
    Thread();  //构造函数
    void setCallback(MainWindow *m,PaintWindow *M,int(* func)(MainWindow *m ,PaintWindow *M));

signals:
    void returnResult(int,QString ,QString);
    void deleLoading();

protected:
    void run();

private:
    int(* myfunc)(MainWindow *m ,PaintWindow *M);
    MainWindow *mm=nullptr;
    PaintWindow *MM=nullptr;


};

#endif // THREAD_H
