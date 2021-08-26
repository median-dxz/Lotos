#include "pagebutton.h"

Pagebutton::Pagebutton(QPushButton *parent):QPushButton(parent)
{
    //Pageb_1.setParent();
    Pageb_1.show();
    connect(&Pageb_1,&QPushButton::clicked,this,&Pagebutton::sendSlot);



}
void Pagebutton::sendSlot(){

    emit mysignal(this->index,this->IsChosen);
}
void Pagebutton::Setindex(int index){
    this->index=index;
}
void Pagebutton::ChangeChosen(){
    if(this->IsChosen==false) IsChosen=true;
}
bool Pagebutton::GetChosen(){
    return this->IsChosen;
}
void Pagebutton::ChangeText(){
    if( this->IsChosen == true)Pageb_1.setText("^_^ ");
    else{Pageb_1.setText("@_@");}
}
void Pagebutton::ChangeFalseChosen(){
    this->IsChosen = false;
}
