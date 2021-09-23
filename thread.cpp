#include "thread.h"

#include <QMainWindow>
Thread::Thread()
{


}

void Thread::run()
{
    myfunc(mm ,MM);
    emit returnResult(1,"111","www");  //将结果放在信号中
    emit deleLoading();
}

void Thread::setCallback(MainWindow *m,PaintWindow *M,int(* func)(MainWindow *m ,PaintWindow *M))
{
    mm = m;
    MM = M;
//    mm->hide();
//    mm->Painting(1);
//    MM->show();
    myfunc = func;
}


