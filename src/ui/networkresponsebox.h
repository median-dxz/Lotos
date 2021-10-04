#ifndef NETWORKRESPONSEBOX_H
#define NETWORKRESPONSEBOX_H

#include <QWidget>
#include "messagebox.h"

class QProgressBar;
class QPropertyAnimation;

class NetworkResponseBox : public MessageBox {
    Q_OBJECT
   public:
    explicit NetworkResponseBox(QWidget *parent = nullptr);
   public slots:
    void setTip(const QString &text);
    void setProgress(int v);

   protected:
    void resizeEvent(QResizeEvent *) override;

   private:
    QLabel *lab_tip;
    QProgressBar *progress;
};

#endif  // NETWORKRESPONSEBOX_H
