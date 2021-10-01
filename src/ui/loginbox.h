#ifndef LOGINBOX_H
#define LOGINBOX_H

#include <QWidget>
#include "messagebox.h"

class LoginBox : public MessageBox {
    Q_OBJECT
   public:
    explicit LoginBox(QWidget *parent = nullptr);
};

#endif  // LOGINBOX_H
