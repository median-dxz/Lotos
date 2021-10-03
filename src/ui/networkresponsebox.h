#ifndef NETWORKRESPONSEBOX_H
#define NETWORKRESPONSEBOX_H

#include <QWidget>
#include "messagebox.h"

class NetworkResponseBox : public MessageBox {
    Q_OBJECT
   public:
    explicit NetworkResponseBox(QWidget *parent = nullptr);
   public slots:
    void setTip(const QString &text);

   private:
    QLabel *lab_tip;
};

#endif  // NETWORKRESPONSEBOX_H
