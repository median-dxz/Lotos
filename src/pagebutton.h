#ifndef PAGEBUTTON_H
#define PAGEBUTTON_H

#include <QPushButton>

QT_BEGIN_NAMESPACE

class PageButton : public QPushButton {
    Q_OBJECT
   public:
    explicit PageButton(QWidget *parent = nullptr);

    void setIndex(int);

   public slots:
    void emitClicked();
    void setCurrentChosen(int index);

   signals:
    void indexChanged(int);

   private:
    int index;
};

#endif  // PAGEBUTTON_H
