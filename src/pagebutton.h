#ifndef PAGEBUTTON_H
#define PAGEBUTTON_H

#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>

QT_BEGIN_NAMESPACE

class PageButton : public QPushButton {
    Q_OBJECT
   public:
    explicit PageButton(QWidget *parent = nullptr);

    void setIndex(int);
    void setIconPath(QString path, QString pathIgnited);
    void drawIcon(bool ignited);

   public slots:
    void emitClicked();
    void setCurrentChosen(int index);

   signals:
    void indexChanged(int);

   private:
    int index;

    QString normalIconPath;
    QString ignitedIconPath;
};

#endif  // PAGEBUTTON_H
