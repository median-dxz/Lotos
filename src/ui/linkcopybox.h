#ifndef LINKCOPYBOX_H
#define LINKCOPYBOX_H

#include <QWidget>
#include "messagebox.h"

class LinkCopyBox : public MessageBox {
    Q_OBJECT
   public:
    explicit LinkCopyBox(QWidget *parent = nullptr);
};

#endif  // LINKCOPYBOX_H
