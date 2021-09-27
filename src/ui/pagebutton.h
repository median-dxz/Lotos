#ifndef PAGEBUTTON_H
#define PAGEBUTTON_H

#include <QPushButton>

class QLabel;
class QGraphicsDropShadowEffect;

class PageButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(int bgC READ getBgC WRITE setBgC NOTIFY bgCChanged)
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

    int getBgC() const;
    void setBgC(const int &value);

   public slots:
    void emitClicked();
    void setCurrentChosen(int index);

   protected:
    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

   signals:
    void indexChanged(int);
    void bgCChanged(int);

   private:
    int index;
    bool chosen = false;
    bool hover = false;
    int bgC = 0;
    QLabel *iconLabel;
    QPoint iconOffset;
    QString normalIconPath;
    QString ignitedIconPath;

    QGraphicsDropShadowEffect *shadow;
};

#endif  // PAGEBUTTON_H
