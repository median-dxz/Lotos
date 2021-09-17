#include "thread.h"

#include <QMainWindow>
Thread::Thread()
{
    //
}

void Thread::run()
{
//    int result = 0;
//    for(int i = 0; i < 2000000000; i++)
//        result += i;

    emit returnResult(myfunc());  //将结果放在信号中
    emit deleLoading();
}

void Thread::setCallback(int(* func)())
{
    myfunc = func;
}
