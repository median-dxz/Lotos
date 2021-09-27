#ifndef PAGEBUTTON_H
#define PAGEBUTTON_H

#include <QPushButton>

class QLabel;
class QGraphicsDropShadowEffect;

class PageButton : public QPushButton {
    Q_OBJECT
   public:
    explicit PageButton(QWidget *parent = nullptr);

    void setIndex(int index);
    int Index() const;
    bool isChecked() const;
    void setChecked(bool c);

    void setIconOffset(int x, int y);
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
    bool chosen = false;
    QLabel *iconLabel;
    QPoint iconOffset;
    QString normalIconPath;
    QString ignitedIconPath;

    QGraphicsDropShadowEffect *shadow;
};

#endif  // PAGEBUTTON_H
