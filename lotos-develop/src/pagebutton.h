#ifndef PAGEBUTTON_H
#define PAGEBUTTON_H

#include<QPushButton>


QT_BEGIN_NAMESPACE

class Pagebutton : public QPushButton{
        Q_OBJECT
public:
    QPushButton Pageb_1;
    explicit Pagebutton( QPushButton *parent = nullptr);
    void sendSlot();
    void Setindex(int);
    void ChangeChosen();
    void ChangeFalseChosen();
    bool GetChosen();
    void ChangeText();
signals:
    void mysignal(int,bool);
private:
    int index;
    bool IsChosen=false;
};

#endif // PAGEBUTTON_H
