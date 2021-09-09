#ifndef PAGEBUTTON_H
#define PAGEBUTTON_H

#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QLabel>

QT_BEGIN_NAMESPACE

class PageButton : public QPushButton {
    Q_OBJECT
   public:
    explicit PageButton(QWidget *parent = nullptr);

    void setIndex(int);
    void setIconPath(QString path, QString pathIgnited);
    void drawIcon(bool ignited);
    void DrawPix(QString iconPath);
    QString & getnormalIconPath();
    QString & getignitedIconPath();


   public slots:
    void emitClicked();
    void setCurrentChosen(int index);

   signals:
    void indexChanged(int);

   private:
    int index;
    QLabel *label = new QLabel(this);
    QString normalIconPath;
    QString ignitedIconPath;
};

#endif  // PAGEBUTTON_H
