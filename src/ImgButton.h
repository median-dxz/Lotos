#ifndef IMGBUTTON_H
#define IMGBUTTON_H

#include <QEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>


class ImgButton : public QPushButton {
    Q_OBJECT
   public:
    explicit ImgButton(QWidget *parent = nullptr);
    ImgButton(QString normal, QString floating = "", QString press = "");
    void SetIconPath(QString normal, QString hover, QString press);

   signals:

   public slots:

   protected:
    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

   private:
    QString normalIconPath;
    QString pressIconPath;
    QString hoverIconPath;
    void DrawIcon(QString iconPath);
};

#endif  // IMGBUTTON_H
