#ifndef PAGEBUTTON_H
#define PAGEBUTTON_H

#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>

class PageButton : public QPushButton {
    Q_OBJECT
   public:
    explicit PageButton(QWidget *parent = nullptr);

    void setIndex(int index);
    void setIconOffset(int x, int y);
    int Index() const;
    void setIconPath(QString pathNormal, QString pathIgnited);

    void drawPix(const QString iconPath);
    QString getNormalIconPath() const;
    QString getIgnitedIconPath() const;

   public slots:
    void emitClicked();
    void setCurrentChosen(int index);

   protected:
    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;

   signals:
    void indexChanged(int);

   private:
    int index;
    QLabel *iconLabel = new QLabel(this);
    QPoint iconOffset;
    QString normalIconPath;
    QString ignitedIconPath;
};

#endif  // PAGEBUTTON_H
