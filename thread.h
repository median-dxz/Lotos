#ifndef THREAD_H
#define THREAD_H

#include <QThread>

class Thread : public QThread{
    Q_OBJECT

public:
    Thread();  //构造函数
    void setCallback(int(* func)());

signals:
    void returnResult(int);
    void deleLoading();

protected:
    void run();

private:
    int(* myfunc)();
};

#endif // THREAD_H
