#ifndef LINKCOPYBOX_H
#define LINKCOPYBOX_H

#include <QWidget>
#include "messagebox.h"

class QLabel;
class QLineEdit;
class PictureTableLine;

class LinkCopyBox : public MessageBox {
    Q_OBJECT
   public:
    explicit LinkCopyBox(QWidget *parent = nullptr);

   protected:
    bool eventFilter(QObject *, QEvent *) override;

   public slots:
    void setLink(int index, QString link);

   private:
    QLabel *lab[6];
    QLineEdit *edt[6];
};

#endif  // LINKCOPYBOX_H
